#ifndef __INTERNAL__V20MBCPIN_H__
#define __INTERNAL__V20MBCPIN_H__

#include <Arduino.h>
#include <stdint.h>

class V20MbcPin
{
public:
  void begin(void);
  uint8_t getPIN_nINTA(void);
  void setPIN_CLK_HIGH(void);
  void setPIN_CLK_LOW(void);
  void setPIN_RES_HIGH(void);
  void setPIN_RES_LOW(void);
  void setPIN_nRAMEN_HIGH(void);
  void setPIN_nRAMEN_LOW(void);
  uint8_t getPIN_READY(void);
  uint8_t getPIN_nWR(void);
  uint8_t getPIN_nRD(void);
  uint8_t getPIN_AD1AD0(void);
  uint8_t getPIN_IO_nM(void);
  void setPIN_INTR_HIGH(void);
  void setPIN_INTR_LOW(void);
  void setPIN_nRDYRES_HIGH(void);
  void setPIN_nRDYRES_LOW(void);
  void setPIN_nHOLDRES_HIGH(void);
  void setPIN_nHOLDRES_LOW(void);
  uint8_t getPORT_DATA(void);
  void setPORT_DATA(uint8_t data);
  void releasePORT_DATA(void);

private:
  void __initPIN_nINTA(void);
  void __initPIN_CLK(void);
  void __initPIN_RES(void);
  void __initPIN_nRAMEN(void);
  void __initPIN_READY(void);
  void __initPIN_nWR(void);
  void __initPIN_nRD(void);
  void __initPIN_AD1AD0(void);
  void __initPIN_IO_nM(void);
  void __initPIN_INTR(void);
  void __initPIN_nRDYRES(void);
  void __initPIN_nHOLDRES(void);
  uint8_t __demangleData(uint8_t data);
};

#endif // __INTERNAL__V20MBCPIN_H__
