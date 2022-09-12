#ifndef __INTERNAL__DEVTTY_H__
#define __INTERNAL__DEVTTY_H__

class DevTty
{
public:
  void begin(uint8_t pin_rts, uint8_t pin_cts);
  static void waitKey(void);

private:
  void __resetUterm(uint8_t pin_rts, uint8_t pin_cts);
};

#endif // __INTERNAL__DEVTTY_H__
