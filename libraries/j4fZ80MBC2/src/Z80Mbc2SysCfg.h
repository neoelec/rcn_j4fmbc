#ifndef __INTERNAL__Z80MBC2DEVBANK_H__
#define __INTERNAL__Z80MBC2DEVBANK_H__

#include <j4fMbc.h>

#include "Z80Mbc2Pin.h"

class Z80Mbc2IoClass;

class Z80Mbc2SysCfg : public MbcDevIo
{
public:
  void begin(Z80Mbc2IoClass *mbcIo, Z80Mbc2Pin *pin)
  {
    mbcIo_ = mbcIo;
    pin_ = pin;
  }

protected:
  Z80Mbc2IoClass *mbcIo_;
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
class Z80Mbc2DevWrSETBANK : public Z80Mbc2SysCfg
{
public:
  void run(MbcIo *io);
};

// SETIRQ - enable/disable the IRQ generation
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                              X  X  X  X  X  X  X  0    Serial Rx IRQ not enabled
//                              X  X  X  X  X  X  X  1    Serial Rx IRQ enabled
//                              X  X  X  X  X  X  0  X    Systick IRQ not enabled
//                              X  X  X  X  X  X  1  X    Systick IRQ enabled
//
//
// The INT_ signal is shared among various interrupt requests. This allows to use the simplified
//  Mode 1 scheme of the Z80 CPU (fixed jump to 0x0038 on INT_ signal active) with multiple interrupt causes.
// The SETIRQ purpose is to enable/disable the generation of an IRQ (using the INT_ signal)
//  selecting wich event you want enable.
// When a IRQ is enabled you have to serve it on the Z80 side with a ISR (Interrupt Service Routine).
// Inside the ISR code, you have to read the SYSIRQ Opcode to know the exact causes of the interrupt (see the
//  SYSIRQ Opcode) because multiple causes/bits could be active, so your ISR must be written to check and serve
//  them all.
//
// NOTE 1: Only D0 and D1 are currently used.
// NOTE 2: At reset time all the IRQ triggers are normally disabled (unless they are enabled for special
//         boot cases).
class Z80Mbc2DevWrSETIRQ : public Z80Mbc2SysCfg
{
public:
  void run(MbcIo *io);
};

// SETTICK - set the Systick timer time (milliseconds)
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    Systick time (binary) [1..255]
//
// Set/change the time (millisecond) used for the Systick timer.
// At reset time the default value is 100ms.
// See SETIRQ and SYSIRQ Opcodes for more info.
//
// NOTE: If the time is 0 milliseconds the set operation is ignored.
class Z80Mbc2DevWrSETTICK : public Z80Mbc2SysCfg
{
public:
  void run(MbcIo *io);
};

// SYSIRQ - return the "interrupt status byte":
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                              X  X  X  X  X  X  X  0    Serial Rx IRQ not set
//                              X  X  X  X  X  X  X  1    Serial Rx IRQ set
//                              X  X  X  X  X  X  0  X    Systick IRQ not set
//                              X  X  X  X  X  X  1  X    Systick IRQ set
//
//
// The INT_ signal is shared among various interrupt requests. This allows to use the simplified
// Mode 1 scheme of the Z80 CPU (fixed jump to 0x0038 on INT_ signal active) with multiple interrupt causes.
// The SYSIRQ purpose is to allow the Z80 CPU to know the exact causes of the interrupt reading the
// "interrupt status byte" that contains up to eight "interrupt status bits". So the ISR (Interrupt Service
// Routine) should be structured to read at first the "interrupt status byte" using the SYSIRQ Opcode,
// and than execute the needed actions before return to the normal execution.
// Note that multiple causes/bits could be active.
//
//
//
// NOTE 1: Only D0 and D1 "interrupt status bit" are currently used.
// NOTE 2: After the SYSIRQ call all the "interrupt status bits" are cleared.
// NOTE 3: The INT_ signal is always reset (set to HIGH) after this I/O operation, so you have to call it
//         always from inside the ISR (on the Z80 side) before to re-enable the Z80 IRQ again.
class Z80Mbc2DevRdSYSIRQ : public Z80Mbc2SysCfg
{
public:
  void run(MbcIo *io);
};

#endif // __INTERNAL__Z80MBC2DEVBANK_H__
