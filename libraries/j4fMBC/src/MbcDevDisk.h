#ifndef __INTERNAL__MBCDEVDISK_H__
#define __INTERNAL__MBCDEVDISK_H__

#include "j4fMbc.h"

class MbcDevDisk
{
public:
  void begin(MbcDisk *disk)
  {
    disk_ = disk;
  }

protected:
  MbcDisk *disk_;
};

// DISK EMULATION
// SELDISK - select the emulated disk number (binary). 100 disks are supported [0..99]:
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    DISK number (binary) [0..99]
//
//
// Opens the "disk file" correspondig to the selected disk number, doing some checks.
// A "disk file" is a binary file that emulates a disk using a LBA-like logical sector number.
// Every "disk file" must have a dimension of 8388608 bytes, corresponding to 16384 LBA-like logical sectors
//  (each sector is 512 bytes long), correspinding to 512 tracks of 32 sectors each (see SELTRACK and
//  SELSECT opcodes).
// Errors are stored into "errDisk" (see ERRDISK opcode).
//
//
// ...........................................................................................
//
// "Disk file" filename convention:
//
// Every "disk file" must follow the sintax "DSsNnn.DSK" where
//
//    "s" is the "disk set" and must be in the [0..9] range (always one numeric ASCII character)
//    "nn" is the "disk number" and must be in the [00..99] range (always two numeric ASCII characters)
//
// ...........................................................................................
//
//
// NOTE 1: The maximum disks number may be lower due the limitations of the used OS (e.g. CP/M 2.2 supports
//         a maximum of 16 disks)
// NOTE 2: Because SELDISK opens the "disk file" used for disk emulation, before using WRITESECT or READSECT
//         a SELDISK must be performed at first.
class MbcDevWrSELDISKClass : public MbcDevIo, public MbcDevDisk
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrSELDISKClass::run(MbcIo *io)
{
  disk_->ioWrSELDISK(io);
}

// DISK EMULATION
// SELTRACK - select the emulated track number (word splitted in 2 bytes in sequence: DATA 0 and DATA 1):
//
//                I/O DATA 0:  D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    Track number (binary) LSB [0..255]
//
//                I/O DATA 1:  D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    Track number (binary) MSB [0..1]
//
//
// Stores the selected track number into "trackSel" for "disk file" access.
// A "disk file" is a binary file that emulates a disk using a LBA-like logical sector number.
// The SELTRACK and SELSECT operations convert the legacy track/sector address into a LBA-like logical
//  sector number used to set the logical sector address inside the "disk file".
// A control is performed on both current sector and track number for valid values.
// Errors are stored into "diskErr" (see ERRDISK opcode).
//
//
// NOTE 1: Allowed track numbers are in the range [0..511] (512 tracks)
// NOTE 2: Before a WRITESECT or READSECT operation at least a SELSECT or a SELTRAK operation
//         must be performed
class MbcDevWrSELTRACKClass : public MbcDevIo, public MbcDevDisk
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrSELTRACKClass::run(MbcIo *io)
{
  disk_->ioWrSELTRACK(io);
}

// DISK EMULATION
// SELSECT - select the emulated sector number (binary):
//
//                  I/O DATA:  D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    Sector number (binary) [0..31]
//
//
// Stores the selected sector number into "sectSel" for "disk file" access.
// A "disk file" is a binary file that emulates a disk using a LBA-like logical sector number.
// The SELTRACK and SELSECT operations convert the legacy track/sector address into a LBA-like logical
//  sector number used to set the logical sector address inside the "disk file".
// A control is performed on both current sector and track number for valid values.
// Errors are stored into "diskErr" (see ERRDISK opcode).
//
//
// NOTE 1: Allowed sector numbers are in the range [0..31] (32 sectors)
// NOTE 2: Before a WRITESECT or READSECT operation at least a SELSECT or a SELTRAK operation
//         must be performed
class MbcDevWrSELSECTClass : public MbcDevIo, public MbcDevDisk
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrSELSECTClass::run(MbcIo *io)
{
  disk_->ioWrSELSECT(io);
}

// DISK EMULATION
// WRITESECT - write 512 data bytes sequentially into the current emulated disk/track/sector:
//
//                 I/O DATA 0: D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    First Data byte
//
//                      |               |
//                      |               |
//                      |               |                 <510 Data Bytes>
//                      |               |
//
//               I/O DATA 511: D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    512th Data byte (Last byte)
//
//
// Writes the current sector (512 bytes) of the current track/sector, one data byte each call.
// All the 512 calls must be always performed sequentially to have a WRITESECT operation correctly done.
// If an error occurs during the WRITESECT operation, all subsequent write data will be ignored and
//  the write finalization will not be done.
// If an error occurs calling any DISK EMULATION opcode (SDMOUNT excluded) immediately before the WRITESECT
//  opcode call, all the write data will be ignored and the WRITESECT operation will not be performed.
// Errors are stored into "diskErr" (see ERRDISK opcode).
//
// NOTE 1: Before a WRITESECT operation at least a SELTRACK or a SELSECT must be always performed
// NOTE 2: Remember to open the right "disk file" at first using the SELDISK opcode
// NOTE 3: The write finalization on SD "disk file" is executed only on the 512th data byte exchange, so be
//         sure that exactly 512 data bytes are exchanged.
class MbcDevWrWRITESECTClass : public MbcDevIo, public MbcDevDisk
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevWrWRITESECTClass::run(MbcIo *io)
{
  disk_->ioWrWRITESECT(io);
}

