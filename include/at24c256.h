#ifndef AT24C256_H
#define AT24C256_H

#include <stdint.h>
#include <stddef.h>
#include "gd32f30x.h"

#define EEPROM_ADDR(x) ((0x50 << 1) + (x))          /*!< EEPROM Адрес 0x50*/
#define EEPROM_MAX_BYTE_ADDR  0x7FFFU
#define EEPROM_CAPACITY       (EEPROM_MAX_BYTE_ADDR + 1U)

#define I2C_TIMEOUT  1000000U
/**
 * @brief Запись одного байта в eeprom
 * 
 * @return 0 - операция успешно завершена
 * @return -1 - ошибка обмена по I2C
 */
int8_t eeprom_write_byte(uint16_t address_dst, uint8_t data);

/**
 * @brief Чтение одного байта из eeprom
 * 
 * @return 0 - операция успешно завершена
 * @return -1 - ошибка обмена по I2C
 */
int8_t eeprom_read_byte(uint16_t address_src, uint8_t *res);


int8_t eeprom_write(uint16_t address_dst, const uint8_t *data, size_t length);

int8_t eeprom_read(uint16_t address_src, uint8_t *data, size_t length);

#endif
