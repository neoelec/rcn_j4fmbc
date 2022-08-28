#ifndef __INTERNAL__V20MBCMENU_H__
#define __INTERNAL__V20MBCMENU_H__

#include <j4fDev.h>
#include <SerialMenuCmd.h>

#include "j4fV20Mbc.h"

class V20MbcMenu
{
public:
  void begin(V20MbcDev &dev);
  SerialMenuCmd *getMenuCmd(void);
  bool isDone(void);
  void blinkLed(void);
  void doChangeBootMode(void);
  void doCmdListBootMode(void);
  void doCmdToggleAutoexecEn(void);
  void doCmdChangeClockMode(void);
  void doCmdPrintConfiguration(void);
  void doCmdAdjustRtc(void);
  void doCmdExit(void);

private:
  void __adjustRtc(stDateTimeGroup &dtg);

private:
  DevRtc *rtc_;
  DevSd *sd_;

  V20MbcCfg cfg_;
  SerialMenuCmd menu_cmd_;
  bool done_;
};

#endif // __INTERNAL__V20MBCMENU_H__
