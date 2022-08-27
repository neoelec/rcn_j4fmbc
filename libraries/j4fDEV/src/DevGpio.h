#ifndef __INTERNAL__DEVGPIO_H__
#define __INTERNAL__DEVGPIO_H__

#include <Adafruit_MCP23X17.h>

class DevGpio
{
public:
  void begin(void);
  bool isAvailable(void);
  inline void writeGPIOA(uint8_t value);
  inline void writeGPIOB(uint8_t value);
  inline void writeIODIRA(uint8_t value);
  inline void writeIODIRB(uint8_t value);
  inline void writeGPPUA(uint8_t value);
  inline void writeGPPUB(uint8_t value);
  inline uint8_t readGPIOA(void);
  inline uint8_t readGPIOB(void);

private:
  Adafruit_MCP23X17 mcp_;
  bool available_;
};

inline void DevGpio::writeGPIOA(uint8_t value)
{
  if (available_)
    mcp_.writeGPIOA(value);
}

inline void DevGpio::writeGPIOB(uint8_t value)
{
  if (available_)
    mcp_.writeGPIOB(value);
}

inline void DevGpio::writeIODIRA(uint8_t value)
{
  if (!available_)
    return;

  for (uint8_t pin = 0; pin < 8; pin++)
  {
    uint8_t mode = value & (1 << pin) ? INPUT : OUTPUT;
    mcp_.pinMode(pin, mode);
  }
}

inline void DevGpio::writeIODIRB(uint8_t value)
{
  if (!available_)
    return;

  for (uint8_t pin = 0; pin < 8; pin++)
  {
    uint8_t mode = value & (1 << pin) ? INPUT : OUTPUT;
    mcp_.pinMode(pin + 8, mode);
  }
}

inline void DevGpio::writeGPPUA(uint8_t value)
{
  if (!available_)
    return;

  for (uint8_t pin = 0; pin < 8; pin++)
  {
    uint8_t mode = value & (1 << pin) ? INPUT_PULLUP : INPUT;
    mcp_.pinMode(pin, mode);
  }
}

inline void DevGpio::writeGPPUB(uint8_t value)
{
  if (!available_)
    return;

  for (uint8_t pin = 0; pin < 8; pin++)
  {
    uint8_t mode = value & (1 << pin) ? INPUT_PULLUP : INPUT;
    mcp_.pinMode(pin + 8, mode);
  }
}

inline uint8_t DevGpio::readGPIOA(void)
{
  if (!available_)
    return 0x00;

  return mcp_.readGPIOA();
}

inline uint8_t DevGpio::readGPIOB(void)
{
  if (!available_)
    return 0x00;

  return mcp_.readGPIOB();
}

#endif // __INTERNAL__DEVGPIO_H__
