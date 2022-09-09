#ifndef __INTERNAL__MBCDEV_H__
#define __INTERNAL__MBCDEV_H__

#include <j4fDev.h>

#include "MbcDevDisk.h"

class __MbcDevMcuVariant
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

class MbcDevClass : public __MbcDevMcuVariant
{
public:
  void begin(uint8_t pin_user, uint8_t pin_rst, uint8_t pin_cts);
  DevGpio *getGpio(void);
  DevRtc *getRtc(void);
  DevSd *getSd(void);
  DevTty *getTty(void);
  DevUser *getUser(void);
  MbcDisk *getDisk(void);

private:
  DevGpio gpio_;
  DevRtc rtc_;
  DevSd sd_;
  DevTty tty_;
  DevUser user_;
  MbcDisk disk_;
};

extern MbcDevClass MbcDev;

#endif // __INTERNAL__MBCDEV_H__
