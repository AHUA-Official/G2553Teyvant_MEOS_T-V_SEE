#ifndef __iom48v_h
#define __iom48v_h

/* ATmega48 header file for
 * ImageCraft ICCAVR compiler
 */

/* i/o register addresses
 * >= 0x60 are memory mapped only
 */

/* sorted by function rather than by address
 */

/* last changed 2004/01/09
 */

/* Port D */
#define PIND	(*(volatile unsigned char *)0x29)
#define DDRD	(*(volatile unsigned char *)0x2A)
#define PORTD	(*(volatile unsigned char *)0x2B)

/* Port C */
#define PINC	(*(volatile unsigned char *)0x26)
#define DDRC	(*(volatile unsigned char *)0x27)
#define PORTC	(*(volatile unsigned char *)0x28)

/* Port B */
#define PINB	(*(volatile unsigned char *)0x23)
#define DDRB	(*(volatile unsigned char *)0x24)
#define PORTB	(*(volatile unsigned char *)0x25)

/* External Interrupts */
#define EIFR	(*(volatile unsigned char *)0x3C)
#define  INTF1    1
#define  INTF0    0
#define EIMSK	(*(volatile unsigned char *)0x3D)
#define  INT1     1
#define  INT0     0

/* GPIO */
#define GPIOR2	(*(volatile unsigned char *)0x4B)
#define GPIOR1	(*(volatile unsigned char *)0x4A)
#define GPIOR0	(*(volatile unsigned char *)0x3E)


/* ADC */
#define ADC 	(*(volatile unsigned int *)0x78) /* m/m */
#define ADCL	(*(volatile unsigned char *)0x78) /* m/m */
#define ADCH	(*(volatile unsigned char *)0x79) /* m/m */
#define ADCSRB	(*(volatile unsigned char *)0x7B) /* m/m */
#define  ACME     6
#define  ADTS2    2
#define  ADTS1    1
#define  ADTS0    0
#define ADCSRA	(*(volatile unsigned char *)0x7A) /* m/m */
#define  ADEN     7
#define  ADSC     6
#define  ADRF     5
#define  ADATE    5
#define  ADIF     4
#define  ADIE     3
#define  ADPS2    2
#define  ADPS1    1
#define  ADPS0    0
#define ADMUX	(*(volatile unsigned char *)0x7C) /* m/m */
#define  REFS1    7
#define  REFS0    6
#define  ADLAR    5
#define  MUX3     3
#define  MUX2     2
#define  MUX1     1
#define  MUX0     0

/* Analog Comparator Control and Status Register */
#define ACSR	(*(volatile unsigned char *)0x50)
#define  ACD      7
#define  ACBG     6
#define  ACO      5
#define  ACI      4
#define  ACIE     3
#define  ACIC     2
#define  ACIS1    1
#define  ACIS0    0


/* USART0 */
#define UBRR0H	(*(volatile unsigned char *)0xC5) /* m/m */
#define UBRR0L	(*(volatile unsigned char *)0xC4) /* m/m */
#define UBRR0	(*(volatile unsigned int *)0xC4) /* m/m */
#define UCSR0C	(*(volatile unsigned char *)0xC2) /* m/m */
#define  UMSEL01  7
#define  UMSEL00  6
#define  UPM01    5
#define  UPM00    4
#define  USBS0    3
#define  UCSZ01   2
#define  UDORD0   2
#define  UCSZ00   1
#define  UCPHA0   1
#define  UCPOL0   0
#define UCSR0B	(*(volatile unsigned char *)0xC1) /* m/m */
#define  RXCIE0   7
#define  TXCIE0   6
#define  UDRIE0   5
#define  RXEN0    4
#define  TXEN0    3
#define  UCSZ02   2
#define  RXB80    1
#define  TXB80    0
#define UCSR0A	(*(volatile unsigned char *)0xC0) /* m/m */
#define  RXC0     7
#define  TXC0     6
#define  UDRE0    5
#define  FE0      4
#define  DOR0     3
#define  UPE0     2
#define  U2X0     1
#define  MPCM0    0
#define UDR0	(*(volatile unsigned char *)0xC6) /* m/m */


