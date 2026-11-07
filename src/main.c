#include "gd32f30x.h"
#include "at24c256.h"
#include "i2c.h"
#include "uart.h"



int main(void)
{
    
    i2c_init();
    uart_init();

    while (1) {
        
    }

}