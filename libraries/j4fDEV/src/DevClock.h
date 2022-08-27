#ifndef __INTERNAL__DEVCLOCK_H__
#define __INTERNAL__DEVCLOCK_H__

#include <stdint.h>

class DevClock {
public:
  virtual void begin(void) = 0;
  virtual void enable(void) = 0;
  virtual void setMode(uint8_t mode) = 0;
};

#endif // __INTERNAL__DEVCLOCK_H__
