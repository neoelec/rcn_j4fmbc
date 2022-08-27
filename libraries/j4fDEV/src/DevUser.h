#ifndef __INTERNAL__DEVUSER_H__
#define __INTERNAL__DEVUSER_H__

#include <stdint.h>

class DevUser
{
public:
  void begin(uint8_t pin_user);
  void setLedOn(void);
  void setLedOff(void);
  uint8_t getKey(void);

private:
  uint8_t pin_user_;
};

#endif // __INTERNAL__DEVUSER_H__
