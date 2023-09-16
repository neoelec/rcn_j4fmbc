#include "j4fZ80Mbc2.h"

void Z80Mbc2DevWrSETBANK::run(MbcIo *io)
{
  switch (io->getData())
  {
  case 0:
    pin_->setPIN_BANK0_HIGH();
    pin_->setPIN_BANK1_LOW();
    break;
  case 1:
    pin_->setPIN_BANK0_HIGH();
    pin_->setPIN_BANK1_HIGH();
    break;
  case 2:
    pin_->setPIN_BANK0_LOW();
    pin_->setPIN_BANK1_HIGH();
    break;
  }

  io->setCommand(MbcIo::NO_OPERATION);
}

void Z80Mbc2DevWrSETIRQ::run(MbcIo *io)
{
  union Z80Mbc2IrqStatus recevied;

  recevied.raw_ = io->getData();

  if (recevied.rx_)
    mbcIo_->enableIrqTtyRx();
  else
    mbcIo_->disableIrqTtyRx();

  if (recevied.sys_tick_)
    mbcIo_->enableIrqTick();
  else
    mbcIo_->disableIrqTick();

  io->setCommand(MbcIo::NO_OPERATION);
}

void Z80Mbc2DevWrSETTICK::run(MbcIo *io)
{
  int8_t sys_tick = (int8_t)io->getData();

  if (sys_tick > 0)
    mbcIo_->setSysTick(sys_tick);

  io->setCommand(MbcIo::NO_OPERATION);
}

void Z80Mbc2DevRdSYSIRQ::run(MbcIo *io)
{
  io->setData(mbcIo_->getIrqStatus());
  mbcIo_->setIrqStatus(0);
  io->setCommand(MbcIo::NO_OPERATION);
}
