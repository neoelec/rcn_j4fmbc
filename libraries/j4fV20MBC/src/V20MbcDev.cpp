// SPDX-License-Identifier: GPL-3_.0-or-later

#include "j4fV20Mbc.h"

#define PIN_USER 13     // PD5 pin 19   Led USER and key (led USER is ON if LOW)
#define PIN_MCU_nRTS 11 // PD3 pin 17   * RESERVED - NOT USED *
#define PIN_MCU_nCTS 12 // PD4 pin 18   Used only to reset uTerm at boot time

DevClock *__V20MbcDevMcuVariant::getClock(void)
{
  return &clock_;
}

void V20MbcDev::begin(void)
{
  tty_.begin(PIN_MCU_nRTS, PIN_MCU_nCTS);

  Serial.println();
  Serial.println(F("V20-MBC - A250220"));
  Serial.println(F("IOS - I/O Subsystem - S260320-R230520 + Raccoon's MOD"));
  Serial.println();

  clock_.begin();
  gpio_.begin();
  rtc_.begin();
  sd_.begin();
  user_.begin(PIN_USER);

  disk_.begin(&sd_);

  pin_.begin();
}

DevGpio *V20MbcDev::getGpio(void)
{
  return &gpio_;
}

DevRtc *V20MbcDev::getRtc(void)
{
  return &rtc_;
}

DevSd *V20MbcDev::getSd(void)
{
  return &sd_;
}

DevTty *V20MbcDev::getTty(void)
{
  return &tty_;
}

DevUser *V20MbcDev::getUser(void)
{
  return &user_;
}

MbcDisk *V20MbcDev::getDisk(void)
{
  return &disk_;
}

V20MbcPin *V20MbcDev::getPin(void)
{
  return &pin_;
}
