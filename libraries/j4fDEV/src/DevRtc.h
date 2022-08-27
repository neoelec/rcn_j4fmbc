#ifndef __INTERNAL__DEVRTC_H__
#define __INTERNAL__DEVRTC_H__

#include <RTClib.h>

class DevRtc
{
public:
  void begin(void);
  void adjust(DateTime &date_time);
  void refresh(void);
  uint8_t getSecond(void);
  uint8_t getMinute(void);
  uint8_t getHour(void);
  uint8_t getDay(void);
  uint8_t getMonth(void);
  uint8_t getYear(void);
  uint8_t getTemperature(void);
  bool isAvailable(void);
  void printDateTime(void);

private:
  void __beginOnAvailable(void);
  void __beginOnUnavailable(void);
  void __printDateTime(DateTime &date_time);
  void __printTemperature(void);

  RTC_DS3231 ds3231_;
  DateTime date_time_;
  DateTime compiled_date_time_;
  bool available_;
};

#endif // __INTERNAL__DEVRTC_H__
