#ifndef __J4FV20MBCCFG_H__
#define __J4FV20MBCCFG_H__

#ifndef unlikely
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#ifndef likely
#define likely(expr) __builtin_expect(!!(expr), 1)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

#include "V20MbcCfg.h"
#include "V20MbcPin.h"
#include "V20MbcIo.h"
#include "V20MbcLoader.h"
#include "V20MbcMenu.h"

#endif // __J4FV20MBCCFG_H__
