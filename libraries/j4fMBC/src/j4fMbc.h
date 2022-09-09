#ifndef __J4FMBC_H__
#define __J4FMBC_H__

#ifndef unlikely
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#ifndef likely
#define likely(expr) __builtin_expect(!!(expr), 1)
#endif

#include "MbcIo.h"
#include "MbcDisk.h"
#include "MbcDevIo.h"
#include "MbcDevDisk.h"
#include "MbcDevGpio.h"
#include "MbcDevNop.h"
#include "MbcDevRtc.h"
#include "MbcDevSysFlags.h"
#include "MbcDevTty.h"
#include "MbcDevUser.h"

#endif // __J4FMBC_H__
