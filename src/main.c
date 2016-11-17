#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hmi_msg.h"
#include "uart-wrapper.h"
#include "print_helper.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/hd44780_111/hd44780.h"

#define BAUD 9600
#define BLINK_DELAY_MS 100

volatile uint32_t time;

static inline void init_system_clock(void)
{
    TCCR5A = 0;
    TCCR5B = 0;
    TCCR5B |= _BV(WGM52) | _BV(CS52); // Clear Timer on Compare and fCPU/256
    OCR5A = 62549; // 1 sec
    TIMSK5 |= _BV(OCIE5A);
}

static inline void hw_init()
{
    DDRA |= _BV(DDA3); // setting Arduino pin 25 as output
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
    init_system_clock();
    sei(); // enables interupts
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
}

// print program version
static inline void print_prog_version()
{
    fprintf_P(stderr, PSTR(PROG_VERSION),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(LIBC_VERSION), PSTR(__AVR_LIBC_VERSION_STRING__));
}

// print on program startup
static inline void print_program_start()
{
    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout);
    lcd_puts_P(PSTR(STUD_NAME));
    print_ascii_tbl(stdout);
    unsigned char ascii[128];

    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, sizeof(ascii));
    fprintf_P(stdout, PSTR(GET_MONTHS));
}

static inline void search_for_month()
{
    char first_letter;
    fscanf(stdin, "%c", &first_letter);
    fprintf(stdout, "%c\n", first_letter);
    lcd_goto(0x40);

    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(&first_letter, (PGM_P)pgm_read_word(&months[i]), 1)) {
            fprintf_P(stdout, (PGM_P)pgm_read_word(&months[i]));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
            lcd_putc(' ');
        }
    }

    fprintf_P(stdout, PSTR(GET_MONTHS));
    /* Because lcd is 16 characters long(?) add 16 spaces to clear */
    lcd_puts_P(PSTR("                "));
}

static inline void heartbeat()
{
    static uint32_t last_time;
    uint32_t cur_time = time;

    if ((last_time - cur_time) > 0) {
        // arduino pin 25 led toggling
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME "\n"), cur_time);
    }

    last_time = cur_time;
}

int main (void)
{
    hw_init();
    print_prog_version();
    print_program_start();

    while (1) {
        heartbeat();

        if (uart0_available()) {
            search_for_month();
        }
    }
}

/* System clock */
ISR(TIMER5_COMPA_vect)
{
    time++;
}
