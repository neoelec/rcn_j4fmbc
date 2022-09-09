#ifndef __J4FZ80MBC2_H__
#define __J4FZ80MBC2_H__

#ifndef unlikely
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#ifndef likely
#define likely(expr) __builtin_expect(!!(expr), 1)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

#include "Z80Mbc2Cfg.h"
#include "Z80Mbc2Pin.h"
#include "Z80Mbc2DevBank.h"
#include "Z80Mbc2Io.h"
#include "Z80Mbc2Loader.h"
#include "Z80Mbc2Menu.h"

#endif // __J4FZ80MBC2_H__
