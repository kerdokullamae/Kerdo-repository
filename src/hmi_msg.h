#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define PROG_VERSION "Version: %S built on: %S %S\n"
#define LIBC_VERSION "avr-libc version: %S\n"
#define STUD_NAME "Kerdo Kullamäe"
#define GET_MONTHS "Enter month first letter ->"
#define UPTIME "Uptime: %lu s"

#define HELP_CMD "help"
#define HELP_HELP "Get help"
#define VER_CMD "version"
#define VER_HELP "Print FW version"
#define ASCII_CMD "ascii"
#define ASCII_HELP "print ASCII tables"
#define MONTH_CMD "month"
#define MONTH_HELP "Find month from list. Use as: month <string>"
#define CLI_HELP_MSG "Available commands:"
#define CLI_NO_CMD "Command not available.\n Use <help> to get help."
#define CLI_ARGS_MSG "Wrong number of arguments for this command\nUse <help>"

extern PGM_P const months[];

extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];

#endif

