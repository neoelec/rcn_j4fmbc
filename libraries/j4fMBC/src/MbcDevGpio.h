#ifndef __INTERNAL__MBCDEVGPIO_H__
#define __INTERNAL__MBCDEVGPIO_H__

#include <DevGpio.h>

#include "MbcDev.h"
#include "MbcIo.h"

class MbcDevGpio
{
public:
  void begin(DevGpio *gpio)
  {
    gpio_ = gpio;
  }

protected:
  DevGpio *gpio_;
};

// GPIOA Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPIOA value (see MCP23017 datasheet)
class MbcDevWrGPIOA : public MbcDev, public MbcDevGpio
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevWrGPIOA::run(MbcIo &io)
{
  gpio_->writeGPIOA(io.getData());
}

// GPIOB Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPIOB value (see MCP23017 datasheet)
class MbcDevWrGPIOB : public MbcDev, public MbcDevGpio
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevWrGPIOB::run(MbcIo &io)
{
  gpio_->writeGPIOB(io.getData());
}

// IODIRA Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    IODIRA value (see MCP23017 datasheet)
class MbcDevWrIODIRA : public MbcDev, public MbcDevGpio
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevWrIODIRA::run(MbcIo &io)
{
  gpio_->writeIODIRA(io.getData());
}

// IODIRB Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    IODIRB value (see MCP23017 datasheet)
class MbcDevWrIODIRB : public MbcDev, public MbcDevGpio
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevWrIODIRB::run(MbcIo &io)
{
  gpio_->writeIODIRB(io.getData());
}

// GPPUA Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPPUA value (see MCP23017 datasheet)
class MbcDevWrGPPUA : public MbcDev, public MbcDevGpio
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevWrGPPUA::run(MbcIo &io)
{
  gpio_->writeGPPUA(io.getData());
}

// GPPUB Write (GPIO Exp. Mod. ):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPPUB value (see MCP23017 datasheet)
class MbcDevWrGPPUB : public MbcDev, public MbcDevGpio
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevWrGPPUB::run(MbcIo &io)
{
  gpio_->writeGPPUB(io.getData());
}

// GPIOA Read (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPIOA value (see MCP23017 datasheet)
//
// NOTE: a value 0x00 is forced if the GPE Option is not present
class MbcDevRdGPIOA : public MbcDev, public MbcDevGpio
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevRdGPIOA::run(MbcIo &io)
{
  io.setData(gpio_->readGPIOA());
}

// GPIOB Read (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPIOB value (see MCP23017 datasheet)
//
// NOTE: a value 0x00 is forced if the GPE Option is not present
class MbcDevRdGPIOB : public MbcDev, public MbcDevGpio
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevRdGPIOB::run(MbcIo &io)
{
  io.setData(gpio_->readGPIOB());
}

#endif /* __INTERNAL__MBCDEVGPIO_H__ */
