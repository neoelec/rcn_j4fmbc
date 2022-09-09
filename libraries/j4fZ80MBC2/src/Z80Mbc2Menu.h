#ifndef __INTERNAL__Z80MBC2MENU_H__
#define __INTERNAL__Z80MBC2MENU_H__

#include <j4fDev.h>
#include <SerialMenuCmd.h>

#include "Z80Mbc2Cfg.h"

class Z80Mbc2MenuClass
{
public:
  void begin(void);
  void enter(void);
  bool run(void);
  bool isDone(void);
  void doCmdChangeBootMode(void);
  void doCmdListBootMode(void);
  void doCmdToggleAutoexecEn(void);
  void doCmdChangeClockMode(void);
  void doCmdPrintConfiguration(void);
  void doCmdAdjustRtc(void);
  void doCmdChangeBoardType(void);
  void doCmdExit(void);
  void doCmdHelp(void);

private:
  void __adjustRtc(stDateTimeGroup &dtg);
  void __blinkLed(void);
  void __initializeMenuCmd(void);

private:
  DevRtc *rtc_;
  DevSd *sd_;

  Z80Mbc2Cfg cfg_;
  SerialMenuCmd menu_cmd_;
  bool done_;
};

extern Z80Mbc2MenuClass Z80Mbc2Menu;

#endif // __INTERNAL__Z80MBC2MENU_H__
