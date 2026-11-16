#include "gd32f30x.h"
#include "at24c256.h"
#include "i2c.h"
#include "uart.h"
#include "command.h"


int main(void)
{
    char buffer[64];
    char outcome_buffer[64];
    int8_t status = 100;
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
                    eeprom_read(cmd.address, outcome_buffer, 1);
                    break;
                case CMD_WRITE:
                    eeprom_write(cmd.address, &cmd.value, 1);
                    break;
                default:
                    break;
                }
            }
        }
    }

}