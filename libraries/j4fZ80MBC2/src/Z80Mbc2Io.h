#ifndef __INTERNAL__Z80MBC2BUS_H__
#define __INTERNAL__Z80MBC2BUS_H__

#include <j4fDev.h>
#include <j4fMbc.h>

#include "Z80Mbc2IrqStatus.h"
#include "Z80Mbc2SysCfg.h"
#include "Z80Mbc2Loader.h"
#include "Z80Mbc2Pin.h"

class Z80Mbc2IoClass;

class __Z80Mbc2IoState
{
public:
  virtual void handle(Z80Mbc2IoClass *io) = 0;
};

class __Z80Mbc2IoStateInit : public __Z80Mbc2IoState
{
public:
  void handle(Z80Mbc2IoClass *io);
};

class __Z80Mbc2IoStateMenuEnter : public __Z80Mbc2IoState
{
public:
  void handle(Z80Mbc2IoClass *io);
};

class __Z80Mbc2IoStateMenuRun : public __Z80Mbc2IoState
{
public:
  void handle(Z80Mbc2IoClass *io);
};

class __Z80Mbc2IoStateBootstrap : public __Z80Mbc2IoState
{
public:
  void handle(Z80Mbc2IoClass *io);
};

class __Z80Mbc2IoStateRun : public __Z80Mbc2IoState
{
public:
  void handle(Z80Mbc2IoClass *io);
};

class Z80Mbc2IoClass : public MbcIo
{
public:
  void begin(void);
  inline void run(void);
  inline void serialEvent(void);
  inline void runIo(void);
  uint8_t getSysFlag(void);
  void enableIrqTtyRx(void);
  void disableIrqTtyRx(void);
  void enableIrqTick(void);
  void disableIrqTick(void);
  void setSysTick(uint8_t sys_tick);
  void setIrqStatus(uint8_t irq_status);
  uint8_t getIrqStatus(void);

private:
  void __beginIoDev(void);
  void __initFromCfg(void);
  void __initIoDevWr(void);
  void __initIoDevRd(void);
  void __setIoDevWr(uint8_t command, MbcDevIo *dev);
  void __setIoDevRd(uint8_t command, MbcDevIo *dev);
  void __updateSysTick(void);
  inline void __runWrite(void);
  inline void __runRead(void);
  inline void __runInterrupt(void);
  inline void __execWriteCommand(void);
  inline void __execReadCommand(void);

private:
  Z80Mbc2Loader loader_;
  Z80Mbc2Pin pin_;
  uint8_t staticSysFlags_;
  uint8_t wait_count_;
  uint8_t sys_tick_;
  union Z80Mbc2IrqStatus irq_status_;
  uint8_t last_rx_is_empty_;
  bool irq_tty_rx_;
  bool irq_sys_tick_;
  unsigned long timestamp_;

  __Z80Mbc2IoState *state_;
  __Z80Mbc2IoStateInit st_init_;
  __Z80Mbc2IoStateMenuEnter st_menu_enter_;
  __Z80Mbc2IoStateMenuRun st_menu_run_;
  __Z80Mbc2IoStateBootstrap st_bootstrap_;
  __Z80Mbc2IoStateRun st_run_;

  MbcDevIo *io_dev_wr_[MbcIo::WR_NR_CMD];
  MbcDevIo *io_dev_rd_[MbcIo::RD_NR_CMD];

  Z80Mbc2DevWrSETBANK MbcDevWrSETBANK;
  Z80Mbc2DevWrSETIRQ MbcDevWrSETIRQ;
  Z80Mbc2DevWrSETTICK MbcDevWrSETTICK;
  Z80Mbc2DevRdSYSIRQ MbcDevRdSYSIRQ;

  friend class __Z80Mbc2IoState;
  friend class __Z80Mbc2IoStateInit;
  friend class __Z80Mbc2IoStateMenuEnter;
  friend class __Z80Mbc2IoStateMenuRun;
  friend class __Z80Mbc2IoStateBootstrap;
  friend class __Z80Mbc2IoStateRun;
};

extern Z80Mbc2IoClass Z80Mbc2Io;

inline void Z80Mbc2IoClass::run(void)
{
  state_->handle(this);
}

inline void Z80Mbc2IoClass::serialEvent(void)
{
  static unsigned long last;
  unsigned long timestamp;

  /* FIXME: This is an workaround to prevent IRQ storimg */
  timestamp = millis();
  if ((timestamp - last) < 125)
    return;

  last = timestamp;

  if (irq_tty_rx_ && (Serial.available() > 0) && last_rx_is_empty_ == 0) {
    pin_.setPIN_nINT_LOW();
    irq_status_.rx_ = 1;
    pin_.setPIN_nINT_HIGH();
  }
}

#endif // __INTERNAL__Z80MBC2BUS_H__
