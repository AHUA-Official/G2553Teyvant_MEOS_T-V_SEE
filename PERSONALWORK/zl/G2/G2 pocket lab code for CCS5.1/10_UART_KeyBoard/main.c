/*
 * main.c
 */
#include "MSP430G2553.h"
#include "UART_Event.h"
#include "UART_FIFO.h"
#include "UART_Global.h"
#include "UART_init.h"

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
	USCI_A0_init();
	_bis_SR_register(LPM3_bits) ;
	while(1){
	}
}
