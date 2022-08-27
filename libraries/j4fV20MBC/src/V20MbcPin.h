#ifndef __INTERNAL__V20MBCPIN_H__
#define __INTERNAL__V20MBCPIN_H__

#include <Arduino.h>
#include <stdint.h>

#define PIN_nINTA 23     // PC7 pin 29
#define __PIN_nINTA 7    // PC7
#define PIN_CLK 15       // PD7 pin 21
#define __PIN_CLK 7      // PD7
#define PIN_RES 2        // PB2 pin 3
#define __PIN_RES 2      // PB2
#define PIN_nRAMEN 14    // PD6 pin 20
#define __PIN_nRAMEN 6   // PD6
#define PIN_READY 10     // PD2 pin 16
#define __PIN_READY 2    // PD2
#define PIN_nWR 19       // PC3 pin 25
#define __PIN_nWR 3      // PC3 p
#define PIN_nRD 18       // PC2 pin 24
#define __PIN_nRD 2      // PC2
#define PIN_AD0 21       // PC5 pin 27
#define __PIN_AD0 5      // PC5
#define PIN_AD1 22       // PC6 pin 28
#define __PIN_AD1 6      // PC6
#define PIN_IO_nM 20     // PC4 pin 26
#define __PIN_IOnM 4     // PC4
#define PIN_INTR 3       // PB3 pin 4
#define __PIN_INTR 3     // PB3
#define PIN_nRDYRES 0    // PB0 pin 1
#define __PIN_nRDYRES 0  // PB0
#define PIN_nHOLDRES 1   // PB1 pin 2
#define __PIN_nHOLDRES 1 // PB1

class V20MbcPin
{
public:
  void begin(void);
  inline uint8_t getPIN_nINTA(void);
  void setPIN_CLK_HIGH(void);
  void setPIN_CLK_LOW(void);
  inline void setPIN_RES_HIGH(void);
  inline void setPIN_RES_LOW(void);
  inline void setPIN_nRAMEN_HIGH(void);
  inline void setPIN_nRAMEN_LOW(void);
  inline uint8_t getPIN_READY(void);
  inline uint8_t getPIN_nWR(void);
  inline uint8_t getPIN_nRD(void);
  inline uint8_t getPIN_AD1AD0(void);
  inline uint8_t getPIN_IO_nM(void);
  inline void setPIN_INTR_HIGH(void);
  inline void setPIN_INTR_LOW(void);
  inline void setPIN_nRDYRES_HIGH(void);
  inline void setPIN_nRDYRES_LOW(void);
  inline void setPIN_nHOLDRES_HIGH(void);
  inline void setPIN_nHOLDRES_LOW(void);
  inline uint8_t getPORT_DATA(void);
  inline void setPORT_DATA(uint8_t data);
  inline void releasePORT_DATA(void);

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
  inline uint8_t __demangleData(uint8_t data);
};

inline uint8_t V20MbcPin::getPIN_nINTA(void)
{
  return !!(PINC & _BV(__PIN_nINTA));
}

inline void V20MbcPin::setPIN_RES_HIGH(void)
{
  PORTB |= _BV(__PIN_RES);
}

inline void V20MbcPin::setPIN_RES_LOW(void)
{
  PORTB &= ~_BV(__PIN_RES);
}

inline void V20MbcPin::setPIN_nRAMEN_HIGH(void)
{
  PORTD |= _BV(__PIN_nRAMEN);
}

inline void V20MbcPin::setPIN_nRAMEN_LOW(void)
{
  PORTD &= ~_BV(__PIN_nRAMEN);
}

inline uint8_t V20MbcPin::getPIN_READY(void)
{
  return !!(PIND & _BV(__PIN_READY));
}

inline uint8_t V20MbcPin::getPIN_nWR(void)
{
  return !!(PINC & _BV(__PIN_nWR));
}

inline uint8_t V20MbcPin::getPIN_nRD(void)
{
  return !!(PINC & _BV(__PIN_nRD));
}

inline uint8_t V20MbcPin::getPIN_AD1AD0(void)
{
  return (PINC & (_BV(__PIN_AD1) | _BV(__PIN_AD0))) >> __PIN_AD0;
}

inline uint8_t V20MbcPin::getPIN_IO_nM(void)
{
  return !!(PINC & _BV(__PIN_IOnM));
}

inline void V20MbcPin::setPIN_INTR_HIGH(void)
{
  PORTB |= _BV(__PIN_INTR);
}

inline void V20MbcPin::setPIN_INTR_LOW(void)
{
  PORTB &= ~_BV(__PIN_INTR);
}

inline void V20MbcPin::setPIN_nRDYRES_HIGH(void)
{
  PORTB |= _BV(__PIN_nRDYRES);
}

inline void V20MbcPin::setPIN_nRDYRES_LOW(void)
{
  PORTB &= ~_BV(__PIN_nRDYRES);
}

inline void V20MbcPin::setPIN_nHOLDRES_HIGH(void)
{
  PORTB |= _BV(__PIN_nHOLDRES);
}

inline void V20MbcPin::setPIN_nHOLDRES_LOW(void)
{
  PORTB &= ~_BV(__PIN_nHOLDRES);
}

inline uint8_t V20MbcPin::__demangleData(uint8_t data)
{
  union
  {
    struct
    {
      uint8_t d3 : 1;
      uint8_t d2_1 : 2;
      uint8_t d0 : 1;
      uint8_t d7_4 : 4;
    };
    uint8_t raw;
  } __attribute__((packed)) a, b;

  a.raw = data;
  b.raw = data;
  b.d0 = a.d3;
  b.d3 = a.d0;

  return b.raw;
}

inline uint8_t V20MbcPin::getPORT_DATA(void)
{
  return __demangleData(PINA);
}

inline void V20MbcPin::setPORT_DATA(uint8_t data)
{
  DDRA = 0xFF;
  PORTA = __demangleData(data);
}

inline void V20MbcPin::releasePORT_DATA(void)
{
  DDRA = 0x00;
  PORTA = 0xFF;
}

#endif // __INTERNAL__V20MBCPIN_H__
