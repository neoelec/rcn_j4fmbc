// SPDX-License-Identifier: GPL-3_.0-or-later

#include "j4fMbc.h"

MbcDevClass MbcDev;

DevClock *__MbcDevMcuVariant::getClock(void)
{
  return &clock_;
}

void MbcDevClass::begin(uint8_t pin_user, uint8_t pin_rst, uint8_t pin_cts)
{
  tty_.begin(pin_rst, pin_cts);

  clock_.begin();
  gpio_.begin();
  rtc_.begin();
  sd_.begin();
  user_.begin(pin_user);

  disk_.begin(&sd_);
}

DevGpio *MbcDevClass::getGpio(void)
{
  return &gpio_;
}

DevRtc *MbcDevClass::getRtc(void)
{
  return &rtc_;
}

DevSd *MbcDevClass::getSd(void)
{
  return &sd_;
}

DevTty *MbcDevClass::getTty(void)
{
  return &tty_;
}

DevUser *MbcDevClass::getUser(void)
{
  return &user_;
}

MbcDisk *MbcDevClass::getDisk(void)
{
  return &disk_;
}
