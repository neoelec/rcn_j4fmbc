#ifndef __INTERNAL__MBCDISK_H__
#define __INTERNAL__MBCDISK_H__

#include <DevSd.h>

#include "MbcDev.h"
#include "MbcIo.h"

#define MAX_TRACK 512
#define MAX_SECTOR 32
#define MAX_DISK_PER_SET 99

#define SZ_BUFFER 32
#define SE_DISK_SET_NAME 12

#define DISK_SET_NAME_FMT "DSxNyy.DSK"

class MbcDisk
{
public:
  enum
  {
    ILL_DISK_NUM = 16,
    ILL_TRACK_NUM,
    ILL_SECT_NUM,
    UNEXPECTED_EOF,
  };

  void begin(DevSd *sd);
  inline void setDiskSetIdx(uint8_t disk_set);
  inline void ioWrSELDISK(MbcIo &io);
  inline void ioWrSELTRACK(MbcIo &io);
  inline void ioWrSELSECT(MbcIo &io);
  inline void ioWrWRITESECT(MbcIo &io);
  inline void ioRdERRDISK(MbcIo &io);
  inline void ioRdREADSECT(MbcIo &io);
  inline void ioRdSDMOUNT(MbcIo &io);

private:
  inline uint32_t __trackToSector(uint32_t track_number);
  inline uint32_t __sectorToOffset(uint32_t sector_number);
  inline bool __isValidTrackAndSector(void);

private:
  DevSd *sd_;
  uint8_t buffer_[SZ_BUFFER];
  char disk_set_name_[SE_DISK_SET_NAME];
  uint8_t disk_set_idx_;
  uint8_t disk_error_;
  uint16_t track_selected_;
  uint16_t sector_selected_;
};

inline void MbcDisk::setDiskSetIdx(uint8_t disk_set_idx)
{
  disk_set_idx_ = disk_set_idx;
}

inline void MbcDisk::ioWrSELDISK(MbcIo &io)
{
  const uint8_t disk_number = io.getData();

  if (disk_number <= MAX_DISK_PER_SET)
  {
    disk_set_name_[2] = disk_set_idx_ + '0';
    disk_set_name_[4] = (disk_number / 10) + '0';
    disk_set_name_[5] = (disk_number % 10) + '0';
    disk_error_ = sd_->open(disk_set_name_);
  }
  else
    disk_error_ = ILL_DISK_NUM;
}

inline void MbcDisk::ioWrSELTRACK(MbcIo &io)
{
  const uint16_t io_count = io.getCount();
  const uint8_t io_data = io.getData();

  if (!io_count)
    track_selected_ = io_data;
  else
  {
    track_selected_ = (static_cast<uint16_t>(io_data) << 8) | lowByte(track_selected_);

    if (__isValidTrackAndSector())
      disk_error_ = FR_OK;
    else
    {
      if (sector_selected_ < MAX_SECTOR)
        disk_error_ = ILL_TRACK_NUM;
      else
        disk_error_ = ILL_SECT_NUM;
    }

    io.setCommand(MbcIo::NO_OPERATION);
  }

  io.setCount(io_count + 1);
}

inline void MbcDisk::ioWrSELSECT(MbcIo &io)
{
  sector_selected_ = io.getData();

  if (__isValidTrackAndSector())
    disk_error_ = FR_OK;
  else
  {
    if (sector_selected_ < MAX_SECTOR)
      disk_error_ = ILL_TRACK_NUM;
    else
      disk_error_ = ILL_SECT_NUM;
  }
}

inline void MbcDisk::ioWrWRITESECT(MbcIo &io)
{
  const uint16_t io_count = io.getCount();

  if (!io_count)
  {
    if (__isValidTrackAndSector() && (disk_error_ == FR_OK))
    {
      uint32_t sector = __trackToSector(track_selected_);
      uint32_t offset = __sectorToOffset(sector + sector_selected_);

      disk_error_ = sd_->lseek(offset);
    }
  }

  if (disk_error_ == FR_OK)
  {
    uint16_t index = io_count % SZ_BUFFER;

    buffer_[index] = io.getData();

    if (index == (SZ_BUFFER - 1))
    {
      uint8_t sz_wrote;

      disk_error_ = sd_->write(buffer_, SZ_BUFFER, sz_wrote);
      if (sz_wrote < SZ_BUFFER)
        disk_error_ = UNEXPECTED_EOF;

      if (io_count >= (MAX_TRACK - 1))
      {
        if (disk_error_ == FR_OK)
          disk_error_ = sd_->write(NULL, 0, sz_wrote);

        io.setCommand(MbcIo::NO_OPERATION);
      }
    }
  }

  io.setCount(io_count + 1);
}

inline void MbcDisk::ioRdERRDISK(MbcIo &io)
{
  io.setData(disk_error_);
}

inline void MbcDisk::ioRdREADSECT(MbcIo &io)
{
  const uint16_t io_count = io.getCount();

  if (!io_count)
  {
    if (__isValidTrackAndSector() && (disk_error_ == FR_OK))
    {
      uint32_t sector = __trackToSector(track_selected_);
      uint32_t offset = __sectorToOffset(sector + sector_selected_);

      disk_error_ = sd_->lseek(offset);
    }
  }

  if (disk_error_ == FR_OK)
  {
    uint16_t index = io_count % SZ_BUFFER;

    if (!index)
    {
      uint8_t sz_read;

      disk_error_ = sd_->read(buffer_, SZ_BUFFER, sz_read);
      if (sz_read < SZ_BUFFER)
        disk_error_ = UNEXPECTED_EOF;
    }

    if (disk_error_ == FR_OK)
      io.setData(buffer_[index]);
  }

  if (io_count >= (MAX_TRACK - 1))
    io.setCommand(MbcIo::NO_OPERATION);

  io.setCount(io_count + 1);
}

inline void MbcDisk::ioRdSDMOUNT(MbcIo &io)
{
  io.setData(sd_->mount());
}

inline uint32_t MbcDisk::__trackToSector(uint32_t track_number)
{
  return track_number << 5;
}

inline uint32_t MbcDisk::__sectorToOffset(uint32_t sector_number)
{
  return sector_number << 9;
}

inline bool MbcDisk::__isValidTrackAndSector(void)
{
  return (track_selected_ < MAX_TRACK) && (sector_selected_ < MAX_SECTOR);
}

#endif // __INTERNAL__MBCDISK_H__
