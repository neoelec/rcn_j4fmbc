// SPDX-License-Identifier: GPL-3.0-or-later

#include <Arduino.h>

#include "j4fV20Mbc.h"

void V20MbcLoader::begin(V20MbcPin *pin)
{
  clock_ = MbcDev.getClock();
  sd_ = MbcDev.getSd();
  disk_ = MbcDev.getDisk();
  pin_ = pin;

  // Reset the V20 CPU and load a byte into RAM just to pulse the ALE line resetting the HALT
  // status led if turned on at power up (see U7 in the A250220 schematic).
  // I don't like to see the red HALT led on after a power on when the RTC is asking for
  // a valid date/time...
  __warmReset();
  __storeOneByte(0x00, 0x0000);
}

void V20MbcLoader::bootstrap(void)
{
  V20MbcCfg cfg;

  cfg.begin(sd_);
  cfg.printCfg();
  disk_->setDiskSetIdx(cfg.getDiskSet());
  clock_->setMode(cfg.getClkMode());

  if (cfg.isMode80())
    __loadFwFor80Mode(&cfg);
  else
    __loadFwFor86Mode(&cfg);

  pin_->setPIN_nRDYRES_LOW();
  __warmReset();
  pin_->setPIN_nHOLDRES_HIGH();
  pin_->setPIN_RES_HIGH();
  clock_->enable();
  Serial.println(F("IOS: V20 CPU is running from now"));

  while (Serial.available() > 0)
    Serial.read();

  delay(1);
  pin_->setPIN_RES_LOW();
}

void V20MbcLoader::__loadFwFor86Mode(V20MbcCfg *cfg)
{
  __warmReset();
  __setBootAddress(cfg);
  __loadFwFile(cfg);
  __haltCpu();
}

void V20MbcLoader::__loadFwFor80Mode(V20MbcCfg *cfg)
{
//  const uint8_t op_JMPF = 0xEA;
  V20MbcCfg cfg80;
  uint16_t boot_size;

  cfg80.begin(sd_, "S170420.CSV");
  cfg80.printCfg();

  __warmReset();
  __setBootAddress(&cfg80);
  boot_size = __loadFwFile(cfg);
  //  __haltCpu();
  //  __warmReset();
  __loadFwFile(&cfg80);
  __storeOneWord(cfg->getBootOffset(), cfg80.getPtrBootOffset());
  __storeOneWord(boot_size, cfg80.getPtrBootSize());
  __haltCpu();
}

void V20MbcLoader::__setBootAddress(V20MbcCfg *cfg)
{
  const uint8_t op_JMPF = 0xEA;

  __loadDataSegment(0xFFFF);
  __storeOneByte(op_JMPF, 0x0000);
  __storeOneWord(cfg->getBootOffset(), 0x0001);
  __storeOneWord(cfg->getBootSegment(), 0x0003);
}

void V20MbcLoader::__warmReset(void)
{
  pin_->setPIN_RES_HIGH();
  __pulseClock(1000);
  pin_->setPIN_RES_LOW();
  pin_->setPIN_nRDYRES_HIGH();
}

void V20MbcLoader::__haltCpu(void)
{
  const uint8_t op_HLT = 0xF4;

  __feedOneByte(op_HLT);
  __pulseClock(15);
  delay(300);
}

void V20MbcLoader::__pulseClock(uint16_t n)
{
  for (uint16_t i = 0; i < n; i++)
  {
    pin_->setPIN_CLK_HIGH();
    pin_->setPIN_CLK_LOW();
  }
}

void V20MbcLoader::__openFwFile(const char *fw_file)
{
  uint8_t error;

__remount:
  while (true)
  {
    error = sd_->mount();
    if (error == FR_OK)
      break;

    sd_->printError(error, DevSd::MOUNT);
    sd_->waitKey();
  }

  while (true)
  {
    error = sd_->open(fw_file);
    if (error == FR_OK)
      break;

    if (error != FR_NO_FILE)
      goto __remount;

    sd_->printError(error, DevSd::OPEN, fw_file);
    sd_->waitKey();
  }
}

uint16_t V20MbcLoader::__loadFwFile(V20MbcCfg *cfg)
{
  uint16_t sz_read;

  __loadDataSegment(cfg->getLoadSegment());
  __openFwFile(cfg->getBootFile());
  Serial.printf(F("IOS: Loading boot program (%-12s)..."), cfg->getBootFile());
  sz_read = ____loadFwFile(cfg->getLoadOffset(), cfg->getBootFile());
  Serial.println(F(" Done"));

  return sz_read;
}

