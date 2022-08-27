#ifndef __INTERNAL__DEVCLOCKATMEGA1284_H__
#define __INTERNAL__DEVCLOCKATMEGA1284_H__

#include "DevClock.h"

class DevClockATmega1284 : public DevClock{
public:
  void begin(void);
  void enable(void);
  void setMode(uint8_t mode);
};

#endif // __INTERNAL__DEVCLOCKATMEGA1284_H__
