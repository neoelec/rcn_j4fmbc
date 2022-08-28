// SPDX-License-Identifier: GPL-3.0-or-later

#include <Arduino.h>

#include "j4fZ80Mbc2.h"

void Z80Mbc2Loader::begin(Z80Mbc2Dev &dev)
{
  clock_ = dev.getClock();
  sd_ = dev.getSd();
  disk_ = dev.getDisk();
  pin_ = dev.getPin();
}

void Z80Mbc2Loader::bootstrap(void)
{
  Z80Mbc2Cfg cfg;

  cfg.begin(sd_);
  cfg.printCfg();
  disk_->setDiskSetIdx(cfg.getDiskSet());
  clock_->setMode(cfg.getClkMode());

  __warmReset();
  pin_->setPIN_WAIT_nRES_HIGH();
  __setBootAddress(&cfg);
  __loadFwFile(&cfg);
  pin_->setPIN_nRESET_LOW();
  clock_->enable();
  Serial.println(F("IOS: Z80 CPU is running from now"));

  while (Serial.available() > 0)
    Serial.read();

  delay(1);
  pin_->setPIN_nRESET_HIGH();
}

void Z80Mbc2Loader::__setBootAddress(Z80Mbc2Cfg *cfg)
{
  const uint8_t op_JP_nn = 0xC3;
  uint16_t boot_address = cfg->getBootAddress();

  __loadHL(0x0000); // HL = 0x0000 (used as pointer to RAM)
  if (boot_address > 0x0000)
  {
    __loadByteToRam(op_JP_nn);               // Write the JP opcode @ 0x0000;
    __loadByteToRam(lowByte(boot_address));  // Write LSB to jump @ 0x0001
    __loadByteToRam(highByte(boot_address)); // Write MSB to jump @ 0x0002
  }
}

void Z80Mbc2Loader::__warmReset(void)
{
  pin_->setPIN_nRESET_LOW();
  __pulseClock(6);
  pin_->setPIN_nRESET_HIGH();
  __pulseClock(2);
}

void Z80Mbc2Loader::__pulseClock(uint16_t n)
{
  for (uint16_t i = 0; i < n; i++)
  {
    pin_->setPIN_CLK_HIGH();
    pin_->setPIN_CLK_LOW();
  }
}

void Z80Mbc2Loader::__openFwFile(const char *fw_file)
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

void Z80Mbc2Loader::__loadFwFile(Z80Mbc2Cfg *cfg)
{
  __openFwFile(cfg->getBootFile());
  Serial.printf(F("IOS: Loading boot program (%-12s)..."), cfg->getBootFile());
  ____loadFwFile(cfg->getLoadAddress(), cfg->getBootFile());
  Serial.println(F(" Done"));
  __loadHL(cfg->getBootAddress());
}

