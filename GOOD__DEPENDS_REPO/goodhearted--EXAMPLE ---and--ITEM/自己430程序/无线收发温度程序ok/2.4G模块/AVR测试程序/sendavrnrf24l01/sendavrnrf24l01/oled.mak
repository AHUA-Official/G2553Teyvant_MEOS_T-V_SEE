CC = iccavr
CFLAGS =  -ID:\icc\include\ -e -D__ICC_VERSION="7.08" -D_EE_EXTIO -DATMega48  -l -g -Mavr_enhanced_small 
ASFLAGS = $(CFLAGS)  -Wa-g
LFLAGS =  -LD:\icc\lib\ -g -bfunc_lit:0x34.0x1000 -dram_end:0x2ff -bdata:0x100.0x2ff -dhwstk_size:16 -beeprom:0.256 -fihx_coff -S2
FILES = main.o 

OLED:	$(FILES)
	$(CC) -o OLED $(LFLAGS) @OLED.lk  
main.o: E:\sendavrnrf24l01\iom48v.h E:\sendavrnrf24l01\macros.h E:\sendavrnrf24l01\defs.h
main.o:	E:\sendavrnrf24l01\main.c
	$(CC) -c $(CFLAGS) E:\sendavrnrf24l01\main.c
