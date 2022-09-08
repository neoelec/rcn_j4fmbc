#ifndef __INTERNAL__MBCDEV_H__
#define __INTERNAL__MBCDEV_H__

class MbcIo;

class MbcDev
{
public:
  virtual void run(MbcIo *io) = 0;
};

#endif // __INTERNAL__MBCDEV_H__
