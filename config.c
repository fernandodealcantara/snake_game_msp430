#include <msp430.h>
#include "config.h"
#include "functions.h"

void spi_config(){
    P6OUT |= BIT4;        // Set P6.4 for CS
    P6DIR |= BIT4;        // Set P6.4 to output direction
    P3SEL |= BIT0 | BIT2; // P3.0,2 option select

    UCB0CTL1 |= UCSWRST;                         // **Put state machine in reset**
    UCB0CTL0 |= UCMST | UCCKPH | UCMSB | UCSYNC; // 3-pin, 8-bit SPI master, MSB

    UCB0CTL1 |= UCSSEL_2; // SMCLK
    UCB0BR0 = 0x02;       // 2
    UCB0BR1 = 0;          //
    UCB0CTL1 &= ~UCSWRST; // Initialize USCI state machine
}

void uart_config(){
    P3SEL |=  BIT4;  //P3.4 RX ;

    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_1;
    UCA0BR0 = 0x03;
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCBRS_3 | UCBRF_0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
}

void i2c_config(){
    UCB1CTLW0 |=  UCSWRST;
    P4SEL |=  BIT1 | BIT2;
    UCB1CTL0 |= UCMST | UCMODE_3| UCSYNC;
    UCB1CTL1 |= UCSSEL__SMCLK | UCTR;
    UCB1BRW =  100;
    UCB1CTLW0 &= ~UCSWRST;
}

void timer_config(){
    TA0CTL = TASSEL__ACLK | MC__UP;
    TA0CCR0 = HALFSEC;
    TA0CCTL0 |= CCIE;
}

void MAX7219_config(){
    // Initialise MAX7219 with 8x8 led matrix
    spi_send(MAX_NOOP, 0x00);      // NO OP (seems needed after power on)
    spi_send(MAX_SCANLIMIT, 0x07); // Enable all digits (always needed for current/8 per row)
    spi_send(MAX_INTENSITY, 0x01); // Display intensity (0x00 to 0x0F)
    spi_send(MAX_DECODEMODE, 0);   // No BCD decoding for led matrix

    // Clear all rows/digits
    spi_send(MAX_DIGIT0, 0);
    spi_send(MAX_DIGIT1, 0);
    spi_send(MAX_DIGIT2, 0);
    spi_send(MAX_DIGIT3, 0);
    spi_send(MAX_DIGIT4, 0);
    spi_send(MAX_DIGIT5, 0);
    spi_send(MAX_DIGIT6, 0);
    spi_send(MAX_DIGIT7, 0);
    spi_send(MAX_SHUTDOWN, 1); // Wake oscillators/display up
}

void lcd_config(){
    // 8 bits mode
    lcd_write_nibble(0x3, 0);
    lcd_write_nibble(0x3, 0);
    lcd_write_nibble(0x3, 0);

    // 4 bits mode
    lcd_write_nibble(0x2, 0);

    // Turn on cursor and display
    lcd_write_byte(0x0C, 0);

    // Return home and clear display
    lcd_write_byte(0x02, 0);
    lcd_write_byte(0x01, 0);
}
