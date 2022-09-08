// SPDX-License-Identifier: GPL-3.0-or-later

#include "inc/V20MbcSpec.h"

#if defined(CONFIG_V20MBC_BOARD)
#include <j4fV20Mbc.h>
static MbcIo *mbc_io = &V20MbcIo;
#elif defined(CONFIG_Z80MBC2_BOARD)
#include <j4fZ80Mbc2.h>
static MbcIo *mbc_io = &Z80Mbc2Io;
#endif

void setup(void)
{
  mbc_io->begin();
  Serial.println(F("IOS: ARCH/MCU - " J4F_MBC_ARCH));
}

void loop(void)
{
  mbc_io->run();
}

void serialEvent(void)
{
  mbc_io->serialEvent();
}
