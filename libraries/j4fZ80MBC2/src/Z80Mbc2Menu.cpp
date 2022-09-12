// SPDX-License-Identifier: GPL-3.0-or-later

#include <avr/wdt.h>

#include "j4fZ80Mbc2.h"

#define PIN_LED_IOS 0 // PB0 pin 1    Led LED_IOS is ON if HIGH

Z80Mbc2MenuClass Z80Mbc2Menu;

void Z80Mbc2MenuClass::begin(void)
{
  rtc_ = MbcDev.getRtc();
  sd_ = MbcDev.getSd();

  __initializeMenuCmd();
}

void Z80Mbc2MenuClass::enter(void)
{
  Serial.println();
  Serial.print(F("IOS: Entering to menu ..."));

  cfg_.begin(sd_);

  done_ = false;

  menu_cmd_.ShowMenu();
  menu_cmd_.giveCmdPrompt();
}

bool Z80Mbc2MenuClass::run(void)
{
  if (isDone())
    return false;

  uint8_t cmd = menu_cmd_.UserRequest();
  if (cmd)
    menu_cmd_.ExeCommand(cmd);

  return true;
}

bool Z80Mbc2MenuClass::isDone(void)
{
  return done_;
}

void Z80Mbc2MenuClass::doCmdChangeBootMode(void)
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
  cfg_.setBootMode(static_cast<uint8_t>(boot_mode));
  Serial.println();
  cfg_.printCfg();
  Serial.println();
__no_change:
  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2MenuClass::doCmdListBootMode(void)
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

void Z80Mbc2MenuClass::doCmdToggleAutoexecEn(void)
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

void Z80Mbc2MenuClass::doCmdChangeClockMode(void)
{
  String str_idx;
  const long clock_mode_max = Z80Mbc2Cfg::CLOCK_MODE_MAX;
  long clock_mode = cfg_.getClkMode();

  Serial.println();
  Serial.printf(F("IOS: CLK MODE [0-%ld]"), clock_mode_max);
  for (long i = 0; i <= clock_mode_max; i++)
  {
    Serial.println();
    Serial.printf(F("%2ld - "), i);
    cfg_.printClk(static_cast<uint8_t>(i));
  }

  if (!menu_cmd_.getStrValue(str_idx) || !str_idx.length())
    goto __exit;

  if (str_idx.toInt() > clock_mode_max)
    goto __exit;

  clock_mode = str_idx.toInt();

__exit:
  Serial.println();
  Serial.printf(F("IOS: C> "));
  cfg_.printClk(static_cast<uint8_t>(clock_mode));
  Serial.println();
  cfg_.setClkMode(static_cast<uint8_t>(clock_mode));
  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2MenuClass::doCmdAdjustRtc(void)
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

void Z80Mbc2MenuClass::doCmdPrintConfiguration(void)
{
  Serial.println();
  cfg_.printCfg();
  Serial.println();
  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2MenuClass::doCmdChangeBoardType(void)
{
  String str_bm;
  long board_type;
  uint8_t nr_board_type = MbcBoardTypeClass::NR_BOARD_TYPE;

  Serial.println();
  Serial.printf(F("IOS: BOARD TYPE [0-%d]"), nr_board_type - 1);
  for (long i = 0; i < nr_board_type; i++)
  {
    Serial.println();
    Serial.printf(F("%2ld - "), i);
    MbcBoardType.printBoardType(static_cast<uint8_t>(i));
  }

  if (!menu_cmd_.getStrValue(str_bm) || !str_bm.length())
  {
    board_type = MbcBoardType.getBoardType();
    goto __exit;
  }

  board_type = str_bm.toInt();

  if (board_type < 0 || board_type >= nr_board_type)
  {
    Serial.println();
    Serial.printf(F("Wrong Value %ld. It should be 0 <= board_type <= %ld"),
                  board_type, nr_board_type - 1);
    Serial.println();
    goto __no_change;
  }

__exit:
  MbcBoardType.setBoardType(static_cast<uint8_t>(board_type));
  Serial.println();
  MbcBoardType.printBoardType(static_cast<uint8_t>(board_type));
  Serial.println();
__no_change:
  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2MenuClass::doCmdExit(void)
{
  done_ = true;

  Serial.println();
  Serial.println(F("IOS: Reset ..."));
  wdt_enable(WDTO_15MS);
}

void Z80Mbc2MenuClass::doCmdHelp(void)
{
  menu_cmd_.ShowMenu();
  menu_cmd_.giveCmdPrompt();
}

void Z80Mbc2MenuClass::__adjustRtc(stDateTimeGroup &dtg)
{
  DateTime dt(dtg.u16Year, dtg.u8Month, dtg.u8Day, dtg.u8Hour, dtg.u8Min, dtg.u8Sec);

  rtc_->adjust(dt);
}

void Z80Mbc2MenuClass::__blinkLed(void)
{
  static unsigned long timestamp;

  if ((millis() - timestamp) > 200)
  {
    digitalWrite(PIN_LED_IOS, !digitalRead(PIN_LED_IOS));
    timestamp = millis();
  }
}

void Z80Mbc2MenuClass::__initializeMenuCmd(void)
{
  static tMenuCmdTxt prompt[] PROGMEM = "";
  static tMenuCmdTxt txt_p[] PROGMEM = "p - Print Configuration";
  static tMenuCmdTxt txt_b[] PROGMEM = "b - Change Boot Mode";
  static tMenuCmdTxt txt_l[] PROGMEM = "l - List Boot Mode";
  static tMenuCmdTxt txt_a[] PROGMEM = "a - Toggle AUTOEXEC";
  static tMenuCmdTxt txt_c[] PROGMEM = "c - Select CLK Mode";
  static tMenuCmdTxt txt_t[] PROGMEM = "t - Adjust RTC";
  static tMenuCmdTxt txt_y[] PROGMEM = "y - Change Board Type";
  static tMenuCmdTxt txt_x[] PROGMEM = "x - Exit";
  static tMenuCmdTxt txt__[] PROGMEM = "? - Help";
  static stMenuCmd menu_list[] = {
      {txt_p, 'p', []()
       { Z80Mbc2Menu.doCmdPrintConfiguration(); }},
      {txt_b, 'b', []()
       { Z80Mbc2Menu.doCmdChangeBootMode(); }},
      {txt_l, 'l', []()
       { Z80Mbc2Menu.doCmdListBootMode(); }},
      {txt_a, 'a', []()
       { Z80Mbc2Menu.doCmdToggleAutoexecEn(); }},
      {txt_c, 'c', []()
       { Z80Mbc2Menu.doCmdChangeClockMode(); }},
      {txt_t, 't', []()
       { Z80Mbc2Menu.doCmdAdjustRtc(); }},
#if defined(ARDUINO_AVR_ATmega1284)
      {txt_y, 'y', []()
       { Z80Mbc2Menu.doCmdChangeBoardType(); }},
#endif
      {txt_x, 'x', []()
       { Z80Mbc2Menu.doCmdExit(); }},
      {txt__, '?', []()
       { Z80Mbc2Menu.doCmdHelp(); }}};

  if (!menu_cmd_.begin(menu_list, ARRAY_SIZE(menu_list), prompt))
  {
    Serial.println(F("IOS: MENU Failed"));
    while (1)
      ;
  }
}
