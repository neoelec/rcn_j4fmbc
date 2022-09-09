#ifndef __INTERNAL__MBCDEVRTC_H__
#define __INTERNAL__MBCDEVRTC_H__

#include <j4fDev.h>

#include "j4fMbc.h"

class MbcDevRtc
{
public:
  void begin(DevRtc *rtc)
  {
    rtc_ = rtc;
  }

protected:
  DevRtc *rtc_;
};

// DATETIME (Read date/time and temperature from the RTC. Binary values):
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                I/O DATA 0   D7 D6 D5 D4 D3 D2 D1 D0    seconds [0..59]     (1st data byte)
//                I/O DATA 1   D7 D6 D5 D4 D3 D2 D1 D0    minutes [0..59]
//                I/O DATA 2   D7 D6 D5 D4 D3 D2 D1 D0    hours   [0..23]
//                I/O DATA 3   D7 D6 D5 D4 D3 D2 D1 D0    day     [1..31]
//                I/O DATA 4   D7 D6 D5 D4 D3 D2 D1 D0    month   [1..12]
//                I/O DATA 5   D7 D6 D5 D4 D3 D2 D1 D0    year    [0..99]
//                I/O DATA 6   D7 D6 D5 D4 D3 D2 D1 D0    tempC   [-128..127] (7th data byte)
//
// NOTE 1: If RTC is not found all read values wil be = 0
// NOTE 2: Overread data (more then 7 bytes read) will be = 0
// NOTE 3: The temperature (Celsius) is a byte with two complement binary format [-128..127]
class MbcDevRdDATETIMEClass : public MbcDevIo, public MbcDevRtc
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdDATETIMEClass::run(MbcIo *io)
{
  const uint16_t io_count = io->getCount();
  uint8_t io_data;

  switch (io_count)
  {
  case 0:
    rtc_->refresh();
    io_data = rtc_->getSecond();
    break;
  case 1:
    io_data = rtc_->getMinute();
    break;
  case 2:
    io_data = rtc_->getHour();
    break;
  case 3:
    io_data = rtc_->getDay();
    break;
  case 4:
    io_data = rtc_->getMonth();
    break;
  case 5:
    io_data = rtc_->getYear();
    break;
  case 6:
    io_data = rtc_->getTemperature();
    break;
  default:
    io->setCommand(MbcIo::NO_OPERATION);
    return;
  }

  io->setData(io_data);
  io->setCount(io_count + 1);
}

extern MbcDevRdDATETIMEClass MbcDevRdDATETIME;

#endif // __INTERNAL__MBCDEVRTC_H__