/* 2-wire SI */
#define TWBR	(*(volatile unsigned char *)0xB8) /* m/m */
#define TWSR	(*(volatile unsigned char *)0xB9) /* m/m */
#define  TWS7     7
#define  TWS6     6
#define  TWS5     5
#define  TWS4     4
#define  TWS3     3
#define  TWPS1    1
#define  TWPS0    0
#define TWAR	(*(volatile unsigned char *)0xBA) /* m/m */
#define  TWGCE    0
#define TWDR	(*(volatile unsigned char *)0xBB) /* m/m */
#define TWCR	(*(volatile unsigned char *)0xBC) /* m/m */
#define  TWINT    7
#define  TWEA     6
#define  TWSTA    5
#define  TWSTO    4
#define  TWWC     3
#define  TWEN     2
#define  TWIE     0
#define TWAMR	(*(volatile unsigned char *)0xBD) /* m/m */
#define  TWAM6    7
#define  TWAM5    6
#define  TWAM4    5
#define  TWAM3    4
#define  TWAM2    3
#define  TWAM1    2
#define  TWAM0    1


/* SPI */
#define SPCR	(*(volatile unsigned char *)0x4C)
#define  SPIE     7
#define  SPE      6
#define  DORD     5
#define  MSTR     4
#define  CPOL     3
#define  CPHA     2
#define  SPR1     1
#define  SPR0     0
#define SPSR	(*(volatile unsigned char *)0x4D)
#define  SPIF     7
#define  WCOL     6
#define  SPI2X    0
#define SPDR	(*(volatile unsigned char *)0x4E)


/* EEPROM */
#define EECR	(*(volatile unsigned char *)0x3F)
#define  EEPM1    5
#define  EEPM0    4
#define  EERIE    3
#define  EEMWE    2
#define  EEMPE    2
#define  EEWE     1
#define  EEPE     1
#define  EERE     0
#define EEDR	(*(volatile unsigned char *)0x40)
#define EEAR	(*(volatile unsigned int *)0x41)
#define EEARL	(*(volatile unsigned char *)0x41)
#define EEARH	(*(volatile unsigned char *)0x42)
/* high byte unused with m48, must be written 0 */


/* MONDR */
#define MONDR	(*(volatile unsigned char *)0x51)

/* MCU */
#define MCUSR	(*(volatile unsigned char *)0x54)
#define  WDRF     3
#define  BORF     2
#define  EXTRF    1
#define  PORF     0
#define MCUCR	(*(volatile unsigned char *)0x55)
#define  PUD      4
/* not on m48 */
/* #define  IVSEL    1 */
/* #define  IVCE     0 */

/* SPM Control and Status Register */
#define SPMCSR	(*(volatile unsigned char *)0x57)
#define  SPMIE    7
#define  RWWSB    6
#define  RWWSRE   4
#define  BLBSET   3
#define  PGWRT    2
#define  PGERS    1
#define  SPMEN    0
#define  SELFPRGEN 0

/* SMCR */
#define SMCR	(*(volatile unsigned char *)0x53)
#define  SM2      3
#define  SM1      2
#define  SM0      1
#define  SE       0

/* Stack Pointer */
#define SP  	(*(volatile unsigned int *)0x5D)
#define SPL 	(*(volatile unsigned char *)0x5D)
#define SPH 	(*(volatile unsigned char *)0x5E)

/* Status REGister */
#define SREG	(*(volatile unsigned char *)0x5F)

/* Watchdog Timer Control Register */
#define WDTCSR	(*(volatile unsigned char *)0x60) /* m/m */
#define  WDIF     7
#define  WDIE     6
#define  WDP3     5
#define  WDCE     4
#define  WDE      3
#define  WDP2     2
#define  WDP1     1
#define  WDP0     0

