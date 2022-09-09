#ifndef __INTERNAL__V20MBCCFG_H__
#define __INTERNAL__V20MBCCFG_H__

#include <j4fDev.h>
#include <stdint.h>

class V20MbcCfg
{
public:
  enum
  {
    CLOCK_MODE_MAX = 1
  };

  bool begin(DevSd *sd, const char *csv_name);
  bool begin(DevSd *sd, uint8_t boot_mode);
  bool begin(DevSd *sd);
  const char *getDescription(void);
  bool isMode80(void);
  const char *getBootFile(void);
  uint16_t getLoadSegment(void);
  uint16_t getLoadOffset(void);
  uint16_t getBootSegment(void);
  uint16_t getBootOffset(void);
  uint16_t getPtrBootOffset(void);
  uint16_t getPtrBootSize(void);
  int8_t getDiskSet(void);
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
  bool __openCsv(const char *csv_name);
  bool __readCsv(char *csv, const char *csv_name);
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
  uint8_t mode_80_;
  char boot_file_[13];
  uint16_t load_segment_;
  uint16_t load_offset_;
  uint16_t boot_segment_;
  uint16_t boot_offset_;
  uint16_t ptr_boot_offset_;
  uint16_t ptr_boot_size_;
  int8_t disk_set_;

  uint8_t boot_mode_;
  uint8_t autoexec_en_;
  uint8_t clock_mode_;
};

#endif // __INTERNAL__V20MBCCFG_H__
