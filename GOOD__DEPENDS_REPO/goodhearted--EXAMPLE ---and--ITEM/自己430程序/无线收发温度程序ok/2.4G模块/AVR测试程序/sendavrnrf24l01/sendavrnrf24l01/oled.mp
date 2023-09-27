NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   0034   0048 =     72. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0034  __idata_start
       007C  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   007C   0346 =    838. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       003E  __start
       003E  __text_start
       005D  _exit
       005E  _main
       00D8  _init_CPU
       00F9  _delay
       0100  _delayms
       0107  _clear_buf
       0111  _SPI_RW
       0128  _SPI_RW_Reg
       0136  _SPI_Read
       0146  _SPI_Read_Buf
       015F  _SPI_Write_Buf
       0176  _RX_Mode
       0195  _TX_Mode
       01C0  pop_xgset003C
       01C5  pop_xgset00FC
       01CC  push_xgsetF0FC
       01CE  push_xgset30FC
       01D0  push_xgset00FC
       01D2  push_xgset003C
       01D7  push_xgsetF000
       01DC  pop_xgsetF000
       01E1  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0100   0048 =     72. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0100  _Buffer
       0100  __data_start
       0140  _TX_ADDRESS
       0145  _accept_flag
       0146  _accept_time
       0148  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   0148   0001 =      1. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0148  _key_debug
       0148  __bss_start
       0149  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0002 =      2. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\iccv7avr\lib\crtAVR.o	[ crtavr.s ]
main.o	[ main.c ]
<library>	[ gpop2.s, gpop3.s, gpush.s, gpushy.s ]

User Global Definitions

ram_end = 0x2ff
hwstk_size = 0x10

User Base Address Definitions

func_lit = 0x34
data = 0x100
eeprom:0.256

