// SPDX-License-Identifier: GPL-3.0-or-later

#include "inc/V20MbcSpec.h"

static MbcIo *mbc_io;

void setup(void)
{
  mbc_io = j4fMbcSpecGetIo();
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
