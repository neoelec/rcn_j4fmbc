#ifndef __INTERNAL__V20MBCBUS_H__
#define __INTERNAL__V20MBCBUS_H__

#include <j4fDev.h>
#include <j4fMbc.h>

#include "V20MbcDev.h"

class V20MbcIo : public MbcIo
{
public:
  void begin(V20MbcDev &dev);
  void run(void);

private:
  void __beginIoDev(V20MbcDev &dev);
  void __initFromCfg(V20MbcDev &dev);
  inline void __runWrite(void);
  inline void __runRead(void);
  inline void __runInterrupt(void);
  inline void __runHalt(void);
  inline void __execWriteOpcode(void);
  inline void __execReadOpcode(void);
  inline void __runRdSYSFLAGS(void);

private:
  DevRtc *rtc_;
  V20MbcPin *pin_;
  uint8_t autoexec_en_;
  uint8_t wait_count_;

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

  MbcDevRdSERIALRX rd_serialrx_;
  MbcDevWrSERIALTX wr_serialtx_;
  MbcDevWrRXIRQFLAG wr_rxirqflag_;
  MbcDevRdATXBUFF rd_atxbuff_;

  MbcDevWrUSERLED wr_userled_;
  MbcDevRdUSERKEY rd_userkey_;
};

#endif // __INTERNAL__V20MBCBUS_H__
