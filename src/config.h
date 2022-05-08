/*
 * Módulo contem as configurações dos modulos utilizados
 */
#ifndef _CONFIG_SNAKE_GAME
#define _CONFIG_SNAKE_GAME

// MAX7219 Register addresses
#define MAX_NOOP 0x00
#define MAX_DIGIT0 0x01
#define MAX_DIGIT1 0x02
#define MAX_DIGIT2 0x03
#define MAX_DIGIT3 0x04
#define MAX_DIGIT4 0x05
#define MAX_DIGIT5 0x06
#define MAX_DIGIT6 0x07
#define MAX_DIGIT7 0x08
#define MAX_DECODEMODE 0x09
#define MAX_INTENSITY 0x0A
#define MAX_SCANLIMIT 0x0B
#define MAX_SHUTDOWN 0x0C
#define MAX_DISPLAYTEST 0x0F

#define HALFSEC 16384 // Tof = 0.5s = T*N = (1/32768)*N, N=16384

void spi_config(void);
void uart_config(void);
void i2c_config(void);
void timer_config(void);
void MAX7219_config(void);
void lcd_config(void);

#endif // _CONFIG_SNAKE_GAME
