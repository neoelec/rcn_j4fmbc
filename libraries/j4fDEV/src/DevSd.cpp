// SPDX-License-Identifier: GPL-3.0-or-later

#include "DevSd.h"

void DevSd::begin(void)
{
  FRESULT error;

  do
  {
    error = PF.begin(&fatfs_);
    printError(error, MOUNT);

    if (error != FR_OK)
      waitKey();
  } while (error != FR_OK);
}

uint8_t DevSd::mount(void)
{
  FRESULT error;

  error = PF.begin(&fatfs_);

  return static_cast<uint8_t>(error & 0xFF);
}

uint8_t DevSd::open(const char *file_name)
{
  FRESULT error;

  error = PF.open(file_name);

  return static_cast<uint8_t>(error & 0xFF);
}

void DevSd::printError(uint8_t error, uint8_t operation, const char *file_name)
{
  __printErrorCode(static_cast<FRESULT>(error));
  __printOperation(operation);

  if (file_name)
    Serial.printf(F(" - File : %s"), file_name);

  Serial.println();
}

void DevSd::__printErrorCode(FRESULT error)
{
  Serial.printf(F("IOS: SD error %u "), static_cast<unsigned int>(error));

  Serial.print(F("("));
  switch (error)
  {
  case FR_OK:
    Serial.print(F("OK"));
    break;
  case FR_DISK_ERR:
    Serial.print(F("DISK_ERR"));
    break;
  case FR_NOT_READY:
    Serial.print(F("NOT_READY"));
    break;
  case FR_NO_FILE:
    Serial.print(F("NO_FILE"));
    break;
  case FR_NOT_OPENED:
    Serial.print(F("NOT_OPENED"));
    break;
  case FR_NOT_ENABLED:
    Serial.print(F("NOT_ENABLED"));
    break;
  case FR_NO_FILESYSTEM:
    Serial.print(F("NO_FILESYSTEM"));
    break;
  default:
    Serial.print(F("UNKNOWN"));
    break;
  }
  Serial.print(F(")"));
}

void DevSd::__printOperation(uint8_t operation)
{
  Serial.print(F(" on "));

  switch (operation)
  {
  case MOUNT:
    Serial.print(F("MOUNT"));
    break;
  case OPEN:
    Serial.print(F("OPEN"));
    break;
  case READ:
    Serial.print(F("READ"));
    break;
  case WRITE:
    Serial.print(F("WRITE"));
    break;
  case SEEK:
    Serial.print(F("SEEK"));
    break;
  default:
    Serial.print(F("UNKNOWN"));
    break;
  }
}

void DevSd::waitKey(void)
{
  __flushTtyRx();

  Serial.println(F("IOS: Check SD and press a key to repeat"));
  Serial.println();

  __waitTtyRx();
  __flushTtyRx();
}

void DevSd::__flushTtyRx(void)
{
  while (Serial.available() > 0)
    Serial.read();
}

void DevSd::__waitTtyRx(void)
{
  while (Serial.available() < 1)
    ;
}