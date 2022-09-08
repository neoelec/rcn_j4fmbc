// SPDX-License-Identifier: GPL-3.0-or-later

#include <j4fV20Mbc.h>

#include "inc/V20MbcSpec.h"

void setup(void)
{
  V20MbcIo.begin();
  Serial.println(F("IOS: ARCH/MCU - " V20MBC_ARCH));
}

void loop(void)
{
  V20MbcIo.run();
}
