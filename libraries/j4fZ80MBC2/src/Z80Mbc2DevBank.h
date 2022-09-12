#ifndef __INTERNAL__Z80MBC2DEVBANK_H__
#define __INTERNAL__Z80MBC2DEVBANK_H__

#include <j4fMbc.h>

#include "Z80Mbc2Pin.h"

class Z80Mbc2DevBank : public MbcDevIo
{
public:
  void begin(Z80Mbc2Pin *pin)
  {
    pin_ = pin;
  }

protected:
  Z80Mbc2Pin *pin_;
};

// BANKED RAM
// SETBANK - select the Os RAM Bank (binary):
//
//                  I/O DATA:  D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    Os Bank number (binary) [0..2]
//
//
// Set a 32kB RAM bank for the lower half of the Z80 address space (from 0x0000 to 0x7FFF).
// The upper half (from 0x8000 to 0xFFFF) is the common fixed bank.
// Allowed Os Bank numbers are from 0 to 2.
//
// Please note that there are three kinds of Bank numbers (see the A040618 schematic):
//
// * the "Os Bank" number is the bank number managed (known) by the Os;
// * the "Logical Bank" number is the bank seen by the Atmega32a (through BANK1 and BANK0 address lines);
// * the "Physical Bank" number is the real bank addressed inside the RAM chip (RAM_A16 and RAM_A15 RAM
//   address lines).
//
// The following tables shows the relations:
//
//
//  Os Bank | Logical Bank |  Z80 Address Bus    |   Physical Bank   |            Notes
//  number  | BANK1 BANK0  |        A15          |  RAM_A16 RAM_A15  |
// ------------------------------------------------------------------------------------------------
//     X    |   X     X    |         1           |     0       1     |  Phy Bank 1 (common fixed)
//     -    |   0     0    |         0           |     0       1     |  Phy Bank 1 (common fixed)
//     0    |   0     1    |         0           |     0       0     |  Phy Bank 0 (Logical Bank 1)
//     2    |   1     0    |         0           |     1       1     |  Phy Bank 3 (Logical Bank 2)
//     1    |   1     1    |         0           |     1       0     |  Phy Bank 2 (Logical Bank 3)
//
//
//
//      Physical Bank      |    Logical Bank     |   Physical Bank   |   Physical RAM Addresses
//          number         |       number        |  RAM_A16 RAM_A15  |
// ------------------------------------------------------------------------------------------------
//            0            |         1           |     0       0     |   From 0x00000 to 0x07FFF
//            1            |         0           |     0       1     |   From 0x08000 to 0x0FFFF
//            2            |         3           |     1       0     |   From 0x01000 to 0x17FFF
//            3            |         2           |     1       1     |   From 0x18000 to 0x1FFFF
//
//
// Note that the Logical Bank 0 can't be used as switchable Os Bank bacause it is the common
//  fixed bank mapped in the upper half of the Z80 address space (from 0x8000 to 0xFFFF).
//
//
// NOTE: If the Os Bank number is greater than 2 no selection is done.
class Z80Mbc2DevWrSETBANK : public Z80Mbc2DevBank
{
public:
  inline void run(MbcIo *io);
};

inline void Z80Mbc2DevWrSETBANK::run(MbcIo *io)
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

#endif // __INTERNAL__Z80MBC2DEVBANK_H__
