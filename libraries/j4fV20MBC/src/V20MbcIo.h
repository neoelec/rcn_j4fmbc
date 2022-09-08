#ifndef __INTERNAL__V20MBCBUS_H__
#define __INTERNAL__V20MBCBUS_H__

#include <j4fDev.h>
#include <j4fMbc.h>

#include "V20MbcDev.h"
#include "V20MbcLoader.h"
#include "V20MbcPin.h"

class V20MbcIoClass;

class __V20MbcIoState
{
public:
  virtual void handle(V20MbcIoClass *io) = 0;
};

class __V20MbcIoStateInit : public __V20MbcIoState
{
public:
  void handle(V20MbcIoClass *io);
};

class __V20MbcIoStateMenuEnter : public __V20MbcIoState
{
public:
  void handle(V20MbcIoClass *io);
};

class __V20MbcIoStateMenuRun : public __V20MbcIoState
{
public:
  void handle(V20MbcIoClass *io);
};

class __V20MbcIoStateBootstrap : public __V20MbcIoState
{
public:
  void handle(V20MbcIoClass *io);
};

class __V20MbcIoStateRun : public __V20MbcIoState
{
public:
  void handle(V20MbcIoClass *io);
};

class V20MbcIoClass : public MbcIo
{
public:
  void begin(void);
  inline void run(void);
  inline void serialEvent(void);
  inline void operateIo(void);
  uint8_t getSysFlag(void);

private:
  void __beginIoDev(void);
  void __initFromCfg(void);
  void __initIoDevWr(void);
  void __initIoDevRd(void);
  void __setIoDevWr(uint8_t command, MbcDev *dev);
  void __setIoDevRd(uint8_t command, MbcDev *dev);
  inline void __runWrite(void);
  inline void __runRead(void);
  inline void __runInterrupt(void);
  inline void __runHalt(void);
  inline void __execWriteOpcode(void);
  inline void __execReadOpcode(void);

private:
  V20MbcDev dev_;
  V20MbcLoader loader_;

  V20MbcPin *pin_;
  uint8_t staticSysFlags_;
  uint8_t autoexec_en_;
  uint8_t wait_count_;

  __V20MbcIoState *state_;
  __V20MbcIoStateInit st_init_;
  __V20MbcIoStateMenuEnter st_menu_enter_;
  __V20MbcIoStateMenuRun st_menu_run_;
  __V20MbcIoStateBootstrap st_bootstrap_;
  __V20MbcIoStateRun st_run_;

  MbcDev *io_dev_wr_[MbcIo::WR_NR_CMD];
  MbcDev *io_dev_rd_[MbcIo::RD_NR_CMD];

  MbcDevRdWrNOP rdwr_nop_;

  MbcDevWrSELDISK wr_seldisk_;
  MbcDevWrSELTRACK wr_seltrack_;
  MbcDevWrSELSECT wr_selsect_;
  MbcDevWrWRITESECT wr_writesect_;
  MbcDevRdERRDISK rd_errdisk_;
  MbcDevRdREADSECT rd_readsect_;
  MbcDevRdSDMOUNT rd_sdmount_;

  MbcDevWrGPIOA wr_gpioa_;
  MbcDevWrGPIOB wr_gpiob_;
  MbcDevWrIODIRA wr_iodira_;
  MbcDevWrIODIRB wr_iodirb_;
  MbcDevWrGPPUA wr_gppua_;
  MbcDevWrGPPUB wr_gppub_;
  MbcDevRdGPIOA rd_gpioa_;
  MbcDevRdGPIOB rd_gpiob_;

  MbcDevRdDATETIME rd_datetime_;

  MbcDevRdSYSFLAGS rd_sysflags_;

  MbcDevRdSERIALRX rd_serialrx_;
  MbcDevWrSERIALTX wr_serialtx_;
  MbcDevWrRXIRQFLAG wr_rxirqflag_;
  MbcDevRdATXBUFF rd_atxbuff_;

  MbcDevWrUSERLED wr_userled_;
  MbcDevRdUSERKEY rd_userkey_;

  friend class __V20MbcIoState;
  friend class __V20MbcIoStateInit;
  friend class __V20MbcIoStateMenuEnter;
  friend class __V20MbcIoStateMenuRun;
  friend class __V20MbcIoStateBootstrap;
  friend class __V20MbcIoStateRun;
};

extern V20MbcIoClass V20MbcIo;

inline void V20MbcIoClass::run(void)
{
  state_->handle(this);
}

inline void V20MbcIoClass::serialEvent(void)
{
}

#endif // __INTERNAL__V20MBCBUS_H__
