// SPDX-License-Identifier: GPL-3.0-or-later

#include <Arduino.h>

#include "j4fDev.h"

void DevTty::begin(uint8_t pin_rts, uint8_t pin_cts)
{
  __resetUterm(pin_rts, pin_cts);

  Serial.begin(115200);

  if (SERIAL_RX_BUFFER_SIZE >= 128)
    Serial.println(F("IOS: Found extended serial Rx buffer"));
}

void DevTty::__resetUterm(uint8_t pin_rts, uint8_t pin_cts)
{
  pinMode(pin_cts, INPUT_PULLUP);
  pinMode(pin_rts, OUTPUT);
  digitalWrite(pin_rts, LOW);
  delay(100);
  digitalWrite(pin_rts, HIGH);
  delay(500);
}
