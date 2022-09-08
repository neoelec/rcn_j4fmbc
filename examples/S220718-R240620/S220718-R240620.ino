// SPDX-License-Identifier: GPL-3.0-or-later

#include <j4fZ80Mbc2.h>

#include "inc/Z80Mbc2Spec.h"

void setup(void)
{
  Z80Mbc2Io.begin();
  Serial.println(F("IOS: ARCH/MCU - " Z80MBC2_ARCH));
}

void loop(void)
{
  Z80Mbc2Io.run();
}

void serialEvent(void)
{
  Z80Mbc2Io.serialEvent();
}
