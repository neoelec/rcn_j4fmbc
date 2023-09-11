#ifndef __INTERNAL__Z80MBC2IRQSTATUS_H__
#define __INTERNAL__Z80MBC2IRQSTATUS_H__

#include <stdint.h>

union Z80Mbc2IrqStatus
{
  struct
  {
    uint8_t rx_ : 1;
    uint8_t sys_tick_ : 1;
    uint8_t reserved_0_ : 6;
  };
  uint8_t raw_;
};

#endif /* __INTERNAL__Z80MBC2IRQSTATUS_H__ */