#!/bin/bash

FILE=S220718-R240620_IOS-Z80-MBC2.ino

perl -pi -e 's/(print|println)(\(".+\")/$1\(F$2\)/g' $FILE
perl -pi -e 's/pf_mount/PF\.begin/g' $FILE
perl -pi -e 's/pf_open/PF\.open/g' $FILE
perl -pi -e 's/pf_read/PF\.readFile/g' $FILE
perl -pi -e 's/pf_write/PF\.writeFile/g' $FILE
perl -pi -e 's/pf_lseek/PF\.seek/g' $FILE
perl -pi -e 's/#include "PetitFS.h"(.+$)/#include <pff.h>    $1\n#include <pffArduino.h>\n\n#if \(SD_CS_PIN != 4\)\n#error "SD_CS_PIN should be 4 in pffArduino.h"\n#endif/' $FILE
