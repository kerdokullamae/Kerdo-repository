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

#include <avr/io.h>
#include <stdio.h>
#include "../lib/andygock_avr-uart/uart.h"

#define UART_STATUS_MASK    0x00FF

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
    return uart0_getc() & UART_STATUS_MASK;
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
