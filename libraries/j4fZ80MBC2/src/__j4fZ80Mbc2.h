#ifndef __INTERNAL__J4FZ80MBC2_H__
#define __INTERNAL__J4FZ80MBC2_H__

#ifndef unlikely
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#ifndef likely
#define likely(expr) __builtin_expect(!!(expr), 1)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

#endif // __INTERNAL__J4FZ80MBC2_H__
