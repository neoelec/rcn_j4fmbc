// SPDX-License-Identifier: GPL-3.0-or-later

#pragma GCC optimize("O2")

#include "j4fV20Mbc.h"

V20MbcIoClass V20MbcIo;

void V20MbcIoClass::begin(void)
{
  staticSysFlags_ = 0;

  dev_.begin();

  __beginIoDev();
  __initFromCfg();
  __initIoDevWr();
  __initIoDevRd();

  state_ = &st_init_;
}

void V20MbcIoClass::__beginIoDev(void)
{
  auto *gpio = dev_.getGpio();
  auto *user = dev_.getUser();
  auto *disk = dev_.getDisk();
  auto *rtc = dev_.getRtc();

  pin_ = dev_.getPin();

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

void V20MbcIoClass::__initFromCfg(void)
{
  V20MbcCfg cfg;

  cfg.begin(dev_.getSd());

  staticSysFlags_ |= cfg.getAutoExecEn() << MbcDevRdSYSFLAGS::AUTOEXEC;
  wait_count_ = 1 << cfg.getClkMode();
}

void V20MbcIoClass::__initIoDevWr(void)
{
  auto *gpio = dev_.getGpio();

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

void V20MbcIoClass::__setIoDevWr(uint8_t command, MbcDev *dev)
{
  io_dev_wr_[command - MbcIo::WR_BEGIN] = dev;
}

void V20MbcIoClass::__initIoDevRd(void)
{
  auto *gpio = dev_.getGpio();

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

void V20MbcIoClass::__setIoDevRd(uint8_t command, MbcDev *dev)
{
  io_dev_rd_[command - MbcIo::RD_BEGIN] = dev;
}

inline void V20MbcIoClass::runIo(void)
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

uint8_t V20MbcIoClass::getSysFlag(void)
{
  return staticSysFlags_;
}

inline void V20MbcIoClass::__runWrite(void)
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

inline void V20MbcIoClass::__runRead(void)
{
  _setAddress(pin_->getPIN_AD1AD0());
  setData(0);

  switch (getAddress())
  {
  case 0: // AD1-AD0 = 0 (I/O read address = 0x00): EXECUTE READ OPCODE.
    __execReadOpcode();
    break;

  case 1: // AD1-AD0 = 1 (I/O read address = 0x01): SERIAL RX.
    rd_serialrx_.run(this);
    pin_->setPIN_INTR_LOW(); // INTR = LOW: Reset the INTR signal (if used).
    break;

  case 2: // AD1-AD0 = 2 (I/O read address = 0x02): SYSFLAGS.
    rd_sysflags_.run(this);
    break;

  case 3: // NOT USED - RESERVED
    break;
  }

  // Current output on data bus
  pin_->setPORT_DATA(getData());

  // Control bus sequence to exit from a wait state (M I/O read cycle)
  pin_->setPIN_nRDYRES_LOW();     // RDYRES_ = LOW: Now is safe reset WAIT FF (exiting from WAIT state)
  delayMicroseconds(wait_count_); // Wait 2us just to be sure that the V20 reads the data and goes HiZ
  pin_->releasePORT_DATA();       //
  pin_->setPIN_nRDYRES_HIGH();    // RDYRES_ = HIGH: Now V20 in HiZ (HOLD), so it's safe deactivate RDYRES_

  // Time critical section!!!
  noInterrupts();               // !!! Start of a time critical section. No interrupt allowed
  pin_->setPIN_nHOLDRES_LOW();  // !!! HOLDRES_ = LOW: Resume V20 from HiZ (reset HOLD FF)
  pin_->setPIN_nHOLDRES_HIGH(); // !!! HOLDRES_ = HIGH
  interrupts();                 // !!! End of a time critical section. Interrupt resumed
}

inline void V20MbcIoClass::__runInterrupt(void)
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
  pin_->releasePORT_DATA();       //
  pin_->setPIN_nRDYRES_HIGH();    // RDYRES_ = HIGH: Now V20 in HiZ (HOLD), so it's safe deactivate RDYRES_

  // Time critical section!!!
  noInterrupts();               // !!! Start of a time critical section. No interrupt allowed
  pin_->setPIN_nHOLDRES_LOW();  // !!! HOLDRES_ = LOW: Resume V20 from HiZ (reset HOLD FF)
  pin_->setPIN_nHOLDRES_HIGH(); // !!! HOLDRES_ = HIGH
  interrupts();                 // !!! End of a time critical section. Interrupt resumed
}

inline void V20MbcIoClass::__runHalt(void)
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

inline void V20MbcIoClass::__execWriteOpcode(void)
{
  uint8_t command = getCommand();

  if (likely(command >= MbcIo::WR_BEGIN && command <= MbcIo::WR_END))
  {
    io_dev_wr_[command - MbcIo::WR_BEGIN]->run(this);

    if (command != MbcIo::WR_SELSECT && command != MbcIo::WR_WRITESECT)
      setCommand(MbcIo::NO_OPERATION);
  }
}

inline void V20MbcIoClass::__execReadOpcode(void)
{
  uint8_t command = getCommand();

  if (likely(command >= MbcIo::RD_BEGIN && command <= MbcIo::RD_END))
  {
    io_dev_rd_[command - MbcIo::RD_BEGIN]->run(this);

    if (command != MbcIo::RD_DATETIME && command != MbcIo::RD_READSECT)
      setCommand(MbcIo::NO_OPERATION);
  }
}

// states

void __V20MbcIoStateInit::handle(V20MbcIoClass *io)
{
  V20MbcDev &dev = io->dev_;
  DevUser *user = dev.getUser();

  if (!user->getKey())
    io->state_ = &io->st_bootstrap_;
  else
    io->state_ = &io->st_menu_enter_;
}

void __V20MbcIoStateMenuEnter::handle(V20MbcIoClass *io)
{
  V20MbcDev &dev = io->dev_;

  V20MbcMenu.begin(dev);
  V20MbcMenu.enter();

  io->state_ = &io->st_menu_run_;
}

void __V20MbcIoStateMenuRun::handle(V20MbcIoClass *io)
{
  if (!V20MbcMenu.run())
    io->state_ = &io->st_bootstrap_;
}

void __V20MbcIoStateBootstrap::handle(V20MbcIoClass *io)
{
  V20MbcDev &dev = io->dev_;
  V20MbcLoader &loader = io->loader_;

  loader.begin(dev);
  loader.bootstrap();

  io->state_ = &io->st_run_;
}

void __V20MbcIoStateRun::handle(V20MbcIoClass *io)
{
  io->runIo();
}
