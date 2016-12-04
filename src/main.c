#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "hmi_msg.h"
#include "uart-wrapper.h"
#include "print_helper.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/helius_microrl/microrl.h"
#include "cli_microrl.h"


#define BAUDRATE 9600
#define BLINK_DELAY_MS 100
#define LED_INIT DDRA |= _BV(DDA3);
#define LED_TOGGLE PORTA ^= _BV(PORTA3)
#define UART_STATUS_MASK    0x00FF

//	Create	microrl	object	and	pointer	on	it
static microrl_t rl;
static microrl_t *prl = &rl;

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
    LED_INIT;

    init_system_clock();

    uart0_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;

    lcd_init();
    lcd_clrscr();

    sei();
}

static inline void start_ui (void)
{
    print_version(stderr);

    // print student name
    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout); // Add a new line to the uart printout
    lcd_puts_P(PSTR(STUD_NAME));
}

static inline void start_cli(void)
{
    // Call init with ptr to microrl instance and print callback
    microrl_init (prl, cli_print);
    // Set callback for execute
    microrl_set_execute_callback (prl, cli_execute);
}

static inline void heartbeat()
{
    static uint32_t last_time;
    uint32_t cur_time;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        cur_time = time;
    }
    if ((last_time - cur_time) > 0) {
        LED_TOGGLE;
        fprintf_P(stderr, PSTR(UPTIME "\n"), cur_time);
    }

    last_time = cur_time;
}

int main (void)
{
    hw_init();
    start_ui();
    start_cli();

    while (1) {
        heartbeat();
        microrl_insert_char (prl, cli_get_char());
    }
}

/* System clock */
ISR(TIMER5_COMPA_vect)
{
    time++;
}
