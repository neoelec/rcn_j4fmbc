#ifndef __INTERNAL__DEVSD_H__
#define __INTERNAL__DEVSD_H__

#include <PFF3a.h>

class DevSd
{
public:
  enum
  {
    MOUNT = 0,
    OPEN,
    READ,
    WRITE,
    SEEK,
  };

  void begin(void);
  uint8_t mount(void);
  uint8_t open(const char *file_name);
  inline uint8_t read(void *buf, uint8_t sz_to_read, uint8_t &sz_read);
  inline uint8_t write(const void *buf, uint8_t sz_to_write, uint8_t &sz_wrote);
  inline uint8_t lseek(uint32_t sz_offset);
  void printError(uint8_t error, uint8_t operation, const char *file_name = nullptr);
  void waitKey(void);

private:
  void __printErrorCode(FRESULT error);
  void __printOperation(uint8_t operation);

  FATFS fatfs_;
};

inline uint8_t DevSd::read(void *buf, uint8_t sz_to_read, uint8_t &__sz_read)
{
  UINT sz_read;
  FRESULT error;

  error = PFF3a.read(buf, sz_to_read, &sz_read);

  __sz_read = static_cast<uint8_t>(sz_read);

  return static_cast<uint8_t>(error);
}

inline uint8_t DevSd::write(const void *buf, uint8_t sz_to_write, uint8_t &__sz_wrote)
{
  UINT sz_wrote;
  FRESULT error;

  error = PFF3a.write(buf, sz_to_write, &sz_wrote);

  __sz_wrote = static_cast<uint8_t>(sz_wrote);

  return static_cast<uint8_t>(error);
}

inline uint8_t DevSd::lseek(uint32_t sz_offset)
{
  FRESULT error;

  error = PFF3a.lseek(sz_offset);

  return static_cast<uint8_t>(error);
}

#endif // __INTERNAL__DEVSD_H__
