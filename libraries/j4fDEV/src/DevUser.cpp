// SPDX-License-Identifier: GPL-3.0-or-later

#include <Arduino.h>

#include "DevUser.h"

void DevUser::begin(uint8_t pin_user)
{
  pin_user_ = pin_user;

  setLedOff();
}

void DevUser::setLedOn(void)
{
  pinMode(pin_user_, OUTPUT);
  digitalWrite(pin_user_, LOW);
}

void DevUser::setLedOff(void)
{
  pinMode(pin_user_, OUTPUT);
  digitalWrite(pin_user_, HIGH);
}

uint8_t DevUser::getKey(void)
{
  int led_value = digitalRead(pin_user_);
  int key_value;

  pinMode(pin_user_, INPUT_PULLUP);
  key_value = digitalRead(pin_user_);

  if (led_value == LOW)
    setLedOn();
  else
    setLedOff();

  return !key_value;
}
