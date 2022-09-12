#ifndef __INTERNAL__J4FDEVH_H__
#define __INTERNAL__J4FDEVH_H__

#ifndef unlikely
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#ifndef likely
#define likely(expr) __builtin_expect(!!(expr), 1)
#endif

#endif // __INTERNAL__J4FDEVH_H__
