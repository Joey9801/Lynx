#include "debug.h"

void debug_send(const char *data)
{
    while (*data)
    {
        usart_send_blocking(USART1, *data);
        data++;
    }
}

void debug_send_int(int value)
{
        int i;
        int nr_digits = 0;
        char buffer[25];

        if (value < 0) {
                usart_send_blocking(USART1, '-');
                value = value * -1;
        }

        if (value == 0) {
                usart_send_blocking(USART1, '0');
        }

        while (value > 0) {
                buffer[nr_digits++] = "0123456789"[value % 10];
                value /= 10;
        }

        for (i = nr_digits-1; i >= 0; i--) {
                usart_send_blocking(USART1, buffer[i]);
        }
}
