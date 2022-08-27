// SPDX-License-Identifier: GPL-3.0-or-later

#include <avr/io.h>

#include "DevClockATmega1284.h"

#if defined(ARDUINO_AVR_ATmega1284)
void DevClockATmega1284::begin(void)
{
  ASSR &= ~_BV(AS2);
  TCCR2B |= _BV(CS20);
  TCCR2B &= ~(_BV(CS21) | _BV(CS22));
  TCCR2A |= _BV(WGM21);
  TCCR2A &= ~_BV(WGM20);
}

void DevClockATmega1284::enable(void)
{
  TCCR2A |= _BV(COM2A0);
  TCCR2A &= ~_BV(COM2A1);
}

void DevClockATmega1284::setMode(uint8_t mode)
{
  OCR2A = mode;
}
#endif