// DISK EMULATION
// ERRDISK - read the error code after a SELDISK, SELSECT, SELTRACK, WRITESECT, READSECT
//           or SDMOUNT operation
//
//                I/O DATA:    D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    DISK error code (binary)
//
//
// Error codes table:
//
//    error code    | description
// ---------------------------------------------------------------------------------------------------
//        0         |  No error
//        1         |  DISK_ERR: the function failed due to a hard error in the disk function,
//                  |   a wrong FAT structure or an internal error
//        2         |  NOT_READY: the storage device could not be initialized due to a hard error or
//                  |   no medium
//        3         |  NO_FILE: could not find the file
//        4         |  NOT_OPENED: the file has not been opened
//        5         |  NOT_ENABLED: the volume has not been mounted
//        6         |  NO_FILESYSTEM: there is no valid FAT partition on the drive
//       16         |  Illegal disk number
//       17         |  Illegal track number
//       18         |  Illegal sector number
//       19         |  Reached an unexpected EOF
//
//
//
//
// NOTE 1: ERRDISK code is referred to the previous SELDISK, SELSECT, SELTRACK, WRITESECT or READSECT
//         operation
// NOTE 2: Error codes from 0 to 6 come from the PetitFS library implementation
// NOTE 3: ERRDISK must not be used to read the resulting error code after a SDMOUNT operation
//         (see the SDMOUNT opcode)
class MbcDevRdERRDISKClass : public MbcDevIo, public MbcDevDisk
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdERRDISKClass::run(MbcIo *io)
{
  disk_->ioRdERRDISK(io);
}

// DISK EMULATION
// READSECT - read 512 data bytes sequentially from the current emulated disk/track/sector:
//
//                 I/O DATA:   D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                 I/O DATA 0  D7 D6 D5 D4 D3 D2 D1 D0    First Data byte
//
//                      |               |
//                      |               |
//                      |               |                 <510 Data Bytes>
//                      |               |
//
//               I/O DATA 127  D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    512th Data byte (Last byte)
//
//
// Reads the current sector (512 bytes) of the current track/sector, one data byte each call.
// All the 512 calls must be always performed sequentially to have a READSECT operation correctly done.
// If an error occurs during the READSECT operation, all subsequent read data will be = 0.
// If an error occurs calling any DISK EMULATION opcode (SDMOUNT excluded) immediately before the READSECT
//  opcode call, all the read data will be will be = 0 and the READSECT operation will not be performed.
// Errors are stored into "diskErr" (see ERRDISK opcode).
//
// NOTE 1: Before a READSECT operation at least a SELTRACK or a SELSECT must be always performed
// NOTE 2: Remember to open the right "disk file" at first using the SELDISK opcode
class MbcDevRdREADSECTClass : public MbcDevIo, public MbcDevDisk
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdREADSECTClass::run(MbcIo *io)
{
  disk_->ioRdREADSECT(io);
}

// DISK EMULATION
// SDMOUNT - mount a volume on SD, returning an error code (binary):
//
//                 I/O DATA 0: D7 D6 D5 D4 D3 D2 D1 D0
//                            ---------------------------------------------------------
//                             D7 D6 D5 D4 D3 D2 D1 D0    error code (binary)
//
//
//
// NOTE 1: This opcode is "normally" not used. Only needed if using a virtual disk from a custom program
//         loaded with iLoad or with the Autoboot mode (e.g. ViDiT). Can be used to handle SD hot-swapping
// NOTE 2: For error codes explanation see ERRDISK opcode
// NOTE 3: Only for this disk opcode, the resulting error is read as a data byte without using the
//         ERRDISK opcode
class MbcDevRdSDMOUNTClass : public MbcDevIo, public MbcDevDisk
{
public:
  inline void run(MbcIo *io);
};

inline void MbcDevRdSDMOUNTClass::run(MbcIo *io)
{
  disk_->ioRdSDMOUNT(io);
}

extern MbcDevWrSELDISKClass MbcDevWrSELDISK;
extern MbcDevWrSELTRACKClass MbcDevWrSELTRACK;
extern MbcDevWrSELSECTClass MbcDevWrSELSECT;
extern MbcDevWrWRITESECTClass MbcDevWrWRITESECT;
extern MbcDevRdERRDISKClass MbcDevRdERRDISK;
extern MbcDevRdREADSECTClass MbcDevRdREADSECT;
extern MbcDevRdSDMOUNTClass MbcDevRdSDMOUNT;

#endif // __INTERNAL__MBCDEVDISK_H__
