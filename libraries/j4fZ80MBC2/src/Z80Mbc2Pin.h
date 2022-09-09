#ifndef __INTERNAL__Z80MBC2PIN_H__
#define __INTERNAL__Z80MBC2PIN_H__

#include <Arduino.h>
#include <stdint.h>

class Z80Mbc2Pin
{
public:
  void begin(void);
  uint8_t getPIN_AD0(void);
  uint8_t getPIN_nWR(void);
  uint8_t getPIN_nRD(void);
  uint8_t getPIN_nMREQ(void);
  void setPIN_nRESET_HIGH(void);
  void setPIN_nRESET_LOW(void);
  void setPIN_BANK1_HIGH(void);
  void setPIN_BANK1_LOW(void);
  void setPIN_BANK0_HIGH(void);
  void setPIN_BANK0_LOW(void);
  void setPIN_nINT_HIGH(void);
  void setPIN_nINT_LOW(void);
  void setPIN_RAM_CE2_HIGH(void);
  void setPIN_RAM_CE2_LOW(void);
  uint8_t getPIN_nWAIT(void);
  void setPIN_nBUSREQ_HIGH(void);
  void setPIN_nBUSREQ_LOW(void);
  void setPIN_CLK_HIGH(void);
  void setPIN_CLK_LOW(void);
  void setPIN_WAIT_nRES_HIGH(void);
  void setPIN_WAIT_nRES_LOW(void);
  uint8_t getPORT_DATA(void);
  void setPORT_DATA(uint8_t data);
  void releasePORT_DATA(void);

private:
  void __initPIN_AD0(void);
  void __initPIN_nWR(void);
  void __initPIN_nRD(void);
  void __initPIN_nMREQ(void);
  void __initPIN_nRESET(void);
  void __initPIN_BANK1(void);
  void __initPIN_BANK0(void);
  void __initPIN_nINT(void);
  void __initPIN_RAM_CE2(void);
  void __initPIN_nWAIT(void);
  void __initPIN_nBUSREQ(void);
  void __initPIN_CLK(void);
  void __initPIN_WAIT_nRES(void);
};

#endif // __INTERNAL__Z80MBC2PIN_H__
