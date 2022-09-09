#ifdef ARDUINO_AVR_ATmega32
#ifndef __ARDUINO_AVR_ATmega32_H__
#define __ARDUINO_AVR_ATmega32_H__

#define J4F_MBC_ARCH "ARDUINO_AVR_ATmega32"

#if defined(CONFIG_V20MBC_BOARD)
#include <j4fV20Mbc.h>
#elif defined(CONFIG_Z80MBC2_BOARD)
#include <j4fZ80Mbc2.h>
#endif

static inline MbcIo *j4fMbcSpecGetIo(void)
{
#if defined(CONFIG_V20MBC_BOARD)
  return &V20MbcIo;
#elif defined(CONFIG_Z80MBC2_BOARD)
  return &Z80Mbc2Io;
#else
  return nullptr;
#endif
}

#endif // __ARDUINO_AVR_ATmega32_H__
#endif // ARDUINO_AVR_ATmega32
