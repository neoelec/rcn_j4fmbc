#ifndef __INTERNAL__MBCDEVTTY_H__
#define __INTERNAL__MBCDEVTTY_H__

#include "j4fMbc.h"

// SERIAL RX:
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    ASCII char read from serial
//
// NOTE 1: If there is no input char, a value 0xFF is forced as input char.
// NOTE 2: The INTR signal is always reset (set to LOW) after this I/O operation.
// NOTE 3: This I/O do not require any previous STORE OPCODE operation.
class MbcDevRdSERIALRXClass : public MbcDevIo
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdSERIALRXClass::run(MbcIo *io)
{
  if (unlikely(Serial.available() > 0))
    io->setData(Serial.read());
  else
    io->setData(0xFF);
}

// SERIAL TX:
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    ASCII char to be sent to serial
class MbcDevWrSERIALTXClass : public MbcDevIo
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrSERIALTXClass::run(MbcIo *io)
{
  Serial.write(io->getData());
}

// RX IRQ FLAG:
// Set or reset the flag to enable/disable the INTR line when a valid data is preset into the Rx serial buffer
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                              x  x  x  x  x  x  x  0    Rx IRQ off
//                              x  x  x  x  x  x  x  1    Rx IRQ on
//
// NOTE: The default value after a reset/power on is 0 (Rx IRQ off)
class MbcDevWrRXIRQFLAGClass : public MbcDevIo
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrRXIRQFLAGClass::run(MbcIo *io)
{
  if (io->getData() & (0x1 << MbcIo::IRQ_RX))
    io->setIrq(MbcIo::IRQ_RX);
}

// ATXBUFF - return the current available free space (in bytes) in the TX buffer:
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    free space in bytes (binary)
//
// NOTE: This opcode is intended to avoid delays in serial Tx operations, as the IOS hold the V20
//       in a wait status if the TX buffer is full.
class MbcDevRdATXBUFFClass : public MbcDevIo
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdATXBUFFClass::run(MbcIo *io)
{
  io->setData(Serial.availableForWrite());
}

extern MbcDevRdSERIALRXClass MbcDevRdSERIALRX;
extern MbcDevWrSERIALTXClass MbcDevWrSERIALTX;
extern MbcDevWrRXIRQFLAGClass MbcDevWrRXIRQFLAG;
extern MbcDevRdATXBUFFClass MbcDevRdATXBUFF;

#endif // __INTERNAL__MBCDEVTTY_H__
