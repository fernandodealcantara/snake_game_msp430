#include <msp430.h>
#include <stdint.h>
#include "functions.h"

unsigned short lfsr = 0xCAFE;   // seed to generate pseudo-random numbers
unsigned bit;

// Send 16 bits as: xxxxaaaadddddddd (ignore, address, data)
// and use active low Chip Select
void spi_send(uint8_t address, uint8_t byte){
    P6OUT &= ~BIT4; // CS low

    while (!(UCB0IFG & UCTXIFG)){} // Wait until done
    UCB0TXBUF = (address & 0x0F);

    while (!(UCB0IFG & UCTXIFG)){} // Wait until done
    UCB0TXBUF = byte;

    while (!(UCB0IFG & UCTXIFG)){} // Wait until done
    P6OUT |= BIT4; // CS high
}

void i2c_send(uint8_t address, uint8_t byte){
    UCB1I2CSA  = address;             // Set address
    UCB1CTLW0 |= UCTXSTT | UCTR;   // Start

    while(!(UCB1IFG & UCTXIFG));   // Wait start
    UCB1TXBUF = byte;

    while(!(UCB1IFG & UCTXIFG));   // Wait byte be sent
    UCB1CTLW0 |= UCTXSTP;          // Stop

    while(UCB1CTLW0 & UCTXSTP);    // Wait stop
}

uint8_t reverse_byte(uint8_t byte){
    uint8_t reversed_byte = 0x0;
    reversed_byte |= (BIT7 & byte) >> 7;
    reversed_byte |= (BIT6 & byte) >> 5;
    reversed_byte |= (BIT5 & byte) >> 3;
    reversed_byte |= (BIT4 & byte) >> 1;
    reversed_byte |= (BIT3 & byte) << 1;
    reversed_byte |= (BIT2 & byte) << 3;
    reversed_byte |= (BIT1 & byte) << 5;
    reversed_byte |= (BIT0 & byte) << 7;
    return reversed_byte;
}

unsigned rand(){ // https://en.wikipedia.org/wiki/Linear-feedback_shift_register
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  (lfsr >> 1) | (bit << 15);
}

void lcd_write_nibble(uint8_t nibble, uint8_t rs){
                                     // BT   EN   RW  RS
    i2c_send(LCD_ADDRESS, nibble << 4 | BT | 0  | 0 | rs );
    i2c_send(LCD_ADDRESS, nibble << 4 | BT | EN | 0 | rs );
    i2c_send(LCD_ADDRESS, nibble << 4 | BT | 0  | 0 | rs );
}

void lcd_write_byte(uint8_t byte, uint8_t rs){
    lcd_write_nibble(byte >> 4  , rs);
    lcd_write_nibble(byte & 0x0F, rs);
}

void print(char *str){
    // Return home and clear display
    lcd_write_byte(0x02, 0);
    lcd_write_byte(0x01, 0);
    while(*str) lcd_write_byte(*str++, 1); // print string
}
