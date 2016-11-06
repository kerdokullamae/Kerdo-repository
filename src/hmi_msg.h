#include <avr/pgmspace.h>
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define prog_version "Version: %s built on: %s %s\n"
#define libc_version "avr-libc version: %s\n"
#define stud_name "Kerdo Kullamäe"
#define get_months "Enter month first letter ->"
const char months[6][10] PROGMEM = {
									"January",
									"February",
									"March",
									"April",
									"May",
									"June",
                                      };

#endif

