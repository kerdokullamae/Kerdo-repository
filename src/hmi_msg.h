/*This file is part of Foobar.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>. */

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
#define READ_CMD "read"
#define READ_HELP "Read UID from RFID card"
#define ADD_CMD "add"
#define ADD_HELP "Add card to memory by name, card needs to be in contact"
#define REMOVE_CMD "remove"
#define REMOVE_HELP "Remove card from memory by name"
#define LIST_CMD "list"
#define LIST_HELP "List all valid cards"

extern PGM_P const months[];

extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];
extern const char read_cmd[];
extern const char read_help[];
extern const char add_cmd[];
extern const char add_help[];
extern const char remove_cmd[];
extern const char remove_help[];
extern const char list_cmd[];
extern const char list_help[];
#endif

