// SPDX-License-Identifier: GPL-3.0-or-later

#include "j4fV20Mbc.h"

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
