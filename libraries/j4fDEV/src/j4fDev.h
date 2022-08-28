#ifndef __J4FDEV_H__
#define __J4FDEV_H__

#ifndef unlikely
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#ifndef likely
#define likely(expr) __builtin_expect(!!(expr), 1)
#endif

#include "DevClock.h"
#include "DevClockATmega1284.h"
#include "DevClockATmega32.h"
#include "DevGpio.h"
#include "DevRtc.h"
#include "DevSd.h"
#include "DevTty.h"
#include "DevUser.h"

#endif // __J4FDEV_H__