uint16_t V20MbcLoader::____loadFwFile(uint16_t offset, const char *fw_file)
{
  bool over_run = false;
  bool empty_file = true;
  uint8_t buffer[32];
  uint16_t load_offset;
  uint8_t error;
  uint8_t sz_read;

  do
  {
    load_offset = offset;

    do
    {
      error = sd_->read(buffer, sizeof(buffer), sz_read);
      if (sz_read > 0)
        empty_file = false;

      if (error != FR_OK)
        break;

      for (uint8_t i = 0; i < sz_read; i++)
      {
        if (over_run)
          goto __over_run;

        __storeOneByte(buffer[i], load_offset);

        if (++load_offset == 0)
          over_run = true;
      }
    } while (sz_read == sizeof(buffer));

    if (error)
    {
      sd_->printError(error, DevSd::READ, fw_file);
      sd_->waitKey();
      sd_->lseek(0);
    }
  } while (error);

  if (empty_file)
    goto __empty_file;

  return load_offset - offset;

__over_run:
  Serial.println();
  Serial.print(F("IOS: Overrun error - Load aborted!"));
  while (1)
    ;

__empty_file:
  Serial.println();
  Serial.print(F("IOS: Empty file - Load aborted!"));
  while (1)
    ;

  return 0;
}

#define MAX_RD_CLKPULSES 10

// "Feed" one byte to the V20 CPU.
//
// Wait for a Read Bus Cycle and force a byte on the Data Bus putting the SRAM in HiZ.
// It is assumed here a Memory Read Bus Cycles (no check is done on IOM_).
// For more info about the V20 bus timing see the uPD70108H datasheet.
//
// NOTE: Two kind of errors are detected here and will abort any further operation:
//
//               1 = Timeout error waiting the Read Bus Cycle,
//               2 = Unexpected bus state
//
void V20MbcLoader::__feedOneByte(uint8_t data)
{
  int8_t i;

  i = MAX_RD_CLKPULSES;
  do
  {
    if (i <= 0)
    // Bus error: Timeout error waiting the Read Bus Cycle
    {
      Serial.println();
      Serial.println(F("IOS: Read Bus Cycle timeout"));
      while (1)
        ;
    }

    __pulseClock(1);
    i--;
  } while (pin_->getPIN_nRD());
  //
  // Now RD_ = LOW so we are at the T1 state of a Read Bus Cycle
  //
  __pulseClock(1); // Now at T2 bus state
  if (pin_->getPIN_nRD())
  {
    // Bus error: Unexpected bus state
    Serial.println();
    Serial.println(F("IOS: Unexpected bus state"));
    while (1)
      ;
  }

  pin_->setPIN_nRAMEN_HIGH(); // Put SRAM in HiZ
  pin_->setPORT_DATA(data);   // Configure data bus D0-D7 (PA0-PA7) as output
                              // Put the byte on data bus (D0 and D3 cleared)
  __pulseClock(1);            // Go to the T3 bus state
  pin_->releasePORT_DATA();   // Set data bus D0-D7 as input with pull-up
  pin_->setPIN_nRAMEN_LOW();  // Enable the SRAM
}

// Load <dataSegment> into the DS data segment register of the V20 CPU.
// For more info about the V20 registers see the uPD70108H datasheet.
//
void V20MbcLoader::__loadDataSegment(uint16_t segment)
{
  const uint8_t op_MOV_AX_imm = 0xB8;
  const uint16_t op_MOV_DS_AX = 0x8ED8;

  __feedOneByte(op_MOV_AX_imm);
  __feedOneByte(lowByte(segment));
  __feedOneByte(highByte(segment));
  __feedOneByte(highByte(op_MOV_DS_AX));
  __feedOneByte(lowByte(op_MOV_DS_AX));
}

// Store one byte into RAM at the given memory offset using the V20 CPU.
// The effective phisycal memory address will be the sum of the <memOffset> with
// the current content of the DS register left-shited by 4.
// For more info about the V20 physical addressing mode see the uPD70108H datasheet.
//
void V20MbcLoader::__storeOneByte(uint8_t data, uint16_t offset)
{
  const uint16_t op_MOV_mem_imm = 0xC606;

  __feedOneByte(highByte(op_MOV_mem_imm));
  __feedOneByte(lowByte(op_MOV_mem_imm));
  __feedOneByte(lowByte(offset));
  __feedOneByte(highByte(offset));
  __feedOneByte(data);
}

void V20MbcLoader::__storeOneWord(uint16_t data, uint16_t offset)
{
  __storeOneByte(lowByte(data), offset);
  __storeOneByte(highByte(data), offset + 1);
}
