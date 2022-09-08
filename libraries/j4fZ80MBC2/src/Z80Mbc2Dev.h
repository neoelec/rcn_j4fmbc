#ifndef __INTERNAL__Z80MBC2DEV_H__
#define __INTERNAL__Z80MBC2DEV_H__

#include <j4fDev.h>
#include <j4fMbc.h>

#include "Z80Mbc2Pin.h"

class __Z80Mbc2DevMcuVariant
{
public:
  DevClock *getClock(void);

protected:
#if defined(ARDUINO_AVR_ATmega32)
  DevClockATmega32 clock_;
#elif defined(ARDUINO_AVR_ATmega1284)
  DevClockATmega1284 clock_;
#endif
};

class Z80Mbc2Dev : public __Z80Mbc2DevMcuVariant
{
public:
  void begin(void);
  DevGpio *getGpio(void);
  DevRtc *getRtc(void);
  DevSd *getSd(void);
  DevTty *getTty(void);
  DevUser *getUser(void);
  MbcDisk *getDisk(void);
  Z80Mbc2Pin *getPin(void);

private:
  DevGpio gpio_;
  DevRtc rtc_;
  DevSd sd_;
  DevTty tty_;
  DevUser user_;
  MbcDisk disk_;
  Z80Mbc2Pin pin_;
};

#endif // __INTERNAL__Z80MBC2DEV_H__
