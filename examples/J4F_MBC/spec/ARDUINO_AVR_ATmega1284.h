#ifdef ARDUINO_AVR_ATmega1284
#ifndef __ARDUINO_AVR_ATmega1284_H__
#define __ARDUINO_AVR_ATmega1284_H__

#define J4F_MBC_ARCH "ARDUINO_AVR_ATmega1284"

#include <j4fV20Mbc.h>
#include <j4fZ80Mbc2.h>
#include <j4fMbc.h>

static inline MbcIo *j4fMbcSpecGetIo(void)
{
  uint8_t board_type;

  MbcBoardType.begin();
  board_type = MbcBoardType.getBoardType();

  switch (board_type)
  {
  case MbcBoardTypeClass::TYPE_Z80_MBC2:
    return &Z80Mbc2Io;
  case MbcBoardTypeClass::TYPE_V20_MBC:
    return &V20MbcIo;
  }

  return nullptr;
}

#endif // __ARDUINO_AVR_ATmega1284_H__
#endif // ARDUINO_AVR_ATmega1284
