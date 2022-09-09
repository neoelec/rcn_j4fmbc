#ifndef __INTERNAL__MBCDEVNOP_H__
#define __INTERNAL__MBCDEVNOP_H__

#include "j4fMbc.h"

class MbcDevRdWrNOPClass : public MbcDev
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdWrNOPClass::run(MbcIo *io)
{
}

extern MbcDevRdWrNOPClass MbcDevRdWrNOP;

#endif // __INTERNAL__MBCDEVNOP_H__
