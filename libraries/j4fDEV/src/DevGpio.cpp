// SPDX-License-Identifier: GPL-3.0-or-later

#include "DevGpio.h"

void DevGpio::begin(void)
{
  Serial.print(F("IOS: MCP23017 GPIO Expander - "));

  if (!mcp_.begin_I2C())
  {
    available_ = false;
    Serial.println(F("Not Found."));
  }
  else
  {
    available_ = true;
    Serial.println(F("Found."));
  }
}

bool DevGpio::isAvailable(void)
{
  return available_;
}
