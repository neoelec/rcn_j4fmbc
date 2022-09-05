// SPDX-License-Identifier: GPL-3.0-or-later

#include <j4fZ80Mbc2.h>

#include "inc/Z80Mbc2Spec.h"

static Z80Mbc2Dev dev;
static Z80Mbc2Io io;
static Z80Mbc2Loader loader;

static Z80Mbc2Pin *pin;
static bool irq_tty_rx;

static inline void stateInitializing(void)
{
  dev.begin();
  pin = dev.getPin();

  Serial.println(F("IOS: ARCH/MCU - " Z80MBC2_ARCH));
}

static inline void stateConfiguring(void)
{
  DevUser *user = dev.getUser();

  if (!user->getKey())
    return;

  Z80Mbc2Menu.begin(dev);
  Z80Mbc2Menu.enter();
  Z80Mbc2Menu.run();
}

static inline void statePreparing(void)
{
  Z80Mbc2Cfg cfg;

  io.begin(dev);
  loader.begin(dev);

  cfg.begin(dev.getSd());
  irq_tty_rx = !!cfg.getIrqTtyRx();
  if (irq_tty_rx)
    Serial.println(F("IOS: IRQ-TTY-RX flag is enabled."));
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

void serialEvent(void)
{
  if ((Serial.available()) && irq_tty_rx)
    pin->setPIN_nINT_LOW();
}
