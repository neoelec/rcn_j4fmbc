#ifndef __ARDUINOGLUE_H__
#define __ARDUINOGLUE_H__

#include "diskio.h"

extern BYTE PinCS;

#ifdef __cplusplus
extern "C" {
#endif

DSTATUS glue_disk_initialize(void);
DRESULT glue_disk_readp(BYTE *buff, DWORD sector, UINT offser, UINT count);
DRESULT glue_disk_writep(const BYTE *buff, DWORD sc);

#ifdef __cplusplus
}
#endif

#endif // __ARDUINOGLUE_H__
