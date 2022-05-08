#include <msp430.h>
#include "snake_game.h"
#include "config.h"

/**
 * main.c
 */
int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	spi_config();
	uart_config();
	i2c_config();
	MAX7219_config();
	lcd_config();
	timer_config();
	__enable_interrupt();

	board_initial_state();
	update_display_board();
	while(1){};
}

//Echo back RX character
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(){
    char receivedbyte = UCA0RXBUF;

    switch(receivedbyte){
        case '0': snake_command(UP);      break;
        case '1': snake_command(RIGHT);    break;
        case '2': snake_command(DOWN);    break;
        case '3': snake_command(LEFT);   break;
        case '4': snake_command(HALTED);  break;
        case '5': snake_command(RESTART);  break;
        default: break;
    }
}//USCI_A0_ISR

// Function executed each 0.5 seconds
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_CCIFG0_ISR(){
    game_logic();
    update_display_board();
    update_display_lcd();
}
