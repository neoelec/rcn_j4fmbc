#ifndef __INTERNAL__Z80MBC2BUS_H__
#define __INTERNAL__Z80MBC2BUS_H__

#include <j4fDev.h>
#include <j4fMbc.h>

#include "Z80Mbc2Dev.h"
#include "Z80Mbc2DevBank.h"

class Z80Mbc2Io : public MbcIo
{
public:
  void begin(Z80Mbc2Dev &dev);
  void run(void);
  uint8_t getSysFlag(void);

private:
  void __beginIoDev(Z80Mbc2Dev &dev);
  void __initFromCfg(Z80Mbc2Dev &dev);
  void __initIoDevWr(void);
  void __initIoDevRd(void);
  void __setIoDevWr(uint8_t command, MbcDev *dev);
  void __setIoDevRd(uint8_t command, MbcDev *dev);
  inline void __runWrite(void);
  inline void __runRead(void);
  inline void __runInterrupt(void);
  inline void __execWriteCommand(void);
  inline void __execReadCommand(void);

private:
  Z80Mbc2Pin *pin_;
  uint8_t staticSysFlags_;
  uint8_t wait_count_;
  uint8_t last_rx_is_empty_;

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

  Z80Mbc2DevWrSETBANK wr_setbank_;
};

#endif // __INTERNAL__Z80MBC2BUS_H__
