// SPDX-License-Identifier: GPL-3_.0-or-later

#include <EEPROM.h>

#include "j4fMbc.h"

#define IDX_BOARD_TYPE 11

MbcBoardTypeClass MbcBoardType;

void MbcBoardTypeClass::begin(void)
{
  uint8_t board_type = EEPROM.read(IDX_BOARD_TYPE);

  if (board_type >= NR_BOARD_TYPE)
  {
    board_type = 0;
    setBoardType(board_type);
  }

  board_type_ = board_type;
}

uint8_t MbcBoardTypeClass::getBoardType(void)
{
  return board_type_;
}

void MbcBoardTypeClass::setBoardType(uint8_t board_type)
{
  board_type_ = board_type;

  EEPROM.update(IDX_BOARD_TYPE, board_type_);
}

void MbcBoardTypeClass::printBoardType(uint8_t board_type)
{
  switch (board_type)
  {
  case TYPE_Z80_MBC2:
    Serial.print(F("Z80-MBC2"));
    break;
  case TYPE_V20_MBC:
    Serial.print(F("V20-MBC"));
    break;
  default:
    Serial.print(F("UNKNOWN"));
    break;
  }
}
