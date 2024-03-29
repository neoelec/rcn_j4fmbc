#ifndef __INTERNAL__V20MBCMENU_H__
#define __INTERNAL__V20MBCMENU_H__

#include <j4fDev.h>
#include <SerialMenuCmd.h>

#include "V20MbcCfg.h"

class V20MbcMenuClass
{
public:
  void begin(void);
  void enter(void);
  bool run(void);
  bool isDone(void);
  void doChangeBootMode(void);
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
  void __blinkIosLed(void);
  void __turnOffUserLed(void);
  void __initializeMenuCmd(void);

private:
  DevRtc *rtc_;
  DevSd *sd_;

  V20MbcCfg cfg_;
  SerialMenuCmd menu_cmd_;
  bool done_;
};

extern V20MbcMenuClass V20MbcMenu;

#endif // __INTERNAL__V20MBCMENU_H__
