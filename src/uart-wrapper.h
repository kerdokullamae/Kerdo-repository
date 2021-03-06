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

#ifndef UART_WRAPPER_H
#define UART_WRAPPER_H

int uart0_putch(char c, FILE *stream);
int uart0_getch(FILE *stream);

int uart3_putch(char c, FILE *stream);

/* Assign I/O stream to UART */
FILE uart0_io = FDEV_SETUP_STREAM(uart0_putch, uart0_getch, _FDEV_SETUP_RW);

FILE uart3_out = FDEV_SETUP_STREAM(uart3_putch, NULL, _FDEV_SETUP_WRITE);

/* UART_WRAPPER_H */
#endif
