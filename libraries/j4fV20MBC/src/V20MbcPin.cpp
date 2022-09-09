// SPDX-License-Identifier: GPL-3.0-or-later

#include "j4fV20Mbc.h"

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

void V20MbcPin::begin(void)
{
  __initPIN_nINTA();
  __initPIN_CLK();
  __initPIN_RES();
  __initPIN_nRAMEN();
  __initPIN_READY();
  __initPIN_nWR();
  __initPIN_nRD();
  __initPIN_AD1AD0();
  __initPIN_IO_nM();
  __initPIN_INTR();
  __initPIN_nRDYRES();
  __initPIN_nHOLDRES();
  releasePORT_DATA();
}

void V20MbcPin::setPIN_CLK_HIGH(void)
{
  PORTD |= _BV(__PIN_CLK);
}

void V20MbcPin::setPIN_CLK_LOW(void)
{
  PORTD &= ~_BV(__PIN_CLK);
}

void V20MbcPin::__initPIN_nINTA(void)
{
  pinMode(PIN_nINTA, INPUT);
}

void V20MbcPin::__initPIN_CLK(void)
{
  pinMode(PIN_CLK, OUTPUT);
  setPIN_CLK_LOW();
}

void V20MbcPin::__initPIN_RES(void)
{
  pinMode(PIN_RES, OUTPUT);
  setPIN_RES_LOW();
}

void V20MbcPin::__initPIN_nRAMEN(void)
{
  pinMode(PIN_nRAMEN, OUTPUT);
  setPIN_nRAMEN_HIGH();
}

void V20MbcPin::__initPIN_READY(void)
{
  pinMode(PIN_READY, INPUT);
}

void V20MbcPin::__initPIN_nWR(void)
{
  pinMode(PIN_nWR, INPUT_PULLUP);
}

void V20MbcPin::__initPIN_nRD(void)
{
  pinMode(PIN_nRD, INPUT_PULLUP);
}

void V20MbcPin::__initPIN_AD1AD0(void)
{
  pinMode(PIN_AD0, INPUT_PULLUP);
  pinMode(PIN_AD1, INPUT_PULLUP);
}

void V20MbcPin::__initPIN_IO_nM(void)
{
  pinMode(PIN_IO_nM, INPUT_PULLUP);
}

void V20MbcPin::__initPIN_INTR(void)
{
  pinMode(PIN_INTR, OUTPUT);
  setPIN_INTR_LOW();
}

void V20MbcPin::__initPIN_nRDYRES(void)
{
  pinMode(PIN_nRDYRES, OUTPUT);
  setPIN_nRDYRES_LOW();
}

void V20MbcPin::__initPIN_nHOLDRES(void)
{
  pinMode(PIN_nHOLDRES, OUTPUT);
  setPIN_nHOLDRES_LOW();
}

uint8_t V20MbcPin::getPIN_nINTA(void)
{
  return !!(PINC & _BV(__PIN_nINTA));
}

void V20MbcPin::setPIN_RES_HIGH(void)
{
  PORTB |= _BV(__PIN_RES);
}

void V20MbcPin::setPIN_RES_LOW(void)
{
  PORTB &= ~_BV(__PIN_RES);
}

void V20MbcPin::setPIN_nRAMEN_HIGH(void)
{
  PORTD |= _BV(__PIN_nRAMEN);
}

void V20MbcPin::setPIN_nRAMEN_LOW(void)
{
  PORTD &= ~_BV(__PIN_nRAMEN);
}

uint8_t V20MbcPin::getPIN_READY(void)
{
  return !!(PIND & _BV(__PIN_READY));
}

uint8_t V20MbcPin::getPIN_nWR(void)
{
  return !!(PINC & _BV(__PIN_nWR));
}

uint8_t V20MbcPin::getPIN_nRD(void)
{
  return !!(PINC & _BV(__PIN_nRD));
}

uint8_t V20MbcPin::getPIN_AD1AD0(void)
{
  return (PINC & (_BV(__PIN_AD1) | _BV(__PIN_AD0))) >> __PIN_AD0;
}

uint8_t V20MbcPin::getPIN_IO_nM(void)
{
  return !!(PINC & _BV(__PIN_IOnM));
}

void V20MbcPin::setPIN_INTR_HIGH(void)
{
  PORTB |= _BV(__PIN_INTR);
}

void V20MbcPin::setPIN_INTR_LOW(void)
{
  PORTB &= ~_BV(__PIN_INTR);
}

void V20MbcPin::setPIN_nRDYRES_HIGH(void)
{
  PORTB |= _BV(__PIN_nRDYRES);
}

void V20MbcPin::setPIN_nRDYRES_LOW(void)
{
  PORTB &= ~_BV(__PIN_nRDYRES);
}

void V20MbcPin::setPIN_nHOLDRES_HIGH(void)
{
  PORTB |= _BV(__PIN_nHOLDRES);
}

void V20MbcPin::setPIN_nHOLDRES_LOW(void)
{
  PORTB &= ~_BV(__PIN_nHOLDRES);
}

uint8_t V20MbcPin::__demangleData(uint8_t data)
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

uint8_t V20MbcPin::getPORT_DATA(void)
{
  return __demangleData(PINA);
}

void V20MbcPin::setPORT_DATA(uint8_t data)
{
  DDRA = 0xFF;
  PORTA = __demangleData(data);
}

void V20MbcPin::releasePORT_DATA(void)
{
  DDRA = 0x00;
  PORTA = 0xFF;
}
