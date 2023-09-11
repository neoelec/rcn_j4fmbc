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
}

