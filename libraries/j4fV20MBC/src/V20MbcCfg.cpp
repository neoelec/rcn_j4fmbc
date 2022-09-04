// SPDX-License-Identifier: GPL-3.0-or-later

#include <EEPROM.h>

#include "j4fV20Mbc.h"

#define SZ_BUF 32
#define SZ_CSV_BUF (SZ_BUF * 3)

#define IDX_BOOT_MODE 10
#define IDX_AUTOEXEC_EN 12
#define IDX_CLOCK_MODE 13

#define CSV_NAME_FMT "BOOTMOxx.CSV"

bool V20MbcCfg::begin(DevSd *sd)
{
  sd_ = sd;
  __initNrBootMode();
  __initBootMode();
  __initAutoExecEn();
  __initClkMode();

  return __beginBootMode(boot_mode_);
}

bool V20MbcCfg::begin(DevSd *sd, uint8_t boot_mode)
{
  bool is_ok;

  sd_ = sd;
  is_ok = __beginBootMode(boot_mode);
  __initAutoExecEn();
  __initClkMode();

  return is_ok;
}

bool V20MbcCfg::begin(DevSd *sd, const char *csv_name)
{
  bool is_ok;

  sd_ = sd;
  is_ok = __beginCsvName(csv_name);
  __initAutoExecEn();
  __initClkMode();

  return is_ok;
}

bool V20MbcCfg::__beginBootMode(uint8_t boot_mode)
{
  char csv_name[] = CSV_NAME_FMT;

  csv_name[6] = boot_mode / 10 + '0';
  csv_name[7] = boot_mode % 10 + '0';

  return __beginCsvName(csv_name);
}

bool V20MbcCfg::__beginCsvName(const char *csv_name)
{
  char csv[SZ_CSV_BUF];
  bool is_ok = __openCsv(csv_name);

  if (!is_ok)
    return is_ok;

  is_ok = __readCsv(csv, csv_name);
  if (is_ok)
    __parseCsv(csv);

  return is_ok;
}

bool V20MbcCfg::__openCsv(const char *csv_name)
{
  uint8_t error;
  uint8_t operation;

  operation = DevSd::OPEN;
  error = sd_->open(csv_name);
  if (error == FR_NO_FILE)
    return false;

  if (error == FR_OK)
    return true;

  sd_->printError(error, operation, csv_name);
  while (1)
    ;

  return false;
}

bool V20MbcCfg::__readCsv(char *csv, const char *csv_name)
{
  char buf[SZ_BUF];
  uint8_t error;
  uint8_t operation;
  uint8_t sz_read;
  size_t pos = 0;

  operation = DevSd::READ;
  do
  {
    error = sd_->read(buf, sizeof(buf), sz_read);
    if (error != FR_OK)
      goto __hang_on_error;

    memcpy(&csv[pos], buf, sz_read);
    pos += sz_read;
  } while (sz_read == sizeof(buf));

  return true;

__hang_on_error:
  sd_->printError(error, operation, csv_name);
  while (1)
    ;

  return false;
}

void V20MbcCfg::__parseCsv(char *csv)
{
  static const char *delim = ",";
  char *ptr = csv;
  char *token;
  unsigned int i = 0;

  token = strsep(&ptr, delim);
  while (token)
  {
    __parseToken(token, i++);
    token = strsep(&ptr, delim);
  }
}

#define __terminateCStr(__str) __str[sizeof(__str) - 1] = '\0'

void V20MbcCfg::__parseToken(char *token, uint8_t i)
{
  long tmp;

  switch (i)
  {
  case 0: // description_
    strncpy(description_, token, sizeof(description_));
    __terminateCStr(description_);
    break;
  case 1: // mode_80_
    tmp = strtol(token, NULL, 10);
    mode_80_ = static_cast<int8_t>(tmp & 0xFF);
    break;
  case 2: // boot_file
    strncpy(boot_file_, token, sizeof(boot_file_));
    __terminateCStr(boot_file_);
    /* NOTE: PF library can only recognize uppercase characters. */
    for (size_t i = 0; i < sizeof(boot_file_) && !boot_file_[i]; i++)
      boot_file_[i] = toupper(boot_file_[i]);
    break;
  case 3: // load_segment_
    tmp = strtol(token, NULL, 16);
    load_segment_ = static_cast<uint16_t>(tmp & 0xFFFF);
    break;
  case 4: // load_offset_
    tmp = strtol(token, NULL, 16);
    load_offset_ = static_cast<uint16_t>(tmp & 0xFFFF);
    break;
  case 5: // boot_segment_
    tmp = strtol(token, NULL, 16);
    boot_segment_ = static_cast<uint16_t>(tmp & 0xFFFF);
    break;
  case 6: // boot_offset_
    tmp = strtol(token, NULL, 16);
    boot_offset_ = static_cast<uint16_t>(tmp & 0xFFFF);
    break;
  case 7: // ptr_boot_offset_
    tmp = strtol(token, NULL, 16);
    ptr_boot_offset_ = static_cast<uint16_t>(tmp & 0xFFFF);
    break;
  case 8: // ptr_boot_size_
    tmp = strtol(token, NULL, 16);
    ptr_boot_size_ = static_cast<uint16_t>(tmp & 0xFFFF);
    break;
  case 9: // disk_set_
    tmp = strtol(token, NULL, 10);
    disk_set_ = static_cast<int8_t>(tmp & 0xFF);
    break;
  }
}

