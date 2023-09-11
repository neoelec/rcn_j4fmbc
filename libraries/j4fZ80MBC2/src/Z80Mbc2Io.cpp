// SPDX-License-Identifier: GPL-3.0-or-later

#include "j4fZ80Mbc2.h"

#define PIN_USER 13     // PD5 pin 19   Led USER and key (led USER is ON if LOW)
#define PIN_MCU_nRTS 23 // PC7 pin 29   * RESERVED - NOT USED *
#define PIN_MCU_nCTS 10 // PD2 pin 16   * RESERVED - NOT USED *

Z80Mbc2IoClass Z80Mbc2Io;

void Z80Mbc2IoClass::begin(void)
{
  staticSysFlags_ = 0;
  last_rx_is_empty_ = 0;
  irq_status_.raw_ = 0;

  MbcDev.begin(PIN_USER, PIN_MCU_nRTS, PIN_MCU_nCTS);

  Serial.println();
  Serial.println(F("Z80-MBC2 - A040618"));
  Serial.println(F("IOS - I/O Subsystem - S220718-R240620 + Raccoon's MOD"));
  Serial.println();

  __beginIoDev();
  __initFromCfg();
  __initIoDevWr();
  __initIoDevRd();

  state_ = &st_init_;
}

void Z80Mbc2IoClass::__beginIoDev(void)
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

  MbcDevWrSETBANK.begin(&pin_);

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

void Z80Mbc2IoClass::__initFromCfg(void)
{
  Z80Mbc2Cfg cfg;

  cfg.begin(MbcDev.getSd());

  staticSysFlags_ |= cfg.getAutoExecEn() << MbcDevRdSYSFLAGSClass::AUTOEXEC;
  wait_count_ = 1 << cfg.getClkMode();

  irq_tty_rx_ = !!cfg.getIrqTtyRx();
  if (irq_tty_rx_)
    Serial.println(F("IOS: IRQ-TTY-RX flag is enabled."));
}

void Z80Mbc2IoClass::__initIoDevWr(void)
{
  auto *gpio = MbcDev.getGpio();

  for (uint8_t command = MbcIo::WR_BEGIN; command <= MbcIo::WR_END; command++)
    __setIoDevWr(command, &MbcDevRdWrNOP);

  __setIoDevWr(MbcIo::WR_USERLED, &MbcDevWrUSERLED);
  __setIoDevWr(MbcIo::WR_SERIALTX, &MbcDevWrSERIALTX);
  __setIoDevWr(MbcIo::WR_SELDISK, &MbcDevWrSELDISK);
  __setIoDevWr(MbcIo::WR_SELTRACK, &MbcDevWrSELTRACK);
  __setIoDevWr(MbcIo::WR_SELSECT, &MbcDevWrSELSECT);
  __setIoDevWr(MbcIo::WR_WRITESECT, &MbcDevWrWRITESECT);
  __setIoDevWr(MbcIo::WR_SETBANK, &MbcDevWrSETBANK);

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

void Z80Mbc2IoClass::__setIoDevWr(uint8_t command, MbcDevIo *dev)
{
  if (command >= ARRAY_SIZE(io_dev_wr_))
    return;

  io_dev_wr_[command /* - MbcIo::WR_BEGIN */] = dev;
}

void Z80Mbc2IoClass::__initIoDevRd(void)
{
  auto *gpio = MbcDev.getGpio();

  for (uint8_t command = MbcIo::RD_BEGIN; command <= MbcIo::RD_END; command++)
    __setIoDevRd(command, &MbcDevRdWrNOP);

  __setIoDevRd(MbcIo::RD_USERKEY, &MbcDevRdUSERKEY);
  __setIoDevRd(MbcIo::RD_SYSFLAGS, &MbcDevRdSYSFLAGS);
  __setIoDevRd(MbcIo::RD_DATETIME, &MbcDevRdDATETIME);
  __setIoDevRd(MbcIo::RD_ERRDISK, &MbcDevRdERRDISK);
  __setIoDevRd(MbcIo::RD_READSECT, &MbcDevRdREADSECT);
  __setIoDevRd(MbcIo::RD_SDMOUNT, &MbcDevRdSDMOUNT);

  if (gpio->isAvailable())
  {
    __setIoDevRd(MbcIo::RD_GPIOA, &MbcDevRdGPIOA);
    __setIoDevRd(MbcIo::RD_GPIOB, &MbcDevRdGPIOB);
  }
}

void Z80Mbc2IoClass::__setIoDevRd(uint8_t command, MbcDevIo *dev)
{
  io_dev_rd_[command - MbcIo::RD_BEGIN] = dev;
}

inline void Z80Mbc2IoClass::runIo(void)
{
  if (!pin_.getPIN_nWAIT())
  {
    if (!pin_.getPIN_nWR())
      __runWrite();
    else if (!pin_.getPIN_nRD())
      __runRead();
    else
      __runInterrupt();
  }
}

uint8_t Z80Mbc2IoClass::getSysFlag(void)
{
  return staticSysFlags_ | ((!!last_rx_is_empty_) << MbcDevRdSYSFLAGSClass::PREV_RX);
}

void Z80Mbc2IoClass::enableIrqTtyRx(void)
{
  irq_tty_rx_ = true;
}

void Z80Mbc2IoClass::disableIrqTtyRx(void)
{
  irq_tty_rx_ = false;
}

inline void Z80Mbc2IoClass::__runWrite(void)
{
  _setAddress(pin_.getPIN_AD0());
  setData(pin_.getPORT_DATA());

  if (getAddress())
  {
    setCommand(getData());
    setCount(0);
  }
  else
    __execWriteCommand();

  // Control bus sequence to exit from a wait state (M I/O write cycle)
  pin_.setPIN_nBUSREQ_LOW();      // Request for a DMA
  pin_.setPIN_WAIT_nRES_LOW();    // Reset WAIT FF exiting from WAIT state
  delayMicroseconds(wait_count_); //
  pin_.setPIN_WAIT_nRES_HIGH();   // Now Z80 is in DMA, so it's safe set WAIT_RES_ HIGH again
  pin_.setPIN_nBUSREQ_HIGH();     // Resume Z80 from DMA
}

inline void Z80Mbc2IoClass::__runRead(void)
{
  _setAddress(pin_.getPIN_AD0());
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

    irq_status_.rx_ = 0;
  }
  else
    __execReadCommand();

  pin_.setPORT_DATA(getData()); // Configure Z80 data bus D0-D7 (PA0-PA7) as output
                                // Current output on data bus

  // Control bus sequence to exit from a wait state (M I/O read cycle)
  pin_.setPIN_nBUSREQ_LOW();      // Request for a DMA
  pin_.setPIN_WAIT_nRES_LOW();    // Now is safe reset WAIT FF (exiting from WAIT state)
  delayMicroseconds(wait_count_); // Wait 2us just to be sure that Z80 read the data and go HiZ
  delayMicroseconds(wait_count_); //
  pin_.releasePORT_DATA();        // Configure Z80 data bus D0-D7 (PA0-PA7) as input with pull-up
  pin_.setPIN_WAIT_nRES_HIGH();   // Now Z80 is in DMA (HiZ), so it's safe set WAIT_RES_ HIGH again
  pin_.setPIN_nBUSREQ_HIGH();     // Resume Z80 from DMA
}

