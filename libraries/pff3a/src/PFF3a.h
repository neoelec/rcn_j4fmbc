#ifndef __PFF3A_H__
#define __PFF3A_H__

#include "pff.h"

class PFF3aClass
{
public:
  FRESULT begin(FATFS *fs, BYTE pincs);
  FRESULT open(const char *path);
  FRESULT read(void *buff, UINT btr, UINT *br);
  FRESULT write(const void *buff, UINT btw, UINT *bw);
  FRESULT lseek(DWORD ofs);
  FRESULT opendir(DIR *dj, const char *path);
  FRESULT readdir(DIR *dj, FILINFO *fno);
};

extern PFF3aClass PFF3a;

#endif // __PFF3A_H__
