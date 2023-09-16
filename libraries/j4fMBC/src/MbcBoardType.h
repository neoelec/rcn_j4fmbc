#ifndef __INTERNAL__MBCBOARDTYPE_H__
#define __INTERNAL__MBCBOARDTYPE_H__

#include <stdint.h>

class MbcBoardTypeClass {
public:
  enum {
    TYPE_Z80_MBC2,
    TYPE_V20_MBC,
    NR_BOARD_TYPE,
  };
  void begin(void);
  uint8_t getBoardType(void);
  void setBoardType(uint8_t board_type);
  void printBoardType(uint8_t board_type);

private:
  uint8_t board_type_;
};

extern MbcBoardTypeClass MbcBoardType;

#endif // __INTERNAL__MBCBOARDTYPE_H__