/* CLKPR */
#define CLKPR	(*(volatile unsigned char *)0x61) /* m/m */
#define  CLKPCE   7
#define  CLKPS3   3
#define  CLKPS2   2
#define  CLKPS1   1
#define  CLKPS0   0

/* PRR */
#define PRR       (*(volatile unsigned char *)0x64) /* m/m */
#define  PRTWI    7
#define  PRTIM2   6
#define  PRTIM1   3
#define  PRTIM0   5
#define  PRSPI    2
#define  PRUSART0 1
#define  PRADC    0

/* OSCCAL */
#define OSCCAL    (*(volatile unsigned char *)0x66) /* m/m */


/* PCIFR */
#define PCIFR	(*(volatile unsigned char *)0x3B)
#define  PCIF2    2
#define  PCIF1    1
#define  PCIF0    0

/* PCICR */
#define PCICR	(*(volatile unsigned char *)0x68) /* m/m */
#define  PCIE2    2
#define  PCIE1    1
#define  PCIE0    0

/* PCMSK registers */
#define PCMSK2	(*(volatile unsigned char *)0x6D) /* m/m */
#define  PCINT23  7
#define  PCINT22  6
#define  PCINT21  5
#define  PCINT20  4
#define  PCINT19  3
#define  PCINT18  2
#define  PCINT17  1
#define  PCINT16  0
#define PCMSK1	(*(volatile unsigned char *)0x6C) /* m/m */
#define  PCINT14  6
#define  PCINT13  5
#define  PCINT12  4
#define  PCINT11  3
#define  PCINT10  2
#define  PCINT9   1
#define  PCINT8   0
#define PCMSK0	(*(volatile unsigned char *)0x6B) /* m/m */
#define  PCINT7   7
#define  PCINT6   6
#define  PCINT5   5
#define  PCINT4   4
#define  PCINT3   3
#define  PCINT2   2
#define  PCINT1   1
#define  PCINT0   0

/* EICRA */
#define EICRA	(*(volatile unsigned char *)0x69) /* m/m */
#define  ISC11    3
#define  ISC10    2
#define  ISC01    1
#define  ISC00    0


/* Timer/Counter 0 */
#define OCR0B	(*(volatile unsigned char *)0x48)
#define OCR0A	(*(volatile unsigned char *)0x47)
#define TCNT0	(*(volatile unsigned char *)0x46)
#define TCCR0B	(*(volatile unsigned char *)0x45)
#define  FOC0A    7
#define  FOC0B    6
#define  WGM02    3
#define  CS02     2
#define  CS01     1
#define  CS00     0
#define TCCR0A	(*(volatile unsigned char *)0x44)
#define  COM0A1   7
#define  COM0A0   6
#define  COM0B1   5
#define  COM0B0   4
#define  WGM01    1
#define  WGM00    0

/* Timer/Counter1 */
#define ICR1	(*(volatile unsigned int *)0x86) /* m/m */
#define ICR1L	(*(volatile unsigned char *)0x86) /* m/m */
#define ICR1H	(*(volatile unsigned char *)0x87) /* m/m */
#define OCR1B	(*(volatile unsigned int *)0x8A) /* m/m */
#define OCR1BL	(*(volatile unsigned char *)0x8A) /* m/m */
#define OCR1BH	(*(volatile unsigned char *)0x8B) /* m/m */
#define OCR1A	(*(volatile unsigned int *)0x88) /* m/m */
#define OCR1AL	(*(volatile unsigned char *)0x88) /* m/m */
#define OCR1AH	(*(volatile unsigned char *)0x89) /* m/m */
#define TCNT1	(*(volatile unsigned int *)0x84) /* m/m */
#define TCNT1L	(*(volatile unsigned char *)0x84) /* m/m */
#define TCNT1H	(*(volatile unsigned char *)0x85) /* m/m */
#define TCCR1C	(*(volatile unsigned char *)0x82) /* m/m */
#define  FOC1A    7
#define  FOC1B    6
#define TCCR1B	(*(volatile unsigned char *)0x81) /* m/m */
#define  ICNC1    7
#define  ICES1    6
#define  WGM13    4
#define  WGM12    3
#define  CS12     2
#define  CS11     1
#define  CS10     0
#define TCCR1A	(*(volatile unsigned char *)0x80) /* m/m */
#define  COM1A1   7
#define  COM1A0   6
#define  COM1B1   5
#define  COM1B0   4
#define  WGM11    1
#define  WGM10    0

