#ifndef __INTERNAL__Z80MBC2PIN_H__
#define __INTERNAL__Z80MBC2PIN_H__

#include <Arduino.h>
#include <stdint.h>

#define PIN_AD0 18        // PC2 pin 24   Z80 A0
#define __PIN_AD0 2       // PC2
#define PIN_nWR 19        // PC3 pin 25   Z80 WR
#define __PIN_nWR 3       // PC3
#define PIN_nRD 20        // PC4 pin 26   Z80 RD
#define __PIN_nRD 4       // PC4
#define PIN_nMREQ 21      // PC5 pin 27   Z80 MREQ
#define __PIN_nMREQ 5     // PC5
#define PIN_nRESET 22     // PC6 pin 28   Z80 RESET
#define __PIN_nRESET 6    // PC6
#define PIN_BANK1 11      // PD3 pin 17   RAM Memory bank address (High)
#define __PIN_BANK1 3     // PD3
#define PIN_BANK0 12      // PD4 pin 18   RAM Memory bank address (Low)
#define __PIN_BANK0 4     // PD4
#define PIN_nINT 1        // PB1 pin 2    Z80 control bus
#define __PIN_nINT 1      // PB1
#define PIN_RAM_CE2 2     // PB2 pin 3    RAM Chip Enable (CE2). Active HIGH. Used only during boot
#define __PIN_RAM_CE2 2   // PB2
#define PIN_nWAIT 3       // PB3 pin 4    Z80 WAIT
#define __PIN_nWAIT 3     // PB3
#define PIN_nBUSREQ 14    // PD6 pin 20   Z80 BUSRQ
#define __PIN_nBUSREQ 6   // PD6
#define PIN_CLK 15        // PD7 pin 21   Z80 CLK
#define __PIN_CLK 7       // PD7
#define PIN_WAIT_nRES 0   // PB0 pin 1    Reset the Wait FF
#define __PIN_WAIT_nRES 0 // PB0

class Z80Mbc2Pin
{
public:
  void begin(void);
  inline uint8_t getPIN_AD0(void);
  inline uint8_t getPIN_nWR(void);
  inline uint8_t getPIN_nRD(void);
  inline uint8_t getPIN_nMREQ(void);
  inline void setPIN_nRESET_HIGH(void);
  inline void setPIN_nRESET_LOW(void);
  inline void setPIN_BANK1_HIGH(void);
  inline void setPIN_BANK1_LOW(void);
  inline void setPIN_BANK0_HIGH(void);
  inline void setPIN_BANK0_LOW(void);
  inline void setPIN_nINT_HIGH(void);
  inline void setPIN_nINT_LOW(void);
  inline void setPIN_RAM_CE2_HIGH(void);
  inline void setPIN_RAM_CE2_LOW(void);
  inline uint8_t getPIN_nWAIT(void);
  inline void setPIN_nBUSREQ_HIGH(void);
  inline void setPIN_nBUSREQ_LOW(void);
  inline void setPIN_CLK_HIGH(void);
  inline void setPIN_CLK_LOW(void);
  inline void setPIN_WAIT_nRES_HIGH(void);
  inline void setPIN_WAIT_nRES_LOW(void);
  inline uint8_t getPORT_DATA(void);
  inline void setPORT_DATA(uint8_t data);
  inline void releasePORT_DATA(void);

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

inline uint8_t Z80Mbc2Pin::getPIN_AD0(void)
{
  return !!(PINC & _BV(__PIN_AD0));
}

inline uint8_t Z80Mbc2Pin::getPIN_nWR(void)
{
  return !!(PINC & _BV(__PIN_nWR));
}

inline uint8_t Z80Mbc2Pin::getPIN_nRD(void)
{
  return !!(PINC & _BV(__PIN_nRD));
}

inline uint8_t Z80Mbc2Pin::getPIN_nMREQ(void)
{
  return !!(PINC & _BV(__PIN_nMREQ));
}

inline void Z80Mbc2Pin::setPIN_nRESET_HIGH(void)
{
  PORTC |= _BV(__PIN_nRESET);
}

inline void Z80Mbc2Pin::setPIN_nRESET_LOW(void)
{
  PORTC &= ~_BV(__PIN_nRESET);
}

inline void Z80Mbc2Pin::setPIN_BANK1_HIGH(void)
{
  PORTD |= _BV(__PIN_BANK1);
}

inline void Z80Mbc2Pin::setPIN_BANK1_LOW(void)
{
  PORTD &= ~_BV(__PIN_BANK1);
}

inline void Z80Mbc2Pin::setPIN_BANK0_HIGH(void)
{
  PORTD |= _BV(__PIN_BANK0);
}

inline void Z80Mbc2Pin::setPIN_BANK0_LOW(void)
{
  PORTD &= ~_BV(__PIN_BANK0);
}

inline void Z80Mbc2Pin::setPIN_nINT_HIGH(void)
{
  PORTB |= _BV(__PIN_nINT);
}

inline void Z80Mbc2Pin::setPIN_nINT_LOW(void)
{
  PORTB &= ~_BV(__PIN_nINT);
}

inline void Z80Mbc2Pin::setPIN_RAM_CE2_HIGH(void)
{
  PORTB |= _BV(__PIN_RAM_CE2);
}

inline void Z80Mbc2Pin::setPIN_RAM_CE2_LOW(void)
{
  PORTB &= ~_BV(__PIN_RAM_CE2);
}

inline uint8_t Z80Mbc2Pin::getPIN_nWAIT(void)
{
  return !!(PINB & _BV(__PIN_nWAIT));
}

inline void Z80Mbc2Pin::setPIN_nBUSREQ_HIGH(void)
{
  PORTD |= _BV(__PIN_nBUSREQ);
}

inline void Z80Mbc2Pin::setPIN_nBUSREQ_LOW(void)
{
  PORTD &= ~_BV(__PIN_nBUSREQ);
}

inline void Z80Mbc2Pin::setPIN_CLK_HIGH(void)
{
  PORTD |= _BV(__PIN_CLK);
}

inline void Z80Mbc2Pin::setPIN_CLK_LOW(void)
{
  PORTD &= ~_BV(__PIN_CLK);
}

inline void Z80Mbc2Pin::setPIN_WAIT_nRES_HIGH(void)
{
  PORTB |= _BV(__PIN_WAIT_nRES);
}

inline void Z80Mbc2Pin::setPIN_WAIT_nRES_LOW(void)
{
  PORTB &= ~_BV(__PIN_WAIT_nRES);
}

inline uint8_t Z80Mbc2Pin::getPORT_DATA(void)
{
  return PINA;
}

inline void Z80Mbc2Pin::setPORT_DATA(uint8_t data)
{
  DDRA = 0xFF;
  PORTA = data;
}

inline void Z80Mbc2Pin::releasePORT_DATA(void)
{
  DDRA = 0x00;
  PORTA = 0xFF;
}

#endif // __INTERNAL__Z80MBC2PIN_H__
