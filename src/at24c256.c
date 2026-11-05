#include "at24c256.h"


static void delay_ms(uint32_t ms)
{
    SysTick->LOAD = SystemCoreClock / 1000U - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    while (ms--) {
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0U) {
        }
    }

    SysTick->CTRL = 0U;
}

static int i2c_wait_flag(i2c_flag_enum flag)
{
    uint32_t timeout = I2C_TIMEOUT;
    while (i2c_flag_get(I2C0, flag) == RESET) {
        if (--timeout == 0U) {
            return -1;
        }
    }

    return 0;
}

static int i2c_error_handler(void)
{
    i2c_stop_on_bus(I2C0);
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    return -1;
}

static int eeprom_range_valid(uint16_t address, size_t size) {
    if (address > EEPROM_MAX_BYTE_ADDR) return 0;
    if (size > (size_t)(EEPROM_CAPACITY - address)) return 0;
    return 1;
}

int8_t eeprom_write_byte(uint16_t address_dst, uint8_t data) {
    
    uint32_t timeout;

    timeout = I2C_TIMEOUT;
    
    while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY) != RESET) {
        if (--timeout == 0U) {
            return -1;
        }
    }

    i2c_start_on_bus(I2C0);
    

    if (i2c_wait_flag(I2C_FLAG_SBSEND) != 0) {
        return i2c_error_handler();
    }

    i2c_master_addressing(I2C0, EEPROM_ADDR(0), I2C_TRANSMITTER);

    if (i2c_wait_flag(I2C_FLAG_ADDSEND) != 0) {
        return i2c_error_handler();
    }

    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

   if (i2c_wait_flag(I2C_FLAG_TBE) != 0) {
        return i2c_error_handler();
    }
    i2c_data_transmit(I2C0, (uint8_t)(address_dst >> 8));

    if (i2c_wait_flag(I2C_FLAG_BTC) != 0) {
        return i2c_error_handler();
    }

    i2c_data_transmit(I2C0, (uint8_t)address_dst);

    if (i2c_wait_flag(I2C_FLAG_BTC) != 0) {
        return i2c_error_handler();
    }

    i2c_data_transmit(I2C0, data);

    if (i2c_wait_flag(I2C_FLAG_BTC) != 0) {
        return i2c_error_handler();
    }

    i2c_stop_on_bus(I2C0);

    return 0;
}


int8_t eeprom_read_byte(uint16_t address_src, uint8_t *data)
{
    uint32_t timeout;

    timeout = I2C_TIMEOUT;
    while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY) != RESET) {
        if (--timeout == 0U) {
            return -1;
        }
    }

    i2c_start_on_bus(I2C0);

    if (i2c_wait_flag(I2C_FLAG_SBSEND) != 0) {
        return i2c_error_handler();
    }

    i2c_master_addressing(I2C0, EEPROM_ADDR(0), I2C_TRANSMITTER);

    if (i2c_wait_flag(I2C_FLAG_ADDSEND) != 0) {
        return i2c_error_handler();
    }

    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    if (i2c_wait_flag(I2C_FLAG_TBE) != 0) {
        return i2c_error_handler();
    }

    i2c_data_transmit(I2C0, (uint8_t)(address_src >> 8));

    if (i2c_wait_flag(I2C_FLAG_BTC) != 0) {
        return i2c_error_handler();
    }

    i2c_data_transmit(I2C0, (uint8_t)address_src);

    if (i2c_wait_flag(I2C_FLAG_BTC) != 0) {
        return i2c_error_handler();
    }

    i2c_start_on_bus(I2C0);

    if (i2c_wait_flag(I2C_FLAG_SBSEND) != 0) {
        return i2c_error_handler();
    }

    i2c_master_addressing(I2C0, EEPROM_ADDR(1), I2C_RECEIVER);

    if (i2c_wait_flag(I2C_FLAG_ADDSEND) != 0) {
        return i2c_error_handler();
    }
    i2c_ack_config(I2C0, I2C_ACK_DISABLE);
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    i2c_stop_on_bus(I2C0);

    if (i2c_wait_flag(I2C_FLAG_RBNE) != 0) {
        return i2c_error_handler();
    }

    *data = i2c_data_receive(I2C0);

    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    return 0;
}


int8_t eeprom_write(uint16_t address_dst, const uint8_t *data, size_t length) {
    if (!eeprom_range_valid(address_dst, length)) return -1;
    int8_t write_status;

    size_t first_page_bytes = EEPROM_PAGE_SIZE - address_dst % EEPROM_PAGE_SIZE;
    write_status = eeprom_write_page(address_dst, data, first_page_bytes);
    if (write_status != 0) return -1;
    delay_ms(5);

    address_dst += first_page_bytes;
    data += first_page_bytes;
    length -= first_page_bytes;

    size_t pages_left = length / 64;
    size_t last_page = length % 64;

    for (size_t i = 0; i < pages_left; i++) {
        size_t current_len = length >= 64 ? 64 : length;
        write_status = eeprom_write_page(address_dst, data, current_len);

        length -= current_len;
        address_dst += current_len; 
        data += current_len;

        if (write_status != 0) return -1;
        delay_ms(5);
    }

    write_status = eeprom_write_page(address_dst, data, last_page);
    if (write_status != 0) return -1;

    delay_ms(5);

    return 0;
}


int8_t eeprom_read(uint16_t address_src, uint8_t *data, size_t length) {
    if (!eeprom_range_valid(address_src, length)) return -1;

    int8_t write_status;
    for (size_t i = 0; i < length; i++) {
        write_status = eeprom_read_byte(address_src++, &data[i]);
        if (write_status != 0) return -1;

        delay_ms(5);
    }

    return 0;
}

int8_t eeprom_write_page(uint16_t address_dst, const uint8_t *data, size_t lenght) {
    if (data == NULL || lenght > EEPROM_PAGE_SIZE) {
        return -1;
    }

    uint32_t timeout;

    timeout = I2C_TIMEOUT;
    
    while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY) != RESET) {
        if (--timeout == 0U) {
            return -1;
        }
    }

    i2c_start_on_bus(I2C0);
    

    if (i2c_wait_flag(I2C_FLAG_SBSEND) != 0) {
        return i2c_error_handler();
    }

    i2c_master_addressing(I2C0, EEPROM_ADDR(0), I2C_TRANSMITTER);

    if (i2c_wait_flag(I2C_FLAG_ADDSEND) != 0) {
        return i2c_error_handler();
    }

    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

   if (i2c_wait_flag(I2C_FLAG_TBE) != 0) {
        return i2c_error_handler();
    }
    i2c_data_transmit(I2C0, (uint8_t)(address_dst >> 8));

    if (i2c_wait_flag(I2C_FLAG_BTC) != 0) {
        return i2c_error_handler();
    }

    i2c_data_transmit(I2C0, (uint8_t)address_dst);

    if (i2c_wait_flag(I2C_FLAG_BTC) != 0) {
        return i2c_error_handler();
    }

    for (size_t i = 0; i < lenght; i++) {
        i2c_data_transmit(I2C0, data[i]);

        if (i2c_wait_flag(I2C_FLAG_TBE) != 0) {
            return i2c_error_handler();
        }
    }

    if (i2c_wait_flag(I2C_FLAG_BTC) != 0) {
        return i2c_error_handler();
    }

    i2c_stop_on_bus(I2C0);

    return 0;
}



