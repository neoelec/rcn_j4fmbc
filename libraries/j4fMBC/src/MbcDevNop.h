#ifndef __INTERNAL__MBCDEVNOP_H__
#define __INTERNAL__MBCDEVNOP_H__

#include "j4fMbc.h"

class MbcDevRdWrNOP : public MbcDev
{
public:
  inline void run(MbcIo &io);
};

inline void MbcDevRdWrNOP::run(MbcIo &io)
{
}

#endif // __INTERNAL__MBCDEVNOP_H__
