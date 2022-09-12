#ifndef __INTERNAL__MBCDEVGPIO_H__
#define __INTERNAL__MBCDEVGPIO_H__

#include <j4fDev.h>

#include "MbcDevIo.h"

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
class MbcDevWrGPIOAClass : public MbcDevIo, public MbcDevGpio
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrGPIOAClass::run(MbcIo *io)
{
  gpio_->writeGPIOA(io->getData());
}

// GPIOB Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPIOB value (see MCP23017 datasheet)
class MbcDevWrGPIOBClass : public MbcDevIo, public MbcDevGpio
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrGPIOBClass::run(MbcIo *io)
{
  gpio_->writeGPIOB(io->getData());
}

// IODIRA Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    IODIRA value (see MCP23017 datasheet)
class MbcDevWrIODIRAClass : public MbcDevIo, public MbcDevGpio
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrIODIRAClass::run(MbcIo *io)
{
  gpio_->writeIODIRA(io->getData());
}

// IODIRB Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    IODIRB value (see MCP23017 datasheet)
class MbcDevWrIODIRBClass : public MbcDevIo, public MbcDevGpio
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrIODIRBClass::run(MbcIo *io)
{
  gpio_->writeIODIRB(io->getData());
}

// GPPUA Write (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPPUA value (see MCP23017 datasheet)
class MbcDevWrGPPUAClass : public MbcDevIo, public MbcDevGpio
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrGPPUAClass::run(MbcIo *io)
{
  gpio_->writeGPPUA(io->getData());
}

// GPPUB Write (GPIO Exp. Mod. ):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPPUB value (see MCP23017 datasheet)
class MbcDevWrGPPUBClass : public MbcDevIo, public MbcDevGpio
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrGPPUBClass::run(MbcIo *io)
{
  gpio_->writeGPPUB(io->getData());
}

// GPIOA Read (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPIOA value (see MCP23017 datasheet)
//
// NOTE: a value 0x00 is forced if the GPE Option is not present
class MbcDevRdGPIOAClass : public MbcDevIo, public MbcDevGpio
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdGPIOAClass::run(MbcIo *io)
{
  io->setData(gpio_->readGPIOA());
}

// GPIOB Read (GPE Option):
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    GPIOB value (see MCP23017 datasheet)
//
// NOTE: a value 0x00 is forced if the GPE Option is not present
class MbcDevRdGPIOBClass : public MbcDevIo, public MbcDevGpio
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdGPIOBClass::run(MbcIo *io)
{
  io->setData(gpio_->readGPIOB());
}

extern MbcDevWrGPIOAClass MbcDevWrGPIOA;
extern MbcDevWrGPIOBClass MbcDevWrGPIOB;
extern MbcDevWrIODIRAClass MbcDevWrIODIRA;
extern MbcDevWrIODIRBClass MbcDevWrIODIRB;
extern MbcDevWrGPPUAClass MbcDevWrGPPUA;
extern MbcDevWrGPPUBClass MbcDevWrGPPUB;
extern MbcDevRdGPIOAClass MbcDevRdGPIOA;
extern MbcDevRdGPIOBClass MbcDevRdGPIOB;

#endif /* __INTERNAL__MBCDEVGPIO_H__ */
