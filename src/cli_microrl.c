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

#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"
#include "rfid.h"
#include "../lib/matejx_avr_lib/mfrc522.h"

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1},
    {read_cmd, read_help, read, 0},
    {add_cmd, add_help, add, 1},
    {remove_cmd, remove_help, remove, 1},
    {list_cmd, list_help, list, 0}
};


void cli_print(const char *str)
{
    printf("%s", str);
}


char cli_get_char(void)
{
    if (uart0_peek() != UART_NO_DATA) {
        return uart0_getc() & UART_STATUS_MASK;
    } else {
        return 0x00;
    }
}


void cli_print_help(const char *const *argv)
{
    (void) argv;
    putc('\n', stdout);
    printf_P(PSTR(CLI_HELP_MSG "\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        printf_P(cli_cmds[i].cmd);
        printf_P(PSTR(" : "));
        printf_P(cli_cmds[i].help);
        putc('\n', stdout);
    }
}


void print_version(FILE *stream)
{
    fprintf_P(stream, PSTR(PROG_VERSION "\n"),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stream, PSTR(LIBC_VERSION "\n"),
              PSTR(__AVR_LIBC_VERSION_STRING__),
              PSTR(__VERSION__));
}


void cli_print_ver(const char *const *argv)
{
    (void) argv;
    putc('\n', stdout);
    print_version(stdout);
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    putc('\n', stdout);
    // ASCII table print
    print_ascii_tbl(stdout);
    unsigned char ascii[128];
    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }
    print_for_human(stdout, ascii, sizeof(ascii));
}


void cli_handle_month(const char *const *argv)
{
    putc('\n', stdout);
    lcd_goto(0x40); // Got to the beginning of the next line
    char print_spaces = 16;
    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(argv[1], (PGM_P)pgm_read_word(&months[i]), 
            strlen(argv[1]))) {
                char print_count;
                print_count = fprintf_P(stdout, (PGM_P)pgm_read_word(&months[i]));
                fputc('\n', stdout);
                lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
                lcd_putc(' ');
                print_spaces -= (print_count + 1);
        }
    }
    // Clear
    for (; print_spaces > -1; print_spaces--) {
        lcd_putc(' ');
    }
}


void cli_print_cmd_error(void)
{
    putc('\n', stdout);
    printf_P(PSTR(CLI_NO_CMD "\n"));
}


void cli_print_cmd_arg_error(void)
{
    putc('\n', stdout);
    printf_P(PSTR(CLI_ARGS_MSG "\n"));
}

void read(void)
{
    Uid uid;
    Uid *uid_ptr = &uid;
    printf_P(PSTR("\n"));

    if (PICC_IsNewCardPresent()) {
        printf("Card selected!\n");
        PICC_ReadCardSerial(uid_ptr);
        printf("UID size: 0x%02X\n", uid.size);
        printf("UID sak: 0x%02X\n", uid.sak);
        printf("Card UID: ");

        for (byte i = 0; i < uid.size; i++) {
            printf("%02X", uid.uidByte[i]);
        }

        printf_P(PSTR("\n"));
    } else {
        printf_P((PSTR("No card detected!\n")));
    }
}

void add(const char *const *argv)
{
    int exists = 0;

    if (PICC_IsNewCardPresent()) {
        if (head != NULL) {
            card_t *current;
            current = head;
            int number = 1;

            while (current != NULL && exists == 0) {
                uint8_t *uid = current->uid;
                Uid card_uid;
                Uid *uid_ptra = &card_uid;
                PICC_ReadCardSerial(uid_ptra);

                if (!memcmp(current->name, argv[1], (strlen(current->name) + 1)) ||
                        !memcmp(card_uid.uidByte, current->uid, card_uid.size)) {
                    exists = 1;
                    printf("%d. ", number);

                    for (byte i = 0; i < current->size; i++) {
                        printf("%02X", uid[i]);
                    }

                    printf(" %s", current->name);
                }

                current = current->next;
                number++;
            }
        }

        if (exists == 0) {
            card_t *new_card;
            Uid uid;
            Uid *uid_ptr = &uid;
            new_card = malloc(sizeof(card_t));
            PICC_ReadCardSerial(uid_ptr);
            memcpy(&(new_card->size), &(uid.size), 1);
            memcpy(new_card->uid, uid.uidByte, uid.size);
            new_card->name = malloc(strlen(argv[1]) + 1);

            if (new_card == NULL || new_card->uid == NULL || new_card->name == NULL) {
                printf_P(PSTR("Not enough memory. Remove any of the cards added."));
            }

            strcpy(new_card->name, argv[1]);
            new_card->next = NULL;

            if (head != NULL) {
                card_t *current;
                current = head;

                while (current->next != NULL) {
                    current = current->next;
                }

                current->next = new_card;
            } else {
                head = new_card;
            }
        }
    } else if (exists == 0) {
        printf_P(PSTR("No card found!"));
    }

    printf_P(PSTR("\n"));
}

void remove(const char *const *argv)
{
    card_t *current;
    card_t *previous;
    current = head;
    previous = head;
    int in_list = 0;

    while (current != NULL) {
        if (current == head && current->next == NULL &&
                !memcmp(current->name, argv[1], (strlen(current->name) + 1))) {
            head = NULL;
            free(current->name);
            free(current);
            current = NULL;
            previous = NULL;
            in_list = 1;
        } else if (current == head && current->next != NULL &&
                   !memcmp(current->name, argv[1], (strlen(current->name) + 1))) {
            head = current-> next;
            free(current->name);
            free(current);
            current = head;
            previous = head;
            in_list = 1;
        } else if (current->next != NULL &&
                   !memcmp(current->name, argv[1], (strlen(current->name) + 1))) {
            previous->next = current->next;
            free(current->name);
            free(current);
            current = previous->next;
            in_list = 1;
        } else if (current->next == NULL  &&
                   !memcmp(current->name, argv[1], (strlen(current->name) + 1))) {
            previous->next = NULL;
            free(current->name);
            free(current);
            current = NULL;
            in_list = 1;
        } else if (current->next != NULL) {
            previous = current;
            current = current->next;
        } else {
            previous = current;
            current = NULL;
        }
    }

    if (in_list == 0) {
        printf("No card with such name!");
    }

    printf_P(PSTR("\n"));
}

void list(void)
{
    if (head == NULL) {
        printf_P(PSTR("0 cards added to the list!"));
    } else {
        int number = 1;
        card_t *current;
        current = head;

        while (current->next != NULL) {
            printf("%d. ", number);
            uint8_t *uid = current->uid;

            for (byte i = 0; i < current->size; i++) {
                printf("%02X", uid[i]);
            }

            printf(" %s", current->name);
            current = current->next;
            number++;
            printf_P(PSTR("\n"));
        }

        printf("%d. ", number);
        uint8_t *uid = current->uid;

        for (byte i = 0; i < current->size; i++) {
            printf("%02X", uid[i]);
        }

        printf(" %s", current->name);
    }

    printf_P(PSTR("\n"));
}

int cli_execute(int argc, const char *const *argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {

            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}
