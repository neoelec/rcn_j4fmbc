// SPDX-License-Identifier: GPL-3.0-or-later

#pragma GCC optimize("O2")

#include "Z80Mbc2Cfg.h"
#include "Z80Mbc2Io.h"

void Z80Mbc2Io::begin(Z80Mbc2Dev &dev)
{
  staticSysFlags_ = 0;
  last_rx_is_empty_ = 0;

  __beginIoDev(dev);
  __initFromCfg(dev);
  __initIoDevWr(dev);
  __initIoDevRd(dev);
}

void Z80Mbc2Io::__beginIoDev(Z80Mbc2Dev &dev)
{
  auto *gpio = dev.getGpio();
  auto *user = dev.getUser();
  auto *disk = dev.getDisk();
  auto *rtc = dev.getRtc();

  pin_ = dev.getPin();

  wr_seldisk_.begin(disk);
  wr_seltrack_.begin(disk);
  wr_selsect_.begin(disk);
  wr_writesect_.begin(disk);
  rd_errdisk_.begin(disk);
  rd_readsect_.begin(disk);
  rd_sdmount_.begin(disk);

  wr_gpioa_.begin(gpio);
  wr_gpiob_.begin(gpio);
  wr_iodira_.begin(gpio);
  wr_iodirb_.begin(gpio);
  wr_gppua_.begin(gpio);
  wr_gppub_.begin(gpio);
  rd_gpioa_.begin(gpio);
  rd_gpiob_.begin(gpio);

  rd_datetime_.begin(rtc);

  wr_userled_.begin(user);
  rd_userkey_.begin(user);

  wr_setbank_.begin(pin_);

  staticSysFlags_ |= rtc->isAvailable() << MbcDevRdSYSFLAGS::RTC;
}

void Z80Mbc2Io::__initFromCfg(Z80Mbc2Dev &dev)
{
  Z80Mbc2Cfg cfg;

  cfg.begin(dev.getSd());

  staticSysFlags_ |= cfg.getAutoExecEn() << MbcDevRdSYSFLAGS::AUTOEXEC;
  wait_count_ = 1 << cfg.getClkMode();
}

void Z80Mbc2Io::__initIoDevWr(Z80Mbc2Dev &dev)
{
  auto *gpio = dev.getGpio();

  for (uint8_t command = MbcIo::WR_BEGIN; command <= MbcIo::WR_END; command++)
    __setIoDevWr(command, &rdwr_nop_);

  __setIoDevWr(MbcIo::WR_USERLED, &wr_userled_);
  __setIoDevWr(MbcIo::WR_SERIALTX, &wr_serialtx_);
  __setIoDevWr(MbcIo::WR_SELDISK, &wr_seldisk_);
  __setIoDevWr(MbcIo::WR_SELTRACK, &wr_seltrack_);
  __setIoDevWr(MbcIo::WR_SELSECT, &wr_selsect_);
  __setIoDevWr(MbcIo::WR_WRITESECT, &wr_writesect_);
  __setIoDevWr(MbcIo::WR_SETBANK, &wr_setbank_);

  if (gpio->isAvailable())
  {
    __setIoDevWr(MbcIo::WR_GPIOA, &wr_gpioa_);
    __setIoDevWr(MbcIo::WR_GPIOB, &wr_gpiob_);
    __setIoDevWr(MbcIo::WR_IODIRA, &wr_iodira_);
    __setIoDevWr(MbcIo::WR_IODIRB, &wr_iodirb_);
    __setIoDevWr(MbcIo::WR_GPPUA, &wr_gppua_);
    __setIoDevWr(MbcIo::WR_GPPUB, &wr_gppub_);
  }
}

void Z80Mbc2Io::__setIoDevWr(uint8_t command, MbcDev *dev)
{
  io_dev_wr_[command - MbcIo::WR_BEGIN] = dev;
}

void Z80Mbc2Io::__initIoDevRd(Z80Mbc2Dev &dev)
{
  auto *gpio = dev.getGpio();

  for (uint8_t command = MbcIo::RD_BEGIN; command <= MbcIo::RD_END; command++)
    __setIoDevRd(command, &rdwr_nop_);

  __setIoDevRd(MbcIo::RD_USERKEY, &rd_userkey_);
  __setIoDevRd(MbcIo::RD_SYSFLAGS, &rd_sysflags_);
  __setIoDevRd(MbcIo::RD_DATETIME, &rd_datetime_);
  __setIoDevRd(MbcIo::RD_ERRDISK, &rd_errdisk_);
  __setIoDevRd(MbcIo::RD_READSECT, &rd_readsect_);
  __setIoDevRd(MbcIo::RD_SDMOUNT, &rd_sdmount_);

  if (gpio->isAvailable())
  {
    __setIoDevRd(MbcIo::RD_GPIOA, &rd_gpioa_);
    __setIoDevRd(MbcIo::RD_GPIOB, &rd_gpiob_);
  }
}

