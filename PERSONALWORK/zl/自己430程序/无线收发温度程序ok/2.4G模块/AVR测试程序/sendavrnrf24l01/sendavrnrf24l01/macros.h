#ifndef __MACROS_H
#define __MACROS_H	1

/* FOR ATMEL AVR and TINY AVR ONLY! */

#ifndef BIT
#define BIT(x)	(1 << (x))
#endif

#if defined(_AVR) && !defined(flash)
#define flash	const		/* IAR compatibility */
#endif

#ifndef C_task
#define C_task
#endif

#define _asm	asm			/* old style */

#define WDR() 	asm("wdr")
#define SEI()	asm("sei")
#define CLI()	asm("cli")
#define NOP()	asm("nop")
#define _WDR() 	asm("wdr")
#define _SEI()	asm("sei")
#define _CLI()	asm("cli")
#define _NOP()	asm("nop")

#ifdef _AVR
/* Serial Port Macros
 * for 4 Mhz crystal!
 * 
 * USE THE APPBUILDER for UART initialization!!!
 */
#define BAUD9600	25
#define BAUD19K		12

#define UART_TRANSMIT_ON()	USICR |= 0x8
#define UART_TRANSMIT_OFF()	USICR &= ~0x8
#define UART_RECEIVE_ON()	USICR |= 0x10
#define UART_RECEIVE_OFF()	USICR &= ~0x10

#define COMPRESS_DISABLE		NOCC_START()
#define COMPRESS_REENABLE		NOCC_END()

#define NOCC_START()	asm(".nocc_start")
#define NOCC_END()	asm(".nocc_end")

void _StackCheck(void);
void _StackOverflowed(char);

#endif

#endif
