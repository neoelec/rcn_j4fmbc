// SPDX-License-Identifier: GPL-3.0-or-later

#include "j4fMbc.h"

void MbcDisk::begin(DevSd *sd)
{
  strcpy(disk_set_name_, DISK_SET_NAME_FMT);
  sd_ = sd;
  disk_error_ = UNEXPECTED_EOF;
}