void Z80Mbc2Io::__setIoDevRd(uint8_t command, MbcDev *dev)
{
  io_dev_rd_[command - MbcIo::RD_BEGIN] = dev;
}

void Z80Mbc2Io::run(void)
{
  if (!pin_->getPIN_nWAIT())
  {
    if (!pin_->getPIN_nWR())
      __runWrite();
    else if (!pin_->getPIN_nRD())
      __runRead();
    else
      __runInterrupt();
  }
}

uint8_t Z80Mbc2Io::getSysFlag(void)
{
  return staticSysFlags_ | ((!!last_rx_is_empty_) << MbcDevRdSYSFLAGS::PREV_RX);
}

inline void Z80Mbc2Io::__runWrite(void)
{
  _setAddress(pin_->getPIN_AD0());
  setData(pin_->getPORT_DATA());

  if (getAddress())
  {
    setCommand(getData());
    setCount(0);
  }
  else
    __execWriteCommand();

  // Control bus sequence to exit from a wait state (M I/O write cycle)
  pin_->setPIN_nBUSREQ_LOW();   // Request for a DMA
  pin_->setPIN_WAIT_nRES_LOW(); // Reset WAIT FF exiting from WAIT state
  delayMicroseconds(wait_count_);
  pin_->setPIN_WAIT_nRES_HIGH(); // Now Z80 is in DMA, so it's safe set WAIT_RES_ HIGH again
  pin_->setPIN_nBUSREQ_HIGH();   // Resume Z80 from DMA
}

inline void Z80Mbc2Io::__runRead(void)
{
  _setAddress(pin_->getPIN_AD0());
  setData(0);

  if (getAddress())
  {
    setData(0xFF);
    if (Serial.available() > 0)
    {
      setData(Serial.read());
      last_rx_is_empty_ = 0;
    }
    else
      last_rx_is_empty_ = 1;

    pin_->setPIN_nINT_HIGH();
  }
  else
    __execReadCommand();

  pin_->setPORT_DATA(getData()); // Configure Z80 data bus D0-D7 (PA0-PA7) as output
                                 // Current output on data bus

  // Control bus sequence to exit from a wait state (M I/O read cycle)
  pin_->setPIN_nBUSREQ_LOW();     // Request for a DMA
  pin_->setPIN_WAIT_nRES_LOW();   // Now is safe reset WAIT FF (exiting from WAIT state)
  delayMicroseconds(wait_count_); // Wait 2us just to be sure that Z80 read the data and go HiZ
  delayMicroseconds(wait_count_); //
  pin_->releasePORT_DATA();       // Configure Z80 data bus D0-D7 (PA0-PA7) as input with pull-up
  pin_->setPIN_WAIT_nRES_HIGH();  // Now Z80 is in DMA (HiZ), so it's safe set WAIT_RES_ HIGH again
  pin_->setPIN_nBUSREQ_HIGH();    // Resume Z80 from DMA
}

inline void Z80Mbc2Io::__runInterrupt(void)
{
  // Control bus sequence to exit from a wait state (M interrupt cycle)
  pin_->setPIN_nBUSREQ_LOW();   // Request for a DMA
  pin_->setPIN_WAIT_nRES_LOW(); // Reset WAIT FF exiting from WAIT state
  delayMicroseconds(wait_count_);
  pin_->setPIN_WAIT_nRES_HIGH(); // Now Z80 is in DMA, so it's safe set WAIT_RES_ HIGH again
  pin_->setPIN_nBUSREQ_HIGH();   // Resume Z80 from DMA
}

inline void Z80Mbc2Io::__execWriteCommand(void)
{
  uint8_t command = getCommand();

  if (command >= MbcIo::WR_BEGIN && command <= MbcIo::WR_END)
    io_dev_wr_[command - MbcIo::WR_BEGIN]->run(*this);
  else
    rdwr_nop_.run(*this);
}

inline void Z80Mbc2Io::__execReadCommand(void)
{
  uint8_t command = getCommand();

  if (command >= MbcIo::RD_BEGIN && command <= MbcIo::RD_END)
    io_dev_rd_[command - MbcIo::RD_BEGIN]->run(*this);
  else
    rdwr_nop_.run(*this);
}
