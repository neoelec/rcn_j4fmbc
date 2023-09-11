// SPDX-License-Identifier: GPL-3.0-or-later

#include "j4fV20Mbc.h"

#define PIN_USER 13     // PD5 pin 19   Led USER and key (led USER is ON if LOW)
#define PIN_MCU_nRTS 11 // PD3 pin 17   * RESERVED - NOT USED *
#define PIN_MCU_nCTS 12 // PD4 pin 18   Used only to reset uTerm at boot time

V20MbcIoClass V20MbcIo;

void V20MbcIoClass::begin(void)
{
  staticSysFlags_ = 0;

  MbcDev.begin(PIN_USER, PIN_MCU_nRTS, PIN_MCU_nCTS);

  Serial.println();
  Serial.println(F("V20-MBC - A250220"));
  Serial.println(F("IOS - I/O Subsystem - S260320-R230520 + Raccoon's MOD"));
  Serial.println();

  __beginIoDev();
  __initFromCfg();
  __initIoDevWr();
  __initIoDevRd();

  state_ = &st_init_;
}

void V20MbcIoClass::__beginIoDev(void)
{
  auto *gpio = MbcDev.getGpio();
  auto *user = MbcDev.getUser();
  auto *disk = MbcDev.getDisk();
  auto *rtc = MbcDev.getRtc();

  pin_.begin();

  MbcDevWrSELDISK.begin(disk);
  MbcDevWrSELTRACK.begin(disk);
  MbcDevWrSELSECT.begin(disk);
  MbcDevWrWRITESECT.begin(disk);
  MbcDevRdERRDISK.begin(disk);
  MbcDevRdREADSECT.begin(disk);
  MbcDevRdSDMOUNT.begin(disk);

  MbcDevWrUSERLED.begin(user);
  MbcDevRdUSERKEY.begin(user);

  if (gpio->isAvailable())
  {
    MbcDevWrGPIOA.begin(gpio);
    MbcDevWrGPIOB.begin(gpio);
    MbcDevWrIODIRA.begin(gpio);
    MbcDevWrIODIRB.begin(gpio);
    MbcDevWrGPPUA.begin(gpio);
    MbcDevWrGPPUB.begin(gpio);
    MbcDevRdGPIOA.begin(gpio);
    MbcDevRdGPIOB.begin(gpio);
  }

  if (rtc->isAvailable())
  {
    MbcDevRdDATETIME.begin(rtc);
    staticSysFlags_ |= 1 << MbcDevRdSYSFLAGSClass::RTC;
  }
}

void V20MbcIoClass::__initFromCfg(void)
{
  V20MbcCfg cfg;

  cfg.begin(MbcDev.getSd());

  staticSysFlags_ |= cfg.getAutoExecEn() << MbcDevRdSYSFLAGSClass::AUTOEXEC;
  wait_count_ = 1 << cfg.getClkMode();
}

void V20MbcIoClass::__initIoDevWr(void)
{
  auto *gpio = MbcDev.getGpio();

  for (uint8_t command = MbcIo::WR_BEGIN; command <= MbcIo::WR_END; command++)
    __setIoDevWr(command, &MbcDevRdWrNOP);

  __setIoDevWr(MbcIo::WR_USERLED, &MbcDevWrUSERLED);
  __setIoDevWr(MbcIo::WR_SERIALTX, &MbcDevWrSERIALTX);
  __setIoDevWr(MbcIo::WR_RXIRQFLAG, &MbcDevWrRXIRQFLAG);
  __setIoDevWr(MbcIo::WR_SELDISK, &MbcDevWrSELDISK);
  __setIoDevWr(MbcIo::WR_SELTRACK, &MbcDevWrSELTRACK);
  __setIoDevWr(MbcIo::WR_SELSECT, &MbcDevWrSELSECT);
  __setIoDevWr(MbcIo::WR_WRITESECT, &MbcDevWrWRITESECT);

  if (gpio->isAvailable())
  {
    __setIoDevWr(MbcIo::WR_GPIOA, &MbcDevWrGPIOA);
    __setIoDevWr(MbcIo::WR_GPIOB, &MbcDevWrGPIOB);
    __setIoDevWr(MbcIo::WR_IODIRA, &MbcDevWrIODIRA);
    __setIoDevWr(MbcIo::WR_IODIRB, &MbcDevWrIODIRB);
    __setIoDevWr(MbcIo::WR_GPPUA, &MbcDevWrGPPUA);
    __setIoDevWr(MbcIo::WR_GPPUB, &MbcDevWrGPPUB);
  }
}

void V20MbcIoClass::__setIoDevWr(uint8_t command, MbcDevIo *dev)
{
  io_dev_wr_[command - MbcIo::WR_BEGIN] = dev;
}

void V20MbcIoClass::__initIoDevRd(void)
{
  auto *gpio = MbcDev.getGpio();

  for (uint8_t command = MbcIo::RD_BEGIN; command <= MbcIo::RD_END; command++)
    __setIoDevRd(command, &MbcDevRdWrNOP);

  __setIoDevRd(MbcIo::RD_USERKEY, &MbcDevRdUSERKEY);
  __setIoDevRd(MbcIo::RD_DATETIME, &MbcDevRdDATETIME);
  __setIoDevRd(MbcIo::RD_ERRDISK, &MbcDevRdERRDISK);
  __setIoDevRd(MbcIo::RD_READSECT, &MbcDevRdREADSECT);
  __setIoDevRd(MbcIo::RD_SDMOUNT, &MbcDevRdSDMOUNT);
  __setIoDevRd(MbcIo::RD_ATXBUFF, &MbcDevRdATXBUFF);

  if (gpio->isAvailable())
  {
    __setIoDevRd(MbcIo::RD_GPIOA, &MbcDevRdGPIOA);
    __setIoDevRd(MbcIo::RD_GPIOB, &MbcDevRdGPIOB);
  }
}

