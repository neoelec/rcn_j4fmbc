// SPDX-License-Identifier: GPL-3.0-or-later

#include <EEPROM.h>

#include "j4fZ80Mbc2.h"

#define SZ_BUF 32
#define SZ_CSV_BUF (SZ_BUF * 3)

#define BOOT_MODE_MAX 100

#define IDX_BOOT_MODE 10
#define IDX_AUTOEXEC_EN 12
#define IDX_CLOCK_MODE 13

#define CSV_NAME_FMT "BOOTMOxx.CSV"

bool Z80Mbc2Cfg::begin(DevSd *sd)
{
  sd_ = sd;
  __initNrBootMode();
  __initBootMode();
  __initAutoExecEn();
  __initClkMode();

  return __beginBootMode(boot_mode_);
}

bool Z80Mbc2Cfg::begin(DevSd *sd, uint8_t boot_mode)
{
  bool is_ok;

  sd_ = sd;
  is_ok = __beginBootMode(boot_mode);
  __initAutoExecEn();
  __initClkMode();

  return is_ok;
}

bool Z80Mbc2Cfg::begin(DevSd *sd, const char *csv_name)
{
  bool is_ok;

  sd_ = sd;
  is_ok = __beginCsvName(csv_name);
  __initAutoExecEn();
  __initClkMode();

  return is_ok;
}

bool Z80Mbc2Cfg::__beginBootMode(uint8_t boot_mode)
{
  char csv_name[] = CSV_NAME_FMT;

  csv_name[6] = boot_mode / 10 + '0';
  csv_name[7] = boot_mode % 10 + '0';

  return __beginCsvName(csv_name);
}

bool Z80Mbc2Cfg::__beginCsvName(const char *csv_name)
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

bool Z80Mbc2Cfg::__openCsv(const char *csv_name)
{
  uint8_t operation;
  uint8_t error;

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

bool Z80Mbc2Cfg::__readCsv(char *csv, const char *csv_name)
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

void Z80Mbc2Cfg::__parseCsv(char *csv)
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

void Z80Mbc2Cfg::__parseToken(char *token, uint8_t i)
{
  long tmp;

  switch (i)
  {
  case 0: // description_
    strncpy(description_, token, sizeof(description_));
    __terminateCStr(description_);
    break;
  case 1: // boot_file
    strncpy(boot_file_, token, sizeof(boot_file_));
    __terminateCStr(boot_file_);
    /* NOTE: PF library can only recognize uppercase characters. */
    for (size_t i = 0; i < sizeof(boot_file_) && !boot_file_[i]; i++)
      boot_file_[i] = toupper(boot_file_[i]);
    break;
  case 2: // load_address_
    tmp = strtol(token, NULL, 16);
    load_address_ = static_cast<uint16_t>(tmp);
    break;
  case 3: // boot_address_
    tmp = strtol(token, NULL, 16);
    boot_address_ = static_cast<uint16_t>(tmp);
    break;
  case 4: // disk_set_
    tmp = strtol(token, NULL, 10);
    disk_set_ = static_cast<int8_t>(tmp);
    break;
  case 5: // irq_tty_rx_
    tmp = strtol(token, NULL, 10);
    irq_tty_rx_ = static_cast<uint8_t>(tmp);
    break;
  }
}

const char *Z80Mbc2Cfg::getDescription(void)
{
  return description_;
}

const char *Z80Mbc2Cfg::getBootFile(void)
{
  return boot_file_;
}

uint16_t Z80Mbc2Cfg::getLoadAddress(void)
{
  return load_address_;
}

uint16_t Z80Mbc2Cfg::getBootAddress(void)
{
  return boot_address_;
}

int8_t Z80Mbc2Cfg::getDiskSet(void)
{
  return disk_set_;
}

uint8_t Z80Mbc2Cfg::getIrqTtyRx(void)
{
  return irq_tty_rx_;
}

uint8_t Z80Mbc2Cfg::getBootMode(void)
{
  return boot_mode_;
}

uint8_t Z80Mbc2Cfg::getBoardType(void)
{
  return MbcBoardType.getBoardType();
}

uint8_t Z80Mbc2Cfg::getAutoExecEn(void)
{
  return autoexec_en_;
}

uint8_t Z80Mbc2Cfg::getClkMode(void)
{
  return clock_mode_;
}

void Z80Mbc2Cfg::setBootMode(uint8_t boot_mode)
{
  boot_mode_ = boot_mode;

  __beginBootMode(boot_mode_);

  EEPROM.update(IDX_BOOT_MODE, boot_mode_);
}

void Z80Mbc2Cfg::setBoardType(uint8_t board_type)
{
  MbcBoardType.setBoardType(board_type);
}

void Z80Mbc2Cfg::setAutoExecEn(uint8_t autoexec_en)
{
  autoexec_en_ = autoexec_en;

  EEPROM.update(IDX_AUTOEXEC_EN, autoexec_en_);
}

void Z80Mbc2Cfg::setClkMode(uint8_t clock_mode)
{
  clock_mode_ = clock_mode;

  EEPROM.update(IDX_CLOCK_MODE, clock_mode_);
}

void Z80Mbc2Cfg::__initNrBootMode(void)
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

uint8_t Z80Mbc2Cfg::getNrBootMode(void)
{
  return nr_boot_mode_;
}

void Z80Mbc2Cfg::__initBootMode(void)
{
  uint8_t boot_mode = EEPROM.read(IDX_BOOT_MODE);

  if (boot_mode >= nr_boot_mode_)
  {
    boot_mode = 0;
    setBootMode(boot_mode);
  }

  boot_mode_ = boot_mode;
}

void Z80Mbc2Cfg::__initAutoExecEn(void)
{
  uint8_t autoexec_en = EEPROM.read(IDX_AUTOEXEC_EN);

  if (autoexec_en > 1)
  {
    autoexec_en = 0;
    setAutoExecEn(autoexec_en);
  }

  autoexec_en_ = autoexec_en;
}

void Z80Mbc2Cfg::__initClkMode(void)
{
  uint8_t clock_mode = EEPROM.read(IDX_CLOCK_MODE);

  if (clock_mode > CLOCK_MODE_MAX)
  {
    clock_mode = CLOCK_MODE_MAX;
    setClkMode(clock_mode);
  }

  clock_mode_ = clock_mode;
}

void Z80Mbc2Cfg::printCfg(void)
{
  Serial.printf(F("IOS: %s"), description_);
  Serial.println();
  Serial.printf(F("     F> %-12s"), boot_file_);
  Serial.printf(F(" L> %04X"), load_address_);
  Serial.printf(F(" B> %04X"), load_address_);
  Serial.print(F(" D> "));
  if (disk_set_ >= 0)
    Serial.printf(F("% 2d"), disk_set_);
  else
    Serial.print(F("NA"));
  Serial.printf(F(" T> %-3s"), irq_tty_rx_ ? "ON" : "OFF");
  Serial.printf(F(" A> %-3s"), autoexec_en_ ? "ON" : "OFF");
  Serial.print(F(" C> "));
  printClk(clock_mode_);
  Serial.println();
}

void Z80Mbc2Cfg::printClk(uint8_t clock_mode)
{
  unsigned long khz;

  khz = F_CPU / 1000UL / ((clock_mode + 1) << 1);
  khz += 50;

  Serial.printf(F("% 2lu.%lu MHz"), khz / 1000UL, (khz % 1000UL) / 100UL);
}
