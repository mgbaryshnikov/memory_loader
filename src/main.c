#include "gd32f30x.h"
#include "at24c256.h"
#include "i2c.h"


int main(void)
{
    volatile uint8_t buffer[4] = {0};

    const uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};

    i2c_init();

    volatile int write_result = eeprom_write(0x0000, data, 4);

    volatile int read_result = eeprom_read(0x0000, (uint8_t *)buffer, 4);

    while (1) {
        
    }
}