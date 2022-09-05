// SPDX-License-Identifier: GPL-3.0-or-later

#include <j4fV20Mbc.h>

#include "inc/V20MbcSpec.h"

static V20MbcDev dev;
static V20MbcIo io;
static V20MbcLoader loader;

static inline void stateInitializing(void)
{
  dev.begin();

  Serial.println(F("IOS: ARCH/MCU - " V20MBC_ARCH));
}

static inline void stateConfiguring(void)
{
  DevUser *user = dev.getUser();

  if (!user->getKey())
    return;

  V20MbcMenu.begin(dev);
  V20MbcMenu.enter();
  V20MbcMenu.run();
}

static inline void statePreparing(void)
{
  io.begin(dev);
  loader.begin(dev);
}

static inline void stateBootstrapping(void)
{
  loader.bootstrap();
}

static inline void stateRunning(void)
{
  io.run();
}

void setup(void)
{
  stateInitializing();
  stateConfiguring();
  statePreparing();
  stateBootstrapping();
}

void loop(void)
{
  stateRunning();
}
