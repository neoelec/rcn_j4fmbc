// SPDX-License-Identifier: GPL-3.0-or-later

#include <j4fV20Mbc.h>

#include "inc/V20MbcSpec.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

static V20MbcDev dev;
static V20MbcIo io;
static V20MbcMenu menu;
static V20MbcLoader loader;

static SerialMenuCmd *menu_cmd;

static inline void stateInitializing(void)
{
  dev.begin();

  Serial.println(F("IOS: ARCH/MCU - " V20MBC_ARCH));
}

static inline void stateConfiguring(void)
{
  DevUser *user = dev.getUser();

  if (!user->getKey())
    return;

  menu.begin(dev);

  menu_cmd = menu.getMenuCmd();
  __menuSetup();
  __menuRun();
}

static void __menuSetup(void)
{
  static tMenuCmdTxt prompt[] PROGMEM = "";
  static tMenuCmdTxt txt_b[] PROGMEM = "b - Change Boot Mode";
  static tMenuCmdTxt txt_l[] PROGMEM = "l - List Boot Mode";
  static tMenuCmdTxt txt_a[] PROGMEM = "a - Toggle AUTOEXEC";
  static tMenuCmdTxt txt_c[] PROGMEM = "c - Select CLK Mode";
  static tMenuCmdTxt txt_t[] PROGMEM = "t - Adjust RTC";
  static tMenuCmdTxt txt_x[] PROGMEM = "x - Exit";
  static tMenuCmdTxt txt__[] PROGMEM = "? - Help";
  static stMenuCmd menu_list[] = {
      {txt_b, 'b', []()
       { menu.doChangeBootMode(); }},
      {txt_l, 'l', []()
       { menu.doCmdListBootMode(); }},
      {txt_a, 'a', []()
       { menu.doCmdToggleAutoexecEn(); }},
      {txt_c, 'c', []()
       { menu.doCmdChangeClockMode(); }},
      {txt_t, 't', []()
       { menu.doCmdAdjustRtc(); }},
      {txt_x, 'x', []()
       { menu.doCmdExit(); }},
      {txt__, '?', []()
       { menu_cmd->ShowMenu(); menu_cmd->giveCmdPrompt(); }}};

  if (!menu_cmd->begin(menu_list, ARRAY_SIZE(menu_list), prompt))
  {
    Serial.println(F("IOS: MENU Failed"));
    while (1)
      ;
  }

  menu_cmd->ShowMenu();
  menu_cmd->giveCmdPrompt();
}

static void __menuRun(void)
{
  while (!menu.isDone()) {
    uint8_t cmd = menu_cmd->UserRequest();

    menu.blinkLed();
    if (cmd)
      menu_cmd->ExeCommand(cmd);
  }
}

static inline void statePreparing(void)
{
  io.begin(dev);
  loader.begin(dev);
}

static inline void stateBootstrapping(void)
{
  loader.bootstrap();
}

static inline void stateRunning(void)
{
  io.run();
}

void setup(void)
{
  stateInitializing();
  stateConfiguring();
  statePreparing();
  stateBootstrapping();
}

void loop(void)
{
  stateRunning();
}
