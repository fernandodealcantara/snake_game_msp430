/*
 * Módulo contem funções auxiliares para o funcionamento do jogo
 */
#ifndef _FUNCTIONS_SNAKE_GAME
#define _FUNCTIONS_SNAKE_GAME

#include <msp430.h>
#include <stdint.h>

#define LCD_ADDRESS 0x27

#define BT BIT3
#define EN BIT2
#define RW BIT1

void spi_send(uint8_t address, uint8_t byte);
void i2c_send(uint8_t address, uint8_t byte);
uint8_t reverse_byte(uint8_t byte);
// pseudo-random number generator -> // https://en.wikipedia.org/wiki/Linear-feedback_shift_register
unsigned rand(void);
void print(char *); // show message in lcd
void lcd_write_nibble(uint8_t nibble, uint8_t rs);
void lcd_write_byte(uint8_t byte,   uint8_t rs);

#endif // _FUNCTIONS_SNAKE_GAME
