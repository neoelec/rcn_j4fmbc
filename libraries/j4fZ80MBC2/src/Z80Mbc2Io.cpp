// SPDX-License-Identifier: GPL-3.0-or-later

#pragma GCC optimize("O2")

#include "Z80Mbc2Cfg.h"
#include "Z80Mbc2Io.h"

void Z80Mbc2Io::begin(Z80Mbc2Dev &dev)
{
  __beginIoDev(dev);
  __initFromCfg(dev);

  last_rx_is_empty_ = 0;
}

void Z80Mbc2Io::__beginIoDev(Z80Mbc2Dev &dev)
{
  auto *gpio = dev.getGpio();
  auto *user = dev.getUser();
  auto *disk = dev.getDisk();

  rtc_ = dev.getRtc();
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

  rd_datetime_.begin(rtc_);

  wr_userled_.begin(user);
  rd_userkey_.begin(user);

  wr_setbank_.begin(pin_);
}

void Z80Mbc2Io::__initFromCfg(Z80Mbc2Dev &dev)
{
  Z80Mbc2Cfg cfg;

  cfg.begin(dev.getSd());

  autoexec_en_ = cfg.getAutoExecEn();
  wait_count_ = 1 << cfg.getClkMode();
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
  pin_->setPIN_nBUSREQ_LOW();    // Request for a DMA
  pin_->setPIN_WAIT_nRES_LOW();  // Reset WAIT FF exiting from WAIT state
  delayMicroseconds(wait_count_);
  pin_->setPIN_WAIT_nRES_HIGH(); // Now Z80 is in DMA, so it's safe set WAIT_RES_ HIGH again
  pin_->setPIN_nBUSREQ_HIGH();   // Resume Z80 from DMA
}

inline void Z80Mbc2Io::__execWriteCommand(void)
{
  switch (getCommand())
  {
  case MbcIo::WR_USERLED:
    wr_userled_.run(*this);
    break;

  case MbcIo::WR_SERIALTX:
    wr_serialtx_.run(*this);
    break;

  case MbcIo::WR_GPIOA:
    wr_gpioa_.run(*this);
    break;

  case MbcIo::WR_GPIOB:
    wr_gpiob_.run(*this);
    break;

  case MbcIo::WR_IODIRA:
    wr_iodira_.run(*this);
    break;

  case MbcIo::WR_IODIRB:
    wr_iodirb_.run(*this);
    break;

  case MbcIo::WR_GPPUA:
    wr_gppua_.run(*this);
    break;

  case MbcIo::WR_GPPUB:
    wr_gppub_.run(*this);
    break;

  case MbcIo::WR_SELDISK:
    wr_seldisk_.run(*this);
    break;

  case MbcIo::WR_SELTRACK:
    wr_seltrack_.run(*this);
    break;

  case MbcIo::WR_SELSECT:
    wr_selsect_.run(*this);
    break;

  case MbcIo::WR_WRITESECT:
    wr_writesect_.run(*this);
    break;

  case MbcIo::WR_SETBANK:
    wr_setbank_.run(*this);
    break;

  default:
    rdwr_nop_.run(*this);
  }
}

inline void Z80Mbc2Io::__execReadCommand(void)
{
  switch (getCommand())
  {
  case MbcIo::RD_USERKEY:
    rd_userkey_.run(*this);
    break;

  case MbcIo::RD_GPIOA:
    rd_gpioa_.run(*this);
    break;

  case MbcIo::RD_GPIOB:
    rd_gpiob_.run(*this);
    break;

  case MbcIo::RD_SYSFLAGS:
    __runRdSYSFLAGS();
    break;

  case MbcIo::RD_DATETIME:
    rd_datetime_.run(*this);
    break;

  case MbcIo::RD_ERRDISK:
    rd_errdisk_.run(*this);
    break;

  case MbcIo::RD_READSECT:
    rd_readsect_.run(*this);
    break;

  case MbcIo::RD_SDMOUNT:
    rd_sdmount_.run(*this);
    break;

  default:
    rdwr_nop_.run(*this);
  }
}

// SYSFLAGS (Various system flags for the OS):
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                              X  X  X  X  X  X  X  0    AUTOEXEC not enabled
//                              X  X  X  X  X  X  X  1    AUTOEXEC enabled
//                              X  X  X  X  X  X  0  X    DS3231 RTC not found
//                              X  X  X  X  X  X  1  X    DS3231 RTC found
//                              X  X  X  X  X  0  X  X    Serial RX buffer empty
//                              X  X  X  X  X  1  X  X    Serial RX char available
//                              X  X  X  X  0  X  X  X    Previous RX char valid
//                              X  X  X  X  1  X  X  X    Previous RX char was a "buffer empty" flag
//
// NOTE: Currently only D0-D3 are used
inline void Z80Mbc2Io::__runRdSYSFLAGS(void)
{
  uint8_t io_data = autoexec_en_;

  io_data |= (rtc_->isAvailable()) << 1;
  io_data |= (!!Serial.available()) << 2;
  io_data |= (!!last_rx_is_empty_) << 3;
  setData(io_data);
  setCommand(MbcIo::NO_OPERATION);
}