#ifndef __INTERNAL__Z80MBC2LOADER_H__
#define __INTERNAL__Z80MBC2LOADER_H__

#include <j4fDev.h>
#include <j4fMbc.h>

#include "j4fZ80Mbc2.h"

class Z80Mbc2Loader
{
public:
  void begin(Z80Mbc2Dev &dev);
  void bootstrap(void);

private:
  void __initClock(void);
  void __enableClock(void);
  void __loadCsvFile(void);
  void __loadFirmware(Z80Mbc2Cfg *cfg);
  void __setBootAddress(Z80Mbc2Cfg *cfg);
  void __warmReset(void);
  void __pulseClock(uint16_t n);
  void __openFwFile(const char *firmware_file);
  void __loadFwFile(Z80Mbc2Cfg *cfg);
  void ____loadFwFile(uint16_t offset, const char *firmware_file);
  void __loadByteToRam(byte value);
  void __loadHL(uint16_t data);

private:
  DevClock *clock_;
  DevSd *sd_;
  MbcDisk *disk_;
  Z80Mbc2Pin *pin_;

  Z80Mbc2Cfg *cfg_;
  Z80Mbc2Cfg *cfg80_;
};

#endif // __INTERNAL__Z80MBC2LOADER_H__
