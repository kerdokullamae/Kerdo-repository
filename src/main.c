#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hmi_msg.h"
#include "uart.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 100

int main (void)
{
	DDRA |= _BV(DDA3);
	uart0_init();
    uart3_init();
	stdout = stdin = &uart0_io;
	stderr = &uart3_out;
	lcd_init();
	lcd_clrscr();

    fprintf_P(stderr, PSTR(prog_version), PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(libc_version), PSTR(__AVR_LIBC_VERSION_STRING__));
	fprintf_P(stdout, PSTR(stud_name));
    fputc('\n', stdout);

	print_ascii_tbl(stdout);
	unsigned char ascii[128] = {0};

	for (unsigned char i = 0; i < sizeof(ascii); i++) 
	{
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, sizeof(ascii));
    lcd_puts_P(PSTR(stud_name));

    while (1) 
	{
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);

        char first_letter;
        fprintf_P(stdout, PSTR(get_months));
        fscanf(stdin, "%c", &first_letter);
        fprintf(stdout, "%c\n", first_letter);
		lcd_goto(0x40);
        for (int i = 0; i < 6; i++) 
		{
            if (!strncmp_P(&first_letter, months[i], 1)) 
			{
                fprintf_P(stdout, months[i]);
                fputc('\n', stdout);
                lcd_puts_P(months[i]);
                lcd_putc(' ');
            }
        }
	
		/* Because lcd is 16 characters long(?) add 16 spaces to clear */
        lcd_puts_P(PSTR("                "));

        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}

