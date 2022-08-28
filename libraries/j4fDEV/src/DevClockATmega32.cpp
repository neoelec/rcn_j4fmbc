// SPDX-License-Identifier: GPL-3.0-or-later

#include <avr/io.h>

#include "j4fDev.h"

#if defined(ARDUINO_AVR_ATmega32)
void DevClockATmega32::begin(void)
{
  ASSR &= ~_BV(AS2);
  TCCR2 |= _BV(CS20);
  TCCR2 &= ~(_BV(CS21) | _BV(CS22));
  TCCR2 |= _BV(WGM21);
  TCCR2 &= ~_BV(WGM20);
}

void DevClockATmega32::enable(void)
{
  TCCR2 |= _BV(COM20);
  TCCR2 &= ~_BV(COM21);
}

void DevClockATmega32::setMode(uint8_t mode)
{
  OCR2 = mode;
}
#endif
