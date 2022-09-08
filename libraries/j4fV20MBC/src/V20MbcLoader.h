#ifndef __INTERNAL__V20MBCLOADER_H__
#define __INTERNAL__V20MBCLOADER_H__

#include <j4fDev.h>
#include <j4fMbc.h>

#include "V20MbcCfg.h"
#include "V20MbcPin.h"

class V20MbcLoader
{
public:
  void begin(V20MbcDev &dev);
  void bootstrap(void);

private:
  void __loadCsvFile(void);
  void __loadFwFor86Mode(V20MbcCfg *cfg);
  void __loadFwFor80Mode(V20MbcCfg *cfg);
  void __setBootAddress(V20MbcCfg *cfg);
  void __warmReset(void);
  void __haltCpu(void);
  void __pulseClock(uint16_t n);
  void __openFwFile(const char *firmware_file);
  uint16_t __loadFwFile(V20MbcCfg *cfg);
  uint16_t ____loadFwFile(uint16_t offset, const char *firmware_file);
  void __feedOneByte(uint8_t data);
  void __loadDataSegment(uint16_t segment);
  void __storeOneByte(uint8_t data, uint16_t offset);
  void __storeOneWord(uint16_t data, uint16_t offset);

private:
  DevClock *clock_;
  DevSd *sd_;
  MbcDisk *disk_;
  V20MbcPin *pin_;

  V20MbcCfg *cfg_;
  V20MbcCfg *cfg80_;
};

#endif // __INTERNAL__V20MBCLOADER_H__