void V20MbcIoClass::__setIoDevRd(uint8_t command, MbcDevIo *dev)
{
  io_dev_rd_[command - MbcIo::RD_BEGIN] = dev;
}

inline void V20MbcIoClass::runIo(void)
{
  if (!pin_.getPIN_READY())
  {
    if (!pin_.getPIN_nWR())
      __runWrite();
    else if (!pin_.getPIN_nRD())
      __runRead();
    else if (!pin_.getPIN_nINTA())
      __runInterrupt();
    else
      __runHalt();
  }

  if (Serial.available() && getIrq(MbcIo::IRQ_RX))
    pin_.setPIN_INTR_HIGH();
}

uint8_t V20MbcIoClass::getSysFlag(void)
{
  return staticSysFlags_;
}

inline void V20MbcIoClass::__runWrite(void)
{
  _setAddress(pin_.getPIN_AD1AD0());
  setData(pin_.getPORT_DATA());

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
  pin_.setPIN_nRDYRES_LOW();  // RDYRES_ = LOW: Now is safe reset WAIT FF (exiting from WAIT state)
  pin_.setPIN_nRDYRES_HIGH(); // RDYRES_ = HIGH

  // Time critical section!!!
  noInterrupts();              // !!! Start of a time critical section. No interrupt allowed
  pin_.setPIN_nHOLDRES_LOW();  // !!! HOLDRES_ = LOW: Resume V20 from HiZ (reset HOLD FF)
  pin_.setPIN_nHOLDRES_HIGH(); // !!! HOLDRES_ = HIGH
  interrupts();                // !!! End of a time critical section. Interrupt resumed
}

inline void V20MbcIoClass::__runRead(void)
{
  _setAddress(pin_.getPIN_AD1AD0());
  setData(0);

  switch (getAddress())
  {
  case 0: // AD1-AD0 = 0 (I/O read address = 0x00): EXECUTE READ OPCODE.
    __execReadOpcode();
    break;

  case 1: // AD1-AD0 = 1 (I/O read address = 0x01): SERIAL RX.
    MbcDevRdSERIALRX.run(this);
    pin_.setPIN_INTR_LOW(); // INTR = LOW: Reset the INTR signal (if used).
    break;

  case 2: // AD1-AD0 = 2 (I/O read address = 0x02): SYSFLAGS.
    MbcDevRdSYSFLAGS.run(this);
    break;

  case 3: // NOT USED - RESERVED
    break;
  }

  // Current output on data bus
  pin_.setPORT_DATA(getData());

  // Control bus sequence to exit from a wait state (M I/O read cycle)
  pin_.setPIN_nRDYRES_LOW();      // RDYRES_ = LOW: Now is safe reset WAIT FF (exiting from WAIT state)
  delayMicroseconds(wait_count_); // Wait 2us just to be sure that the V20 reads the data and goes HiZ
  pin_.releasePORT_DATA();        //
  pin_.setPIN_nRDYRES_HIGH();     // RDYRES_ = HIGH: Now V20 in HiZ (HOLD), so it's safe deactivate RDYRES_

  // Time critical section!!!
  noInterrupts();              // !!! Start of a time critical section. No interrupt allowed
  pin_.setPIN_nHOLDRES_LOW();  // !!! HOLDRES_ = LOW: Resume V20 from HiZ (reset HOLD FF)
  pin_.setPIN_nHOLDRES_HIGH(); // !!! HOLDRES_ = HIGH
  interrupts();                // !!! End of a time critical section. Interrupt resumed
}

inline void V20MbcIoClass::__runInterrupt(void)
{
  // Now CPU is in the first of the two INTA_ bus cycles
  pin_.setPIN_INTR_LOW(); // INTR = LOW: Reset the INTR signal

  // Current output on data bus
  setData(33); // For now it is fixed
  pin_.setPORT_DATA(getData());

  // Control bus sequence to exit from a wait state (Interrupt)
  pin_.setPIN_nRDYRES_LOW();      // RDYRES_ = LOW: Now is safe reset WAIT FF (exiting from WAIT state)
  delayMicroseconds(wait_count_); // Wait 2us (8 bus cycles @ 4MHz) just to be sure to execute both
  delayMicroseconds(wait_count_); //  the two INTA bus cycles
  pin_.releasePORT_DATA();        //
  pin_.setPIN_nRDYRES_HIGH();     // RDYRES_ = HIGH: Now V20 in HiZ (HOLD), so it's safe deactivate RDYRES_

  // Time critical section!!!
  noInterrupts();              // !!! Start of a time critical section. No interrupt allowed
  pin_.setPIN_nHOLDRES_LOW();  // !!! HOLDRES_ = LOW: Resume V20 from HiZ (reset HOLD FF)
  pin_.setPIN_nHOLDRES_HIGH(); // !!! HOLDRES_ = HIGH
  interrupts();                // !!! End of a time critical section. Interrupt resumed
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

  if (likely(/* command >= MbcIo::WR_BEGIN && */ command <= MbcIo::WR_END))
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
  DevUser *user = MbcDev.getUser();

  if (!user->getKey())
    io->state_ = &io->st_bootstrap_;
  else
    io->state_ = &io->st_menu_enter_;
}

void __V20MbcIoStateMenuEnter::handle(V20MbcIoClass *io)
{
  V20MbcMenu.begin();
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
  V20MbcLoader &loader = io->loader_;
  V20MbcPin *pin = &io->pin_;

  loader.begin(pin);
  loader.bootstrap();

  io->state_ = &io->st_run_;
}

void __V20MbcIoStateRun::handle(V20MbcIoClass *io)
{
  io->runIo();
}
