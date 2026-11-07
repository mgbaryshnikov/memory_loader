#ifndef UART_H
#define UART_H

#include "gd32f30x.h"
#include <stdint.h>
#include <stddef.h>

#define UART_TIMEOUT  1000000U


void uart_init(void);

int8_t uart_send_byte(uint8_t data);

int8_t uart_get_byte(uint8_t *buffer);

int8_t uart_get_string(char *buffer, size_t buffer_size);

#endif