#ifndef __INTERNAL__MBCDEVUSER_H__
#define __INTERNAL__MBCDEVUSER_H__

#include <j4fDev.h>

#include "MbcDevIo.h"

class MbcDevUser
{
public:
  void begin(DevUser *user)
  {
    user_ = user;
  }

protected:
  DevUser *user_;
};

// USER LED:
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                              x  x  x  x  x  x  x  0    USER Led off
//                              x  x  x  x  x  x  x  1    USER Led on
class MbcDevWrUSERLEDClass : public MbcDevIo, public MbcDevUser
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrUSERLEDClass::run(MbcIo *io)
{
  const uint8_t io_data = io->getData();

  if (io_data & 0x01)
    user_->setLedOn();
  else
    user_->setLedOff();

  io->setCommand(MbcIo::NO_OPERATION);
}

// USER KEY:
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                              0  0  0  0  0  0  0  0    USER Key not pressed
//                              0  0  0  0  0  0  0  1    USER Key pressed
class MbcDevRdUSERKEYClass : public MbcDevIo, public MbcDevUser
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdUSERKEYClass::run(MbcIo *io)
{
  io->setData(user_->getKey());
  io->setCommand(MbcIo::NO_OPERATION);
}

extern MbcDevWrUSERLEDClass MbcDevWrUSERLED;
extern MbcDevRdUSERKEYClass MbcDevRdUSERKEY;

#endif // __INTERNAL__MBCDEVUSER_H__
