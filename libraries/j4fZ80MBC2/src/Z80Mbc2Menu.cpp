// SPDX-License-Identifier: GPL-3.0-or-later

#include <avr/wdt.h>

#include "Z80Mbc2Menu.h"

#define PIN_LED_IOS 0 // PB0 pin 1    Led LED_IOS is ON if HIGH

void Z80Mbc2Menu::begin(Z80Mbc2Dev &dev)
{
  rtc_ = dev.getRtc();
  sd_ = dev.getSd();
  cfg_.begin(sd_);
  done_ = false;
}

bool Z80Mbc2Menu::isDone(void)
{
  return done_;
}

SerialMenuCmd *Z80Mbc2Menu::getMenuCmd(void)
{
  return &menu_cmd_;
}

void Z80Mbc2Menu::blinkLed(void)
{
  static unsigned long timestamp;

  if ((millis() - timestamp) > 200)
  {
    digitalWrite(PIN_LED_IOS, !digitalRead(PIN_LED_IOS));
    timestamp = millis();
  }
}

void Z80Mbc2Menu::doChangeBootMode(void)
{
  String str_bm;
  long boot_mode;
  uint8_t nr_boot_mode = cfg_.getNrBootMode();

  Serial.println();
  Serial.printf(F("IOS: BOOT MODE [0-%d]"), nr_boot_mode - 1);
  if (!menu_cmd_.getStrValue(str_bm) || !str_bm.length())
  {
    boot_mode = cfg_.getBootMode();
    goto __exit;
  }

  boot_mode = str_bm.toInt();

  if (boot_mode < 0 || boot_mode >= nr_boot_mode)
  {
    Serial.println();
    Serial.printf(F("Wrong Value %ld. It should be 0 <= boot_mode <= %ld"),
                  boot_mode, nr_boot_mode - 1);
    Serial.println();
    goto __no_change;
  }

__exit:
  cfg_.setBootMode(static_cast<uint8_t>(boot_mode & 0xFF));
  Serial.println();
  cfg_.printCfg();
  Serial.println();
__no_change:
  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2Menu::doCmdListBootMode(void)
{
  Z80Mbc2Cfg cfg_tmp;

  Serial.println();

  for (uint8_t i = 0; i < cfg_.getNrBootMode(); i++)
  {
    cfg_tmp.begin(sd_, i);
    Serial.printf(F("%2u - %s"), i, cfg_tmp.getDescription());
    Serial.println();
  }

  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2Menu::doCmdToggleAutoexecEn(void)
{
  Serial.println();
  cfg_.setAutoExecEn(!cfg_.getAutoExecEn());

  Serial.print(F("IOS: CP/M Autoexec is "));
  if (cfg_.getAutoExecEn())
    Serial.println(F("ON"));
  else
    Serial.println(F("OFF"));

  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2Menu::doCmdChangeClockMode(void)
{
  String str_idx;
  const long clock_mode_max = CLOCK_MODE_MAX;
  long clock_mode = cfg_.getClkMode();

  Serial.println();
  Serial.printf(F("IOS: CLK MODE [0-%ld]"), clock_mode_max);
  for (long i = 0; i <= clock_mode_max; i++)
  {
    Serial.println();
    Serial.printf(F("%2ld - "), i);
    cfg_.printClk(static_cast<uint8_t>(i & 0xFF));
  }

  if (!menu_cmd_.getStrValue(str_idx) || !str_idx.length())
    goto __exit;

  if (str_idx.toInt() > clock_mode_max)
    goto __exit;

  clock_mode = str_idx.toInt();

__exit:
  Serial.println();
  Serial.printf(F("IOS: C> "));
  cfg_.printClk(static_cast<uint8_t>(clock_mode & 0xFF));
  Serial.println();
  cfg_.setClkMode(static_cast<uint8_t>(clock_mode & 0xFF));
  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2Menu::doCmdAdjustRtc(void)
{
  String str_dtg;
  stDateTimeGroup dtg;

  if (!rtc_->isAvailable())
  {
    Serial.println();
    Serial.print(F("IOS: RTC is NOT available"));
    goto __no_change;
  }

  Serial.println();
  Serial.print(F("IOS: [YYYY-MM-DDThh:mm:ss]"));
  if (!menu_cmd_.getStrOfChar(str_dtg) || !str_dtg.length())
    goto __no_change;

  if (!menu_cmd_.ConvStrToDTg(str_dtg, dtg))
  {
    Serial.println();
    Serial.print(F("IOS: Malformed input"));
    goto __no_change;
  }

  __adjustRtc(dtg);

__no_change:
  Serial.println();
  rtc_->printDateTime();
  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2Menu::__adjustRtc(stDateTimeGroup &dtg)
{
  DateTime dt(dtg.u16Year, dtg.u8Month, dtg.u8Day, dtg.u8Hour, dtg.u8Min, dtg.u8Sec);

  rtc_->adjust(dt);
}

void Z80Mbc2Menu::doCmdExit(void)
{
  done_ = true;

  Serial.println();
  Serial.println(F("IOS: Reset ..."));
  wdt_enable(WDTO_15MS);
}
