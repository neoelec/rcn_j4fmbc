#ifndef __INTERNAL__MBCDEVIO_H__
#define __INTERNAL__MBCDEVIO_H__

#include "MbcIo.h"

class MbcDevIo
{
public:
  virtual void run(MbcIo *io) = 0;
};

#endif // __INTERNAL__MBCDEVIO_H__
