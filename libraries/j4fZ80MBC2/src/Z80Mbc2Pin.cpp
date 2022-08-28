// SPDX-License-Identifier: GPL-3.0-or-later

#include "j4fZ80Mbc2.h"

void Z80Mbc2Pin::begin(void)
{
  __initPIN_AD0();
  __initPIN_nWR();
  __initPIN_nRD();
  __initPIN_nMREQ();
  __initPIN_nRESET();
  __initPIN_BANK1();
  __initPIN_BANK0();
  __initPIN_nINT();
  __initPIN_RAM_CE2();
  __initPIN_nWAIT();
  __initPIN_nBUSREQ();
  __initPIN_CLK();
  __initPIN_WAIT_nRES();
  releasePORT_DATA();
}

void Z80Mbc2Pin::__initPIN_AD0(void)
{
  pinMode(PIN_AD0, INPUT_PULLUP);
}

void Z80Mbc2Pin::__initPIN_nWR(void)
{
  pinMode(PIN_nWR, INPUT_PULLUP);
}

void Z80Mbc2Pin::__initPIN_nRD(void)
{
  pinMode(PIN_nRD, INPUT_PULLUP);
}

void Z80Mbc2Pin::__initPIN_nMREQ(void)
{
  pinMode(PIN_nMREQ, INPUT_PULLUP);
}

void Z80Mbc2Pin::__initPIN_nRESET(void)
{
  pinMode(PIN_nRESET, OUTPUT);
  setPIN_nRESET_LOW();
}

void Z80Mbc2Pin::__initPIN_BANK1(void)
{
  pinMode(PIN_BANK1, OUTPUT);
  setPIN_BANK1_LOW();
}

void Z80Mbc2Pin::__initPIN_BANK0(void)
{
  pinMode(PIN_BANK0, OUTPUT);
  setPIN_BANK0_HIGH();
}

void Z80Mbc2Pin::__initPIN_nINT(void)
{
  pinMode(PIN_nINT, OUTPUT);
  setPIN_nINT_HIGH();
}

void Z80Mbc2Pin::__initPIN_RAM_CE2(void)
{
  pinMode(PIN_RAM_CE2, OUTPUT);
  setPIN_RAM_CE2_HIGH();
}

void Z80Mbc2Pin::__initPIN_nWAIT(void)
{
  pinMode(PIN_nWAIT, INPUT);
}

void Z80Mbc2Pin::__initPIN_nBUSREQ(void)
{
  pinMode(PIN_nBUSREQ, OUTPUT);
  setPIN_nBUSREQ_HIGH();
}

void Z80Mbc2Pin::__initPIN_CLK(void)
{
  pinMode(PIN_CLK, OUTPUT);
  setPIN_CLK_LOW();
}

void Z80Mbc2Pin::__initPIN_WAIT_nRES(void)
{
  pinMode(PIN_WAIT_nRES, OUTPUT);
  setPIN_WAIT_nRES_LOW();
}
