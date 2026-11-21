#include "gd32f30x.h"
#include "at24c256.h"
#include "i2c.h"
#include "uart.h"
#include "command.h"


int main(void)
{
    char buffer[64];
    char outcome_buffer[64];
    volatile int8_t status = 100;
    command_t cmd;
    
    i2c_init();
    uart_init();


    while (1) {
        status = uart_get_string(buffer, sizeof(buffer));
        if (status == 0) {
            if (command_parse(buffer, &cmd) == 0) {
                switch (cmd.command)
                {
                case CMD_READ:
                    status = eeprom_read(cmd.address, outcome_buffer, cmd.length);
                    if (status == 0) {
                        uart_send_string(outcome_buffer, cmd.length);
                    }
                    else {
                        uart_send_string("Error\0\n", 8);
                    }
                    break;
                case CMD_WRITE:
                    status = eeprom_write(cmd.address, cmd.value_arr, cmd.length);
                    if (status == 0) {
                        uart_send_string("Success!\0\n", 11);
                    }
                    else {
                        uart_send_string("Error\0\n", 8);
                    }
                    break;
                case CMD_WRITE_STR:
                    status = eeprom_write(cmd.address, cmd.value_arr, cmd.length);
                    if (status == 0) {
                        uart_send_string("Success!\0\n", 11);
                    } 
                    else {
                        uart_send_string("Error\0\n", 8);
                    }
                default:
                    break;
                }
            }
        }
    }

}