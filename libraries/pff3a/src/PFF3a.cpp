#pragma GCC optimize("O2")

#include <Arduino.h>

#include "diskIoArduino.h"
#include "PFF3a.h"

PFF3aClass PFF3a;

FRESULT PFF3aClass::begin(FATFS *fs, BYTE pincs)
{
  PinCS = pincs;

  pinMode(PinCS, OUTPUT);
  digitalWrite(PinCS, HIGH);

  return pf_mount(fs);
}

FRESULT PFF3aClass::open(const char *path)
{
  return pf_open(path);
}

FRESULT PFF3aClass::read(void *buff, UINT btr, UINT *br)
{
#if PF_USE_READ
  return pf_read(buff, btr, br);
#else
  return FR_NOT_ENABLED;
#endif
}

FRESULT PFF3aClass::write(const void *buff, UINT btw, UINT *bw)
{
#if PF_USE_WRITE
  return pf_write(buff, btw, bw);
#else
  return FR_NOT_ENABLED;
#endif
}

FRESULT PFF3aClass::lseek(DWORD ofs)
{
#if PF_USE_LSEEK
  return pf_lseek(ofs);
#else
  return FR_NOT_ENABLED;
#endif
}

FRESULT PFF3aClass::opendir(DIR *dj, const char *path)
{
#if PF_USE_DIR
  return pf_opendir(dj, path);
#else
  return FR_NOT_ENABLED;
#endif
}

FRESULT PFF3aClass::readdir(DIR *dj, FILINFO *fno)
{
#if PF_USE_DIR
  return pf_readdir(dj, fno);
#else
  return FR_NOT_ENABLED;
#endif
}
