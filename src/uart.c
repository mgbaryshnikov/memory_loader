#include "uart.h"

void uart_init(void) {
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    usart_deinit(USART0);
    usart_baudrate_set(USART0, 9600);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_enable(USART0);
    
}

static int8_t uart_wait_flag(usart_flag_enum flag, uint32_t timeout) {

    while (usart_flag_get(USART0, flag) == RESET) {
        if (--timeout == 0U) return -1;
    }

    return 0;
}


int8_t uart_send_byte(uint8_t byte) {
    if (uart_wait_flag(USART_FLAG_TBE, UART_TIMEOUT) != 0) return -1;

    usart_data_transmit(USART0, byte);

    return 0;
}

int8_t uart_get_byte(uint8_t *buffer) {
    if (uart_wait_flag(USART_FLAG_RBNE, UART_STRING_TIMEOUT) != 0) return -1;

    *buffer = usart_data_receive(USART0);

    return 0;
}

int8_t uart_get_string(char *buffer, size_t buffer_size) {
    uint8_t buffer_byte;
    int8_t status;
    while (buffer_size-- > 1) {
        status = uart_get_byte(&buffer_byte);
        if (status != 0) return -1;
        if (buffer_byte == '\n' || buffer_byte == '\r') {
            *buffer = '\0';
            return 0;
        }
        *buffer = buffer_byte;
        buffer++;
    }
    *buffer = '\0';
    return -2;
}