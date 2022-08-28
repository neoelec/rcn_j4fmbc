// SPDX-License-Identifier: GPL-3_.0-or-later

#include "j4fZ80Mbc2.h"

#define PIN_USER 13     // PD5 pin 19   Led USER and key (led USER is ON if LOW)
#define PIN_MCU_nRTS 23 // PC7 pin 29   * RESERVED - NOT USED *
#define PIN_MCU_nCTS 10 // PD2 pin 16   * RESERVED - NOT USED *

void __Z80Mbc2DevMcuVariant::begin(void)
{
  clock_.begin();
}

DevClock *__Z80Mbc2DevMcuVariant::getClock(void)
{
  return &clock_;
}

void Z80Mbc2Dev::begin(void)
{
  tty_.begin(PIN_MCU_nRTS, PIN_MCU_nCTS);

  Serial.println();
  Serial.println(F("Z80-MBC2 - A040618"));
  Serial.println(F("IOS - I/O Subsystem - S220718-R240620 + Raccoon's MOD"));
  Serial.println();

  __Z80Mbc2DevMcuVariant::begin();

  gpio_.begin();
  rtc_.begin();
  sd_.begin();
  user_.begin(PIN_USER);

  disk_.begin(&sd_);

  pin_.begin();
}

DevGpio *Z80Mbc2Dev::getGpio(void)
{
  return &gpio_;
}

DevRtc *Z80Mbc2Dev::getRtc(void)
{
  return &rtc_;
}

DevSd *Z80Mbc2Dev::getSd(void)
{
  return &sd_;
}

DevTty *Z80Mbc2Dev::getTty(void)
{
  return &tty_;
}

DevUser *Z80Mbc2Dev::getUser(void)
{
  return &user_;
}

MbcDisk *Z80Mbc2Dev::getDisk(void)
{
  return &disk_;
}

Z80Mbc2Pin *Z80Mbc2Dev::getPin(void)
{
  return &pin_;
}