inline void Z80Mbc2IoClass::__runInterrupt(void)
{
  // Control bus sequence to exit from a wait state (M interrupt cycle)
  pin_.setPIN_nBUSREQ_LOW();      // Request for a DMA
  pin_.setPIN_WAIT_nRES_LOW();    // Reset WAIT FF exiting from WAIT state
  delayMicroseconds(wait_count_); //
  pin_.setPIN_WAIT_nRES_HIGH();   // Now Z80 is in DMA, so it's safe set WAIT_RES_ HIGH again
  pin_.setPIN_nBUSREQ_HIGH();     // Resume Z80 from DMA
}

inline void Z80Mbc2IoClass::__execWriteCommand(void)
{
  uint8_t command = getCommand();

  if (likely(/* command >= MbcIo::WR_BEGIN && */ command <= MbcIo::WR_END))
  {
    io_dev_wr_[command - MbcIo::WR_BEGIN]->run(this);

    if (command != MbcIo::WR_SELSECT && command != MbcIo::WR_WRITESECT)
      setCommand(MbcIo::NO_OPERATION);
  }
}

inline void Z80Mbc2IoClass::__execReadCommand(void)
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

void __Z80Mbc2IoStateInit::handle(Z80Mbc2IoClass *io)
{
  DevUser *user = MbcDev.getUser();

  if (!user->getKey())
    io->state_ = &io->st_bootstrap_;
  else
    io->state_ = &io->st_menu_enter_;
}

void __Z80Mbc2IoStateMenuEnter::handle(Z80Mbc2IoClass *io)
{
  Z80Mbc2Menu.begin();
  Z80Mbc2Menu.enter();

  io->state_ = &io->st_menu_run_;
}

void __Z80Mbc2IoStateMenuRun::handle(Z80Mbc2IoClass *io)
{
  if (!Z80Mbc2Menu.run())
    io->state_ = &io->st_bootstrap_;
}

void __Z80Mbc2IoStateBootstrap::handle(Z80Mbc2IoClass *io)
{
  Z80Mbc2Loader &loader = io->loader_;
  Z80Mbc2Pin *pin = &io->pin_;

  loader.begin(pin);
  loader.bootstrap();

  io->state_ = &io->st_run_;
}

void __Z80Mbc2IoStateRun::handle(Z80Mbc2IoClass *io)
{
  io->runIo();
}