void Z80Mbc2Loader::____loadFwFile(uint16_t offset, const char *fw_file)
{
  bool empty_file = true;
  uint8_t buffer[32];
  uint8_t error;
  uint8_t sz_read;

  do
  {
    __loadHL(offset);

    do
    {
      error = sd_->read(buffer, sizeof(buffer), sz_read);
      if (sz_read > 0)
        empty_file = false;

      if (error != FR_OK)
        break;

      for (uint8_t i = 0; i < sz_read; i++)
        __loadByteToRam(buffer[i]);
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

  return;

__empty_file:
  Serial.println();
  Serial.print(F("IOS: Empty file - Load aborted!"));
  while (1)
    ;
}

// Load a given byte to RAM using a sequence of two Z80 instructions forced on the data bus.
// The RAM_CE2 signal is used to force the RAM in HiZ, so the Atmega can write the needed instruction/data
//  on the data bus. Controlling the clock signal and knowing exactly how many clocks pulse are required it
//  is possible control the whole loading process.
// In the following "T" are the T-cycles of the Z80 (See the Z80 datashet).
// The two instruction are "LD (HL), n" and "INC (HL)".
void Z80Mbc2Loader::__loadByteToRam(byte value)
{
  const uint8_t op_LD_HL_n = 0x36;
  const uint8_t op_INC_HL = 0x23;

  // Execute the LD(HL),n instruction (T = 4+3+3). See the Z80 datasheet and manual.
  // After the execution of this instruction the <value> byte is loaded in the memory address pointed by HL.
  __pulseClock(1);                // Execute the T1 cycle of M1 (Command Fetch machine cycle)
  pin_->setPIN_RAM_CE2_LOW();     // Force the RAM in HiZ (CE2 = LOW)
  pin_->setPORT_DATA(op_LD_HL_n); // Configure Z80 data bus D0-D7 (PA0-PA7) as output
                                  // Write "LD (HL), n" opcode on data bus
  __pulseClock(2);                // Execute T2 and T3 cycles of M1
  pin_->releasePORT_DATA();       // Configure Z80 data bus D0-D7 (PA0-PA7) as input...
                                  // ...with pull-up
  __pulseClock(2);                // Complete the execution of M1 and execute the T1 cycle of the following
                                  // Memory Read machine cycle
  pin_->setPORT_DATA(value);      // Configure Z80 data bus D0-D7 (PA0-PA7) as output
                                  // Write the byte to load in RAM on data bus
  __pulseClock(2);                // Execute the T2 and T3 cycles of the Memory Read machine cycle
  pin_->releasePORT_DATA();       // Configure Z80 data bus D0-D7 (PA0-PA7) as input...
                                  // ...with pull-up
  pin_->setPIN_RAM_CE2_HIGH();    // Enable the RAM again (CE2 = HIGH)
  __pulseClock(3);                // Execute all the following Memory Write machine cycle

  // Execute the INC(HL) instruction (T = 6). See the Z80 datasheet and manual.
  // After the execution of this instruction HL points to the next memory address.
  __pulseClock(1);               // Execute the T1 cycle of M1 (Command Fetch machine cycle)
  pin_->setPIN_RAM_CE2_LOW();    // Force the RAM in HiZ (CE2 = LOW)
  pin_->setPORT_DATA(op_INC_HL); // Configure Z80 data bus D0-D7 (PA0-PA7) as output
                                 // Write "INC(HL)" opcode on data bus
  __pulseClock(2);               // Execute T2 and T3 cycles of M1
  pin_->releasePORT_DATA();      // Configure Z80 data bus D0-D7 (PA0-PA7) as input...
                                 // ...with pull-up
  pin_->setPIN_RAM_CE2_HIGH();   // Enable the RAM again (CE2 = HIGH)
  __pulseClock(3);               // Execute all the remaining T cycles
}

// Load "value" word into the HL registers inside the Z80 CPU, using the "LD HL,nn" instruction.
// In the following "T" are the T-cycles of the Z80 (See the Z80 datashet).
void Z80Mbc2Loader::__loadHL(uint16_t value)
{
  const uint8_t op_LD_HL_nn = 0x21;

  // Execute the LD dd,nn instruction (T = 4+3+3), with dd = HL and nn = value. See the Z80 datasheet and manual.
  // After the execution of this instruction the word "value" (16bit) is loaded into HL.
  __pulseClock(1);                     // Execute the T1 cycle of M1 (Command Fetch machine cycle)
  pin_->setPIN_RAM_CE2_LOW();          // Force the RAM in HiZ (CE2 = LOW)
  pin_->setPORT_DATA(op_LD_HL_nn);     // Configure Z80 data bus D0-D7 (PA0-PA7) as output
                                       // Write "LD HL, n" opcode on data bus
  __pulseClock(2);                     // Execute T2 and T3 cycles of M1
  pin_->releasePORT_DATA();            // Configure Z80 data bus D0-D7 (PA0-PA7) as input...
                                       // ...with pull-up
  __pulseClock(2);                     // Complete the execution of M1 and execute the T1 cycle of the following
                                       // Memory Read machine cycle
  pin_->setPORT_DATA(lowByte(value));  // Configure Z80 data bus D0-D7 (PA0-PA7) as output
                                       // Write first byte of "value" to load in HL
  __pulseClock(3);                     // Execute the T2 and T3 cycles of the first Memory Read machine cycle
                                       // and T1, of the second Memory Read machine cycle
  pin_->setPORT_DATA(highByte(value)); // Write second byte of "value" to load in HL
  __pulseClock(2);                     // Execute the T2 and T3 cycles of the second Memory Read machine cycle
  pin_->releasePORT_DATA();            // Configure Z80 data bus D0-D7 (PA0-PA7) as input...
                                       // ...with pull-up
  pin_->setPIN_RAM_CE2_HIGH();         // Enable the RAM again (CE2 = HIGH)
}
