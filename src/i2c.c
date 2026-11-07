#include "i2c.h"

static void i2c_bus_unlock(void) {
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_init(GPIOB, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_bit_set(GPIOB, GPIO_PIN_7);

    for (int i = 0; i < 9; i++) {
        gpio_bit_reset(GPIOB, GPIO_PIN_6);
        for(volatile int d=0; d<50; d++); 
        gpio_bit_set(GPIOB, GPIO_PIN_6);
        for(volatile int d=0; d<50; d++);
    }

    gpio_bit_reset(GPIOB, GPIO_PIN_7);
    for(volatile int d=0; d<50; d++);
    gpio_bit_set(GPIOB, GPIO_PIN_7); 
}


void i2c_init() {

    i2c_bus_unlock();
    rcu_periph_clock_enable(RCU_I2C0);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    i2c_deinit(I2C0);
    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);

    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0);
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    i2c_enable(I2C0);

    
}

