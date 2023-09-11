#ifndef __INTERNAL__Z80MBC2CFG_H__
#define __INTERNAL__Z80MBC2CFG_H__

#include <j4fDev.h>
#include <stdint.h>

class Z80Mbc2Cfg
{
public:
  enum
  {
    CLOCK_MODE_MAX = 2
  };

  bool begin(DevSd *sd, const char *csv_name);
  bool begin(DevSd *sd, uint8_t boot_mode);
  bool begin(DevSd *sd);
  const char *getDescription(void);
  bool isMode80(void);
  const char *getBootFile(void);
  uint16_t getLoadAddress(void);
  uint16_t getBootAddress(void);
  int8_t getDiskSet(void);
  uint8_t getIrqTtyRx(void);
  uint8_t getIrqSysTick(void);
  uint8_t getBootMode(void);
  uint8_t getBoardType(void);
  uint8_t getAutoExecEn(void);
  uint8_t getClkMode(void);
  uint8_t getNrBootMode(void);
  void setBootMode(uint8_t boot_mode);
  void setBoardType(uint8_t board_type);
  void setAutoExecEn(uint8_t autoexec_en);
  void setClkMode(uint8_t clock_mode);
  void printCfg(void);
  void printClk(uint8_t clock_mode);

private:
  bool __openCsv(const char *csv_name);
  bool __readCsv(char *csv, uint8_t sz_csv, const char *csv_name);
  void __parseCsv(char *csv);
  void __parseToken(char *toeke, uint8_t i);
  void __initBootMode(void);
  void __initAutoExecEn(void);
  void __initClkMode(void);
  void __initNrBootMode(void);
  bool __beginBootMode(uint8_t boot_mode);
  bool __beginCsvName(const char *csv_name);

private:
  DevSd *sd_;
  uint8_t nr_boot_mode_;

  char description_[24];
  char boot_file_[13];
  uint16_t load_address_;
  uint16_t boot_address_;
  int8_t disk_set_;
  uint8_t irq_tty_rx_;
  uint8_t irq_sys_tick_;

  uint8_t boot_mode_;
  uint8_t autoexec_en_;
  uint8_t clock_mode_;
};

#endif // __INTERNAL__Z80MBC2CFG_H__
