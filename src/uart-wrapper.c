#include <avr/io.h>
#include <stdio.h>
#include "../lib/andygock_avr-uart/uart.h"

int uart0_putch(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart0_putch('\r', stream);
    }

    uart0_putc(c);
    return 0;
}

int uart0_getch(FILE *stream)
{
    (void) stream;
    return (char)uart0_getc();
}

int uart3_putch(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart3_putch('\r', stream);
    }

    uart3_putc(c);
    return 0;
}
