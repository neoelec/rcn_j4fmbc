#ifndef __INTERNAL__Z80MBC2CFG_H__
#define __INTERNAL__Z80MBC2CFG_H__

#include <j4fDev.h>
#include <stdint.h>

#define BOOT_MODE_MAX 100
#define CLOCK_MODE_MAX 2

class Z80Mbc2Cfg
{
public:
  bool begin(DevSd *sd, const char *cfg_name);
  bool begin(DevSd *sd, uint8_t boot_mode);
  bool begin(DevSd *sd);
  const char *getDescription(void);
  bool isMode80(void);
  const char *getBootFile(void);
  uint16_t getLoadAddress(void);
  uint16_t getBootAddress(void);
  int8_t getDiskSet(void);
  uint8_t getIrqTtyRx(void);
  uint8_t getBootMode(void);
  uint8_t getAutoExecEn(void);
  uint8_t getClkMode(void);
  uint8_t getNrBootMode(void);
  void setBootMode(uint8_t boot_mode);
  void setAutoExecEn(uint8_t autoexec_en);
  void setClkMode(uint8_t clock_mode);
  void printCfg(void);
  void printClk(uint8_t clock_mode);

private:
  bool __openCsv(const char *cfg_name);
  bool __readCsv(char *csv, const char *cfg_name);
  void __parseCsv(char *csv);
  void __parseToken(char *toeke, uint8_t i);
  void __initBootMode(void);
  void __initAutoExecEn(void);
  void __initClkMode(void);
  void __intNrBootMode(void);
  bool __begin_w_boot_mode(uint8_t boot_mode);
  bool __begin_w_cfg_name(const char *cfg_name);

private:
  DevSd *sd_;
  uint8_t nr_boot_mode_;

  char description_[24];
  char boot_file_[13];
  uint16_t load_address_;
  uint16_t boot_address_;
  int8_t disk_set_;
  uint8_t irq_tty_rx_;

  uint8_t boot_mode_;
  uint8_t autoexec_en_;
  uint8_t clock_mode_;
};

#endif // __INTERNAL__Z80MBC2CFG_H__