const char *V20MbcCfg::getDescription(void)
{
  return description_;
}

bool V20MbcCfg::isMode80(void)
{
  return !!mode_80_;
}

const char *V20MbcCfg::getBootFile(void)
{
  return boot_file_;
}

uint16_t V20MbcCfg::getLoadSegment(void)
{
  return load_segment_;
}

uint16_t V20MbcCfg::getLoadOffset(void)
{
  return load_offset_;
}

uint16_t V20MbcCfg::getBootSegment(void)
{
  return boot_segment_;
}

uint16_t V20MbcCfg::getBootOffset(void)
{
  return boot_offset_;
}

uint16_t V20MbcCfg::getPtrBootOffset(void)
{
  return ptr_boot_offset_;
}

uint16_t V20MbcCfg::getPtrBootSize(void)
{
  return ptr_boot_size_;
}

int8_t V20MbcCfg::getDiskSet(void)
{
  return disk_set_;
}

uint8_t V20MbcCfg::getBootMode(void)
{
  return boot_mode_;
}

uint8_t V20MbcCfg::getAutoExecEn(void)
{
  return autoexec_en_;
}

uint8_t V20MbcCfg::getClkMode(void)
{
  return clock_mode_;
}

void V20MbcCfg::setBootMode(uint8_t boot_mode)
{
  boot_mode_ = boot_mode;

  __beginBootMode(boot_mode_);

  EEPROM.update(IDX_BOOT_MODE, boot_mode_);
}

void V20MbcCfg::setAutoExecEn(uint8_t autoexec_en)
{
  autoexec_en_ = autoexec_en;

  EEPROM.update(IDX_AUTOEXEC_EN, autoexec_en_);
}

void V20MbcCfg::setClkMode(uint8_t clock_mode)
{
  clock_mode_ = clock_mode;

  EEPROM.update(IDX_CLOCK_MODE, clock_mode_);
}

void V20MbcCfg::__initNrBootMode(void)
{
  char csv_name[] = CSV_NAME_FMT;

  nr_boot_mode_ = 0;

  for (uint8_t boot_mode = 0; boot_mode < BOOT_MODE_MAX; boot_mode++)
  {
    csv_name[6] = boot_mode / 10 + '0';
    csv_name[7] = boot_mode % 10 + '0';

    if (!__openCsv(csv_name))
      break;

    nr_boot_mode_++;
  }
}

uint8_t V20MbcCfg::getNrBootMode(void)
{
  return nr_boot_mode_;
}

void V20MbcCfg::__initBootMode(void)
{
  uint8_t boot_mode = EEPROM.read(IDX_BOOT_MODE);

  if (boot_mode >= nr_boot_mode_)
  {
    boot_mode = 0;
    setBootMode(boot_mode);
  }

  boot_mode_ = boot_mode;
}

void V20MbcCfg::__initAutoExecEn(void)
{
  uint8_t autoexec_en = EEPROM.read(IDX_AUTOEXEC_EN);

  if (autoexec_en > 1)
  {
    autoexec_en = 0;
    setAutoExecEn(autoexec_en);
  }

  autoexec_en_ = autoexec_en;
}

void V20MbcCfg::__initClkMode(void)
{
  uint8_t clock_mode = EEPROM.read(IDX_CLOCK_MODE);

  if (clock_mode > CLOCK_MODE_MAX)
  {
    clock_mode = CLOCK_MODE_MAX;
    setClkMode(clock_mode);
  }

  clock_mode_ = clock_mode;
}

void V20MbcCfg::printCfg(void)
{
  Serial.printf(F("IOS: %s"), description_);
  Serial.println();
  Serial.printf(F("     F> %-12s"), boot_file_);
  Serial.printf(F(" M> %u"), mode_80_ ? 80 : 86);
  Serial.printf(F(" L> %04X:%04X"), load_segment_, load_offset_);
  Serial.printf(F(" B> %04X:%04X"), boot_segment_, load_offset_);
  Serial.print(F(" D> "));
  if (disk_set_ >= 0)
    Serial.printf(F("% 2d"), disk_set_);
  else
    Serial.print(F("NA"));
  Serial.printf(F(" A> %-3s"), autoexec_en_ ? "ON" : "OFF");
  Serial.print(F(" C> "));
  printClk(clock_mode_);
  Serial.println();
}

void V20MbcCfg::printClk(uint8_t clock_mode)
{
  unsigned long khz;

  khz = F_CPU / 1000UL / ((clock_mode + 1) << 1);
  khz += 50;

  Serial.printf(F("% 2lu.%lu MHz"), khz / 1000UL, (khz % 1000UL) / 100UL);
}
