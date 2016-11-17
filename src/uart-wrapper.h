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
