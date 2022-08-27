// SPDX-License-Identifier: GPL-3.0-or-later

#include "DevRtc.h"

void DevRtc::begin(void)
{
  compiled_date_time_ = DateTime(__DATE__, __TIME__);

  Serial.print(F("IOS: Compiled Date/Time - "));
  __printDateTime(compiled_date_time_);

  Serial.print(F("IOS: DS3231 Date/Time   - "));
  if (!ds3231_.begin())
    __beginOnUnavailable();
  else
    __beginOnAvailable();
}

void DevRtc::__beginOnAvailable(void)
{
  available_ = true;

  if (ds3231_.lostPower())
  {
    Serial.println(F("Failure! Use compild!"));
    adjust(compiled_date_time_);
  }

  refresh();
  __printDateTime(date_time_);

  Serial.print(F("IOS: DS3231 Temperature - "));
  __printTemperature();
}

void DevRtc::__beginOnUnavailable(void)
{
  date_time_ = compiled_date_time_;
  available_ = false;

  Serial.println(F("Not Found."));
}

void DevRtc::__printDateTime(DateTime &date_time)
{
  Serial.printf(F("%02u/%02u/%04u %02u:%02u:%02u"),
                date_time.day(), date_time.month(), date_time.year(),
                date_time.hour(), date_time.minute(), date_time.second());
  Serial.println();
}

void DevRtc::__printTemperature(void)
{
  int celsius_10 = static_cast<int>(ds3231_.getTemperature() * 10.0);

  Serial.printf(F("%d.%d C"), celsius_10 / 10, celsius_10 % 10);
  Serial.println();
}

void DevRtc::adjust(DateTime &date_time)
{
  if (available_)
    ds3231_.adjust(date_time);
}

void DevRtc::refresh(void)
{
  if (available_)
    date_time_ = ds3231_.now();
}

uint8_t DevRtc::getSecond(void)
{
  return date_time_.second();
}

uint8_t DevRtc::getMinute(void)
{
  return date_time_.minute();
}

uint8_t DevRtc::getHour(void)
{
  return date_time_.hour();
}

uint8_t DevRtc::getDay(void)
{
  return date_time_.day();
}

uint8_t DevRtc::getMonth(void)
{
  return date_time_.month();
}

uint8_t DevRtc::getYear(void)
{
  return static_cast<uint8_t>(date_time_.year() - 2000);
}

uint8_t DevRtc::getTemperature(void)
{
  return static_cast<uint8_t>(ds3231_.getTemperature());
}

bool DevRtc::isAvailable(void)
{
  return available_;
}

void DevRtc::printDateTime(void)
{
  __printDateTime(date_time_);
}