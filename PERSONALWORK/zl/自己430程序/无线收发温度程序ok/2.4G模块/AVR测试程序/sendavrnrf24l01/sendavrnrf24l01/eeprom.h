/*
**  Header file : eeprom.h
*/
#ifdef _AVR
/* AVR EEPROM access functions */
#ifndef		__EEPROM__H
#define		__EEPROM__H

/* These defines are passed by the IDE as appropriate */
#if defined(ATMega169) || defined(ATMega48) || defined(ATMega88) || defined(ATMega168)	|| defined(ATMega165)
#define EEPROMread		EEPROMread_extIO
#define EEPROMwrite		EEPROMwrite_extIO
#define EEPROMReadBytes		EEPROMReadBytes_extIO
#define EEPROMWriteBytes	EEPROMWriteBytes_extIO

#elif defined(ATtiny13) || defined(ATtiny2313) || defined(ATtiny26) || defined(AT90S2313) || defined(AT90S2323) || defined(AT90S2333) || defined(AT90S2343) || defined(AT90S4414) || defined(AT90S4433) || defined(AT90S4434)

#define EEPROMread		EEPROMread_256
#define EEPROMwrite		EEPROMwrite_256
#define EEPROMReadBytes		EEPROMReadBytes_256
#define EEPROMWriteBytes	EEPROMWriteBytes_256
#endif

#define _256EEPROMread	EEPROMread_256
#define _256EEPROMwrite	EEPROMwrite_256

int EEPROMwrite( int location, unsigned char);
unsigned char EEPROMread( int);

void EEPROMReadBytes(int addr, void *ptr, int size);
void EEPROMWriteBytes(int addr, void *ptr, int size);

#define EEPROM_READ(addr, dst)		EEPROMReadBytes(addr, &dst, sizeof (dst))
#define EEPROM_WRITE(addr, src)		EEPROMWriteBytes(addr, &src, sizeof (src))

#endif
#endif
