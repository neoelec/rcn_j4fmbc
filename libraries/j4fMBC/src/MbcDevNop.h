#ifndef __INTERNAL__MBCDEVNOP_H__
#define __INTERNAL__MBCDEVNOP_H__

#include "MbcDevIo.h"

class MbcDevRdWrNOPClass : public MbcDevIo
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdWrNOPClass::run(MbcIo *io)
{
  io->setCommand(MbcIo::NO_OPERATION);
}

extern MbcDevRdWrNOPClass MbcDevRdWrNOP;

#endif // __INTERNAL__MBCDEVNOP_H__
