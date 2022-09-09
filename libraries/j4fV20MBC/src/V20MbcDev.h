#ifndef __INTERNAL__V20MBCDEV_H__
#define __INTERNAL__V20MBCDEV_H__

#include <j4fDev.h>
#include <j4fMbc.h>

class __V20MbcDevMcuVariant
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

class V20MbcDev : public __V20MbcDevMcuVariant
{
public:
  void begin(void);
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

#endif // __INTERNAL__V20MBCDEV_H__
