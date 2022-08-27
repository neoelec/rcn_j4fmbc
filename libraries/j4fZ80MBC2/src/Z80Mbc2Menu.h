#ifndef __INTERNAL__Z80MBC2MENU_H__
#define __INTERNAL__Z80MBC2MENU_H__

#include <j4fDev.h>
#include <SerialMenuCmd.h>

#include "Z80Mbc2Cfg.h"
#include "Z80Mbc2Dev.h"

class Z80Mbc2Menu
{
public:
  void begin(Z80Mbc2Dev &dev);
  SerialMenuCmd *getMenuCmd(void);
  bool isDone(void);
  void blinkLed(void);
  void doChangeBootMode(void);
  void doCmdListBootMode(void);
  void doCmdToggleAutoexecEn(void);
  void doCmdChangeClockMode(void);
  void doCmdAdjustRtc(void);
  void doCmdExit(void);

private:
  void __adjustRtc(stDateTimeGroup &dtg);

private:
  DevRtc *rtc_;
  DevSd *sd_;

  Z80Mbc2Cfg cfg_;
  SerialMenuCmd menu_cmd_;
  bool done_;
};

#endif // __INTERNAL__Z80MBC2MENU_H__
