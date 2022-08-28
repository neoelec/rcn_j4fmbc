#ifndef __INTERNAL__DEVCLOCKATMEGA32_H__
#define __INTERNAL__DEVCLOCKATMEGA32_H__

#include "j4fDev.h"

class DevClockATmega32 : public DevClock{
public:
  void begin(void);
  void enable(void);
  void setMode(uint8_t mode);
};

#endif // __INTERNAL__DEVCLOCKATMEGA32_H__
