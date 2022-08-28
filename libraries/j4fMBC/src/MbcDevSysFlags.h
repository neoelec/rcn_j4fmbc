#ifndef __MBCDEVSYSFLAGS_H__
#define __MBCDEVSYSFLAGS_H__

#include "MbcDev.h"
#include "MbcIo.h"

class MbcDevRdSYSFLAGS : public MbcDev
{
public:
  enum {
    AUTOEXEC = 0,
    RTC,
    TTY_RX_BUF,
    PREV_RX,
  };

  inline void run(MbcIo &io);
};

// SYSFLAGS (Various system flags for the OS):
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                              X  X  X  X  X  X  X  0    AUTOEXEC not enabled
//                              X  X  X  X  X  X  X  1    AUTOEXEC enabled
//                              X  X  X  X  X  X  0  X    DS3231 RTC not found
//                              X  X  X  X  X  X  1  X    DS3231 RTC found
//                              X  X  X  X  X  0  X  X    Serial RX buffer empty
//                              X  X  X  X  X  1  X  X    Serial RX char available
//                              X  X  X  X  0  X  X  X    Previous RX char valid
//                              X  X  X  X  1  X  X  X    Previous RX char was a "buffer empty" flag
//
// NOTE: Currently only D0-D3 are used
inline void MbcDevRdSYSFLAGS::run(MbcIo &io)
{
  uint8_t io_data = io.getSysFlag();

  io_data |= (!!Serial.available()) << TTY_RX_BUF;

  io.setData(io_data);
}

#endif // __MBCDEVSYSFLAGS_H__