/* Timer/Counter2 */
#define OCR2B	(*(volatile unsigned char *)0xB4)
#define OCR2A	(*(volatile unsigned char *)0xB3)
#define TCNT2	(*(volatile unsigned char *)0xB2)
#define TCCR2B	(*(volatile unsigned char *)0xB1)
#define  FOC2A    7
#define  FOC2B    6
#define  WGM22    3
#define  CS22     2
#define  CS21     1
#define  CS20     0
#define TCCR2A	(*(volatile unsigned char *)0xB0)
#define  COM2A1   7
#define  COM2A0   6
#define  COM2B1   5
#define  COM2B0   4
#define  WGM21    1
#define  WGM20    0


/* ASSR */
#define ASSR	(*(volatile unsigned char *)0xB6) /* m/m */
#define  EXCLK    6
#define  AS2      5
#define  TCN2UB   4
#define  OCR2AUB  3
#define  OCR2BUB  2
#define  TCR2AUB  1
#define  TCR2BUB  0


/* GTCCR */
#define GTCCR	(*(volatile unsigned char *)0x43)
#define  TSM      7
#define  PSR2     1
#define  PSRASY   1
#define  PSR10    0
#define  PSRSYNC  0


/* Timer/Counter Interrupts */
#define TIFR0	(*(volatile unsigned char *)0x35)
#define  OCF0B    2
#define  OCF0A    1
#define  TOV0     0
#define TIFR1	(*(volatile unsigned char *)0x36)
#define  ICF1     5
#define  OCF1B    2
#define  OCF1A    1
#define  TOV1     0
#define TIFR2	(*(volatile unsigned char *)0x37)
#define  OCF2B    2
#define  OCF2A    1
#define  TOV2     0

#define TIMSK0	(*(volatile unsigned char *)0x6E) /* m/m */
#define  OCIE0B   2
#define  OCIE0A   1
#define  TOIE0    0
#define TIMSK1	(*(volatile unsigned char *)0x6F) /* m/m */
#define  TICIE1   5
#define  OCIE1B   2
#define  OCIE1A   1
#define  TOIE1    0
#define TIMSK2	(*(volatile unsigned char *)0x70) /* m/m */
#define  OCIE2B   2
#define  OCIE2A   1
#define  TOIE2    0


/* Digital Inputs Disahle Registers */
#define DIDR1	(*(volatile unsigned char *)0x7F) /* m/m */
#define  AIN1D    1
#define  AIN0D    0
#define DIDR0	(*(volatile unsigned char *)0x7E) /* m/m */
#define  ADC5D    5
#define  ADC4D    4
#define  ADC3D    3
#define  ADC2D    2
#define  ADC1D    1
#define  ADC0D    0



/* Port B bits */
#define  PORTB7   7
#define  PORTB6   6
#define  PORTB5   5
#define  PORTB4   4
#define  PORTB3   3
#define  PORTB2   2
#define  PORTB1   1
#define  PORTB0   0
#define  PB7      7
#define  PB6      6
#define  PB5      5
#define  PB4      4
#define  PB3      3
#define  PB2      2
#define  PB1      1
#define  PB0      0

#define  DDB7     7
#define  DDB6     6
#define  DDB5     5
#define  DDB4     4
#define  DDB3     3
#define  DDB2     2
#define  DDB1     1
#define  DDB0     0

#define  PINB7    7
#define  PINB6    6
#define  PINB5    5
#define  PINB4    4
#define  PINB3    3
#define  PINB2    2
#define  PINB1    1
#define  PINB0    0

