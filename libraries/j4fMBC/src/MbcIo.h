#ifndef __INTERNAL__MBCBUS_H__
#define __INTERNAL__MBCBUS_H__

#include <stdint.h>

class MbcDevIo;

class MbcIo
{
public:
  enum
  {
    WR_BEGIN = 0x0,
    WR_USERLED = WR_BEGIN,
    WR_SERIALTX,
    WR_RXIRQFLAG,
    WR_GPIOA,
    WR_GPIOB,
    WR_IODIRA,
    WR_IODIRB,
    WR_GPPUA,
    WR_GPPUB,
    WR_SELDISK,
    WR_SELTRACK,
    WR_SELSECT,
    WR_WRITESECT,
    WR_SETBANK,
    WR_END = WR_SETBANK,
    WR_NR_CMD = WR_END - WR_BEGIN + 1,
    RD_BEGIN = 0x80,
    RD_USERKEY = RD_BEGIN,
    RD_GPIOA,
    RD_GPIOB,
    RD_SYSFLAGS,
    RD_DATETIME,
    RD_ERRDISK,
    RD_READSECT,
    RD_SDMOUNT,
    RD_ATXBUFF,
    RD_END = RD_ATXBUFF,
    RD_NR_CMD = RD_END - RD_BEGIN + 1,
    NO_OPERATION = 0xFF,
  };

  enum
  {
    IRQ_RX
  };

  MbcIo() : command_(NO_OPERATION)
  {
  }
  inline uint8_t getAddress(void);
  inline uint8_t getData(void);
  inline uint8_t getCommand(void);
  inline uint16_t getCount(void);
  inline bool getIrq(uint8_t number);
  inline void setData(uint8_t data);
  inline void setCommand(uint8_t command);
  inline void setCount(uint16_t count);
  inline void clearIrq(uint8_t number);
  inline void setIrq(uint8_t number);
  virtual void begin(void) = 0;
  virtual void run(void) = 0;
  virtual void serialEvent(void) = 0;
  virtual uint8_t getSysFlag(void) = 0;

protected:
  inline void _setAddress(uint8_t address);

private:
  uint8_t address_;
  uint8_t data_;
  uint8_t command_;
  uint16_t count_;
  uint8_t irq_;
};

inline uint8_t MbcIo::getAddress(void)
{
  return address_;
}

inline uint8_t MbcIo::getData(void)
{
  return data_;
}

inline uint8_t MbcIo::getCommand(void)
{
  return command_;
}

inline uint16_t MbcIo::getCount(void)
{
  return count_;
}

inline bool MbcIo::getIrq(uint8_t number)
{
  return !!(irq_ & (0x1 << number));
}

inline void MbcIo::setData(uint8_t data)
{
  data_ = data;
}

inline void MbcIo::setCommand(uint8_t command)
{
  command_ = command;
}

inline void MbcIo::setCount(uint16_t count)
{
  count_ = count;
}

inline void MbcIo::clearIrq(uint8_t number)
{
  irq_ &= ~(0x1 << number);
}

inline void MbcIo::setIrq(uint8_t number)
{
  irq_ |= (0x1 << number);
}

inline void MbcIo::_setAddress(uint8_t address)
{
  address_ = address;
}

#endif // __INTERNAL__MBCBUS_H__
