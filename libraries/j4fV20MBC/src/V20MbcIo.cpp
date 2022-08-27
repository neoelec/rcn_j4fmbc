// SPDX-License-Identifier: GPL-3.0-or-later

#pragma GCC optimize("O2")

#include "V20MbcCfg.h"
#include "V20MbcIo.h"

void V20MbcIo::begin(V20MbcDev &dev)
{
  staticSysFlags_ = 0;

  __beginIoDev(dev);
  __initFromCfg(dev);
  __initIoDevWr(dev);
  __initIoDevRd(dev);
}

void V20MbcIo::__beginIoDev(V20MbcDev &dev)
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

  staticSysFlags_ |= rtc->isAvailable() << MbcDevRdSYSFLAGS::RTC;
}

void V20MbcIo::__initFromCfg(V20MbcDev &dev)
{
  V20MbcCfg cfg;

  cfg.begin(dev.getSd());

  staticSysFlags_ |= cfg.getAutoExecEn() << MbcDevRdSYSFLAGS::AUTOEXEC;
  wait_count_ = 1 << cfg.getClkMode();
}

void V20MbcIo::__initIoDevWr(V20MbcDev &dev)
{
  auto *gpio = dev.getGpio();

  for (uint8_t command = MbcIo::WR_BEGIN; command <= MbcIo::WR_END; command++)
    __setIoDevWr(command, &rdwr_nop_);

  __setIoDevWr(MbcIo::WR_USERLED, &wr_userled_);
  __setIoDevWr(MbcIo::WR_SERIALTX, &wr_serialtx_);
  __setIoDevWr(MbcIo::WR_RXIRQFLAG, &wr_rxirqflag_);
  __setIoDevWr(MbcIo::WR_SELDISK, &wr_seldisk_);
  __setIoDevWr(MbcIo::WR_SELTRACK, &wr_seltrack_);
  __setIoDevWr(MbcIo::WR_SELSECT, &wr_selsect_);
  __setIoDevWr(MbcIo::WR_WRITESECT, &wr_writesect_);

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

void V20MbcIo::__setIoDevWr(uint8_t command, MbcDev *dev)
{
  io_dev_wr_[command - MbcIo::WR_BEGIN] = dev;
}

void V20MbcIo::__initIoDevRd(V20MbcDev &dev)
{
  auto *gpio = dev.getGpio();

  for (uint8_t command = MbcIo::RD_BEGIN; command <= MbcIo::RD_END; command++)
    __setIoDevRd(command, &rdwr_nop_);

  __setIoDevRd(MbcIo::RD_USERKEY, &rd_userkey_);
  __setIoDevRd(MbcIo::RD_DATETIME, &rd_datetime_);
  __setIoDevRd(MbcIo::RD_ERRDISK, &rd_errdisk_);
  __setIoDevRd(MbcIo::RD_READSECT, &rd_readsect_);
  __setIoDevRd(MbcIo::RD_SDMOUNT, &rd_sdmount_);
  __setIoDevRd(MbcIo::RD_ATXBUFF, &rd_atxbuff_);

  if (gpio->isAvailable())
  {
    __setIoDevRd(MbcIo::RD_GPIOA, &rd_gpioa_);
    __setIoDevRd(MbcIo::RD_GPIOB, &rd_gpiob_);
  }
}

void V20MbcIo::__setIoDevRd(uint8_t command, MbcDev *dev)
{
  io_dev_rd_[command - MbcIo::RD_BEGIN] = dev;
}

void V20MbcIo::run(void)
{
  if (!pin_->getPIN_READY())
  {
    if (!pin_->getPIN_nWR())
      __runWrite();
    else if (!pin_->getPIN_nRD())
      __runRead();
    else if (!pin_->getPIN_nINTA())
      __runInterrupt();
    else
      __runHalt();
  }

  if (Serial.available() && getIrq(MbcIo::IRQ_RX))
    pin_->setPIN_INTR_HIGH();
}

uint8_t V20MbcIo::getSysFlag(void)
{
  return staticSysFlags_;
}

inline void V20MbcIo::__runWrite(void)
{
  _setAddress(pin_->getPIN_AD1AD0());
  setData(pin_->getPORT_DATA());

  switch (getAddress())
  {
  case 0: // AD1-AD0 = 0 (I/O write address = 0x00): EXECUTE WRITE OPCODE.
    __execWriteOpcode();
    break;

  case 1: // AD1-AD0 = 1 (I/O write address = 0x01): STORE OPCODE.
    setCommand(getData());
    setCount(0);
    break;
  case 2: // NOT USED - RESERVED
    break;

  case 3: // NOT USED - RESERVED
    break;
  }

  // Control bus sequence to exit from a wait state (I/O bus write cycle)
  pin_->setPIN_nRDYRES_LOW();  // RDYRES_ = LOW: Now is safe reset WAIT FF (exiting from WAIT state)
  pin_->setPIN_nRDYRES_HIGH(); // RDYRES_ = HIGH

  // Time critical section!!!
  noInterrupts();               // !!! Start of a time critical section. No interrupt allowed
  pin_->setPIN_nHOLDRES_LOW();  // !!! HOLDRES_ = LOW: Resume V20 from HiZ (reset HOLD FF)
  pin_->setPIN_nHOLDRES_HIGH(); // !!! HOLDRES_ = HIGH
  interrupts();                 // !!! End of a time critical section. Interrupt resumed
}

inline void V20MbcIo::__runRead(void)
{
  _setAddress(pin_->getPIN_AD1AD0());
  setData(0);

  switch (getAddress())
  {
  case 0: // AD1-AD0 = 0 (I/O read address = 0x00): EXECUTE READ OPCODE.
    __execReadOpcode();
    break;

  case 1: // AD1-AD0 = 1 (I/O read address = 0x01): SERIAL RX.
    rd_serialrx_.run(*this);
    pin_->setPIN_INTR_LOW(); // INTR = LOW: Reset the INTR signal (if used).
    break;

  case 2: // AD1-AD0 = 2 (I/O read address = 0x02): SYSFLAGS.
    rd_sysflags_.run(*this);
    break;

  case 3: // NOT USED - RESERVED
    break;
  }

  // Current output on data bus
  pin_->setPORT_DATA(getData());

  // Control bus sequence to exit from a wait state (M I/O read cycle)
  pin_->setPIN_nRDYRES_LOW();     // RDYRES_ = LOW: Now is safe reset WAIT FF (exiting from WAIT state)
  delayMicroseconds(wait_count_); // Wait 2us just to be sure that the V20 reads the data and goes HiZ
  pin_->releasePORT_DATA();
  pin_->setPIN_nRDYRES_HIGH(); // RDYRES_ = HIGH: Now V20 in HiZ (HOLD), so it's safe deactivate RDYRES_

  // Time critical section!!!
  noInterrupts();               // !!! Start of a time critical section. No interrupt allowed
  pin_->setPIN_nHOLDRES_LOW();  // !!! HOLDRES_ = LOW: Resume V20 from HiZ (reset HOLD FF)
  pin_->setPIN_nHOLDRES_HIGH(); // !!! HOLDRES_ = HIGH
  interrupts();                 // !!! End of a time critical section. Interrupt resumed
}

inline void V20MbcIo::__runInterrupt(void)
{
  // Now CPU is in the first of the two INTA_ bus cycles
  pin_->setPIN_INTR_LOW(); // INTR = LOW: Reset the INTR signal

  // Current output on data bus
  setData(33); // For now it is fixed
  pin_->setPORT_DATA(getData());

  // Control bus sequence to exit from a wait state (Interrupt)
  pin_->setPIN_nRDYRES_LOW();     // RDYRES_ = LOW: Now is safe reset WAIT FF (exiting from WAIT state)
  delayMicroseconds(wait_count_); // Wait 2us (8 bus cycles @ 4MHz) just to be sure to execute both
  delayMicroseconds(wait_count_); //  the two INTA bus cycles
  pin_->releasePORT_DATA();
  pin_->setPIN_nRDYRES_HIGH(); // RDYRES_ = HIGH: Now V20 in HiZ (HOLD), so it's safe deactivate RDYRES_

  // Time critical section!!!
  noInterrupts();               // !!! Start of a time critical section. No interrupt allowed
  pin_->setPIN_nHOLDRES_LOW();  // !!! HOLDRES_ = LOW: Resume V20 from HiZ (reset HOLD FF)
  pin_->setPIN_nHOLDRES_HIGH(); // !!! HOLDRES_ = HIGH
  interrupts();                 // !!! End of a time critical section. Interrupt resumed
}

inline void V20MbcIo::__runHalt(void)
{
  static bool halted;

  if (!halted)
  {
    Serial.println();
    Serial.println(F("IOS: CPU HALT detected"));
    Serial.println();
    halted = true;
  }
}

inline void V20MbcIo::__execWriteOpcode(void)
{
  uint8_t command = getCommand();

  if (command >= MbcIo::WR_BEGIN && command <= MbcIo::WR_END)
    io_dev_wr_[command - MbcIo::WR_BEGIN]->run(*this);
  else
    rdwr_nop_.run(*this);
}

inline void V20MbcIo::__execReadOpcode(void)
{
  uint8_t command = getCommand();

  if (command >= MbcIo::RD_BEGIN && command <= MbcIo::RD_END)
    io_dev_rd_[command - MbcIo::RD_BEGIN]->run(*this);
  else
    rdwr_nop_.run(*this);
}