/* Port C bits */
#define  PORTC6   6
#define  PORTC5   5
#define  PORTC4   4
#define  PORTC3   3
#define  PORTC2   2
#define  PORTC1   1
#define  PORTC0   0
#define  PC6      6
#define  PC5      5
#define  PC4      4
#define  PC3      3
#define  PC2      2
#define  PC1      1
#define  PC0      0

#define  DDC6     6
#define  DDC5     5
#define  DDC4     4
#define  DDC3     3
#define  DDC2     2
#define  DDC1     1
#define  DDC0     0

#define  PINC6    6
#define  PINC5    5
#define  PINC4    4
#define  PINC3    3
#define  PINC2    2
#define  PINC1    1
#define  PINC0    0

/* Port D bits */
#define  PORTD7   7
#define  PORTD6   6
#define  PORTD5   5
#define  PORTD4   4
#define  PORTD3   3
#define  PORTD2   2
#define  PORTD1   1
#define  PORTD0   0
#define  PD7      7
#define  PD6      6
#define  PD5      5
#define  PD4      4
#define  PD3      3
#define  PD2      2
#define  PD1      1
#define  PD0      0

#define  DDD7     7
#define  DDD6     6
#define  DDD5     5
#define  DDD4     4
#define  DDD3     3
#define  DDD2     2
#define  DDD1     1
#define  DDD0     0

#define  PIND7    7
#define  PIND6    6
#define  PIND5    5
#define  PIND4    4
#define  PIND3    3
#define  PIND2    2
#define  PIND1    1
#define  PIND0    0



/* Lock and Fuse Bits with LPM/SPM instructions */

/* lock bits (Z = 0x0001) */
#define  BLB12    5
#define  BLB11    4
#define  BLB02    3
#define  BLB01    2

#define  LB2      1
#define  LB1      0

/* fuses low bits (Z = 0x0000) */
#define  CKDIV8   7
#define  CKOUT    6
#define  SUT1     5
#define  SUT0     4
#define  CKSEL3   3
#define  CKSEL2   2
#define  CKSEL1   1
#define  CKSEL0   0

/* fuses high bits (Z = 0x0003) */
#define  RSTDISBL 7
#define  DWEN     6
#define  SPIEN    5
#define  WDTON    4
#define  EESAVE   3
#define  BODLEVEL2 2
#define  BODLEVEL1 1
#define  BODLEVEL0 0


/* extended fuses (Z = 0x0002) */
/* for m48 */
/* #define  SELFPRGEN    0 */ /* already defined with SPMCSR */
/* for m88/168 */
/* #define  BOOTSZ1  2 */
/* #define  BOOTSZ0  1 */
/* #define  BOOTRST  0 */


/* Interrupt Vector Numbers */

#define iv_RESET        1
#define iv_INT0         2
#define iv_INT1         3
#define iv_PCINT0       4
#define iv_PCINT1       5
#define iv_PCINT2       6
#define iv_WDT          7
#define iv_TIMER2_COMPA 8
#define iv_TIMER2_COMPB 9
#define iv_TIMER2_OVF   10
#define iv_TIMER1_CAPT  11
#define iv_TIMER1_COMPA 12
#define iv_TIMER1_COMPB 13
#define iv_TIMER1_OVF   14
#define iv_TIMER0_COMPA 15
#define iv_TIMER0_COMPB 16
#define iv_TIMER0_OVF   17
#define iv_SPI_STC      18
#define iv_USART0_RX    19
#define iv_USART0_RXC   19
#define iv_USART0_DRE   20
#define iv_USART0_UDRE  20
#define iv_USART0_TX    21
#define iv_USART0_TXC   21
#define iv_ADC          22
#define iv_EE_RDY       23
#define iv_EE_READY     23
#define iv_ANA_COMP     24
#define iv_ANALOG_COMP  24
#define iv_TWI          25
#define iv_TWSI         25
#define iv_SPM_RDY      26
#define iv_SPM_READY    26

/*ja*/

#endif
