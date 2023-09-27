
 $include (C8051F330.inc)

		CLK     BIT P0.0
		MISO    BIT P0.1
		MOSI    BIT P0.2
    ;CE     BIT P1.5;+
		CSN     BIT P1.5;P1.7;
		IRQ     BIT P1.3;P0.3;

		STATUS  EQU 20H
		R_LONG1 EQU 21H
		R_DATA  EQU 22H
		R_LONG2 EQU 23H

    ORG 0000H 
    LJMP START
    ORG 0003H 
    RETI

    ORG 000BH
    RETI

    ORG 0013H 
    RETI
    ORG 001BH 
    RETI
    ORG 0023H 
    LCALL UART_INT
    RETI
    ORG 002BH 
    RETI
    ORG 0033H 
    RETI
    ORG 003BH 
    RETI
    ORG 0043H 
    RETI
    ORG 004BH 
    RETI
    ORG 0053H 
    RETI
    ORG 005BH 
    RETI
    ORG 0063H 
		;mov ie,#00h
    RETI
    ORG 006BH 
    RETI
    ORG 0073H 
    RETI
    ORG 0080H
START:
;------------------------------硬件配置。
    MOV PCA0MD,#00H;关看门狗。
		MOV SP,#080H;=================
    LCALL  Init_Device;配置UART,SPI。(允许UART中断).3.0625M

    CLR TR0
		CLR TF0
		MOV TL0,#00H;复位结束监视定时器。
		MOV TH0,#00H

;-----------------------------
    CLR TI0
		CLR RI0
		CLR SPIF
		;SETB CE;
		CLR F0;开始发送数据标志。
		;CLR F1;数据发送进行中标志。
		;MOV R_LONG2,#00H
		MOV DPTR,#0000H
		
		LCALL DELAY4;上电延时等待无线模块。
		LCALL R_TXD;初始化配置寄存器。
		LCALL CLEAR_DATA;清除中断源，和发射缓冲区。
		SETB TR0
STAR1:
		JB F0,STAR2;*****************等待中断/发送数据。
		JNB TF0,STAR1
		;CLR TF0
		;JNB F1,STAR1
		CLR TF0
		MOV DPTR,#0000H
		LJMP STAR1
STAR2:
		LCALL TXD_DATA;发射数据。发射位于XRAM中的512字节数据。

		;INC R_LONG2
		;MOV SBUF0,R_LONG2
		;JNB TI0,$
		;CLR TI0

		;CLR F1

		LJMP STAR1

;=================================================   
;********************子程序模块*******************		
;-------------------------------------------------
;发射数据。发射位于XRAM中的512字节数据。
;-------------------------------------------------
TXD_DATA:

    MOV R2,#20H;数据包长度。
		MOV R1,#00H;
		
		MOV EMI0CN,#00H;在第0页操作数据。
TXD1:
		CLR CSN
    MOV SPI0DAT,#0A0H;写TX数据。
		JNB SPIF,$
		CLR SPIF
TXD2:
    MOVX A,@R1
    MOV SPI0DAT,A
		JNB SPIF,$
		CLR SPIF
    INC R1
		DJNZ R2,TXD2
 		SETB CSN

		;SETB CE
		JB IRQ,$
		;CLR CE

		LCALL CLEAR_DATA

    MOV R2,#20H;数据包长度。
		CJNE R1,#00H,TXD1
;=========================================
		MOV EMI0CN,#01H;在第1页操作数据。
TXD3:
		CLR CSN
    MOV SPI0DAT,#0A0H;写TX数据。
		JNB SPIF,$
		CLR SPIF
TXD4:
    MOVX A,@R1
    MOV SPI0DAT,A
		JNB SPIF,$
		CLR SPIF
    INC R1
		DJNZ R2,TXD4
 		SETB CSN

		;SETB CE
		JB IRQ,$
		;CLR CE

		LCALL CLEAR_DATA

    MOV R2,#20H;数据包长度。
		CJNE R1,#00H,TXD3

		CLR F0

		RET
;-------------------------------------------------

;-------------------------------------------------
;读状态寄存器。
;-------------------------------------------------
READ_STATUS:
		CLR CSN
    MOV SPI0DAT,#0FFH;读状态寄存器。
		JNB SPIF,$
		CLR SPIF
		MOV STATUS,SPI0DAT
 		SETB CSN

		MOV SBUF0,STATUS;-------
		JNB TI0,$;-------------
		CLR TI0;---------------

		RET
;-------------------------------------------------
;清除中断源，和发射缓冲区。
;----------------------------------
CLEAR_DATA:
		CLR CSN
    MOV SPI0DAT,#027H;写STATUS。
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#7EH;清除STATUS标志;
		JNB SPIF,$
		CLR SPIF
		SETB CSN
		NOP

		CLR CSN
		MOV SPI0DAT,#0E1H;冲洗发射缓冲区。
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		RET
;----------------------------------
;-------------------------------------------------
;串口中断处理。
;-------------------------------------------------
UART_INT:
    MOV R_DATA,A;保留A值。
    MOV A,SBUF0
		MOVX @DPTR,A
		CLR RI0
		INC DPTR
		MOV A,DPH
		CJNE A,#02H,UA_IN1
		MOV DPTR,#0000H
		SETB F0;开始发送数据标志。
UA_IN1:
    MOV A,R_DATA;恢复A值。
		MOV TL0,#00H;复位结束监视定时器。
		MOV TH0,#00H
		;SETB F1
		RET   
;-------------------------------------------------
;初始化配置寄存器。
;-------------------------------------------------
R_TXD:
		CLR CSN
    MOV SPI0DAT,#031H;写接收 0通道 字节数。
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#020H;32bytes
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		nop

		CLR CSN 
    MOV SPI0DAT,#032H;写 1通道 字节数。
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#020H;32bytes
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		nop

		CLR CSN
    MOV SPI0DAT,#024H;
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#002H;重新发送 2 。
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		nop
		nop

		CLR CSN
    MOV SPI0DAT,#026H;
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#007H;1Mbps,0dBm。
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		nop

		CLR CSN 
    MOV SPI0DAT,#02AH;写 0通道 5字节=接收=地址D8D8D8D8D8。
		JNB SPIF,$
		CLR SPIF
		MOV R1,#5
R_T1:
    MOV SPI0DAT,#0D8H;
		JNB SPIF,$
		CLR SPIF
		DJNZ R1,R_T1
		SETB CSN
		nop

		CLR CSN 
    MOV SPI0DAT,#030H;写  5字节=发送=地址D8D8D8D8D8。
		JNB SPIF,$
		CLR SPIF
		MOV R1,#5
R_T2:
    MOV SPI0DAT,#0D8H;
		JNB SPIF,$
		CLR SPIF
		DJNZ R1,R_T2
		SETB CSN
		nop

		CLR CSN 
    MOV SPI0DAT,#025H;设定工作频道。
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#020H;32频道
		JNB SPIF,$
		CLR SPIF
		SETB CSN
		nop
;-------------------------------------------------
;发射配置。
;-------------------------------------------------
		CLR CSN
    MOV SPI0DAT,#020H;写CONFIG。
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#00AH;上电，发射态。
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		RET
;===========================================

;-------------------------------------------------
;***********************************************
;3.0625MHz工作配置。
;***********************************************
Timer_Init:
    mov  TCON,      #050h;T0,T1运行控制。
    mov  TMOD,      #021h
    mov  CKCON,     #008h;08H=19200;115200;01H=4800;
    mov  TH1,       #0E5h;F3=115200;E5=57600
    ret

UART_Init:
    mov  SCON0,     #010h
    ret

SPI_Init:
    mov  SPI0CFG,   #040h
    mov  SPI0CN,    #001h
    ret
Port_IO_Init:
    ; P0.0  -  SCK  (SPI0), Push-Pull,  Digital
    ; P0.1  -  MISO (SPI0), Open-Drain, Digital
    ; P0.2  -  MOSI (SPI0), Push-Pull,  Digital
    ; P0.3  -  Skipped,     Open-Drain, Digital
    ; P0.4  -  TX0 (UART0), Push-Pull,  Digital
    ; P0.5  -  RX0 (UART0), Open-Drain, Digital
    ; P0.6  -  Unassigned,  Open-Drain, Digital
    ; P0.7  -  Unassigned,  Open-Drain, Digital

    ; P1.0  -  Unassigned,  Open-Drain, Digital
    ; P1.1  -  Unassigned,  Open-Drain, Digital
    ; P1.2  -  Unassigned,  Open-Drain, Digital
    ; P1.3  -  Skipped,     Open-Drain, Digital
    ; P1.4  -  Unassigned,  Open-Drain, Digital
    ; P1.5  -  Skipped,     Push-Pull,  Digital
    ; P1.6  -  Unassigned,  Open-Drain, Digital
    ; P1.7  -  Skipped,     Push-Pull,  Digital

    mov  P0MDOUT,   #015h
    mov  P1MDOUT,   #0A0h
    mov  P0SKIP,    #008h
    mov  P1SKIP,    #0A8h
    mov  XBR0,      #003h
    mov  XBR1,      #040h
    ret
Interrupts_Init:
    mov  IE,        #090h
    ret

; Initialization function for device,
; Call Init_Device from your main program
Init_Device:
    lcall Timer_Init
    lcall UART_Init
    lcall SPI_Init
    lcall Port_IO_Init
    lcall Interrupts_Init
    ret
;===========================================
;===  延时子程序  ===
DELAY4:
  MOV R5,#20
DEL11:
	MOV	R6,#250
DEL10:
	MOV	R7,#0FFH
DEL9:
	DJNZ	R7,DEL9
	DJNZ	R6,DEL10
	DJNZ  R5,DEL11
	RET

DELAY3:;R5=3,每扇区写间的延时等待。
  MOV R5,#3
DEL6:
	MOV	R6,#250
DEL5:
	MOV	R7,#0FFH
DEL4:
	DJNZ	R7,DEL4
	DJNZ	R6,DEL5
	DJNZ  R5,DEL6
	RET

DELAY2:
  MOV R5,#1
DEL3:
	MOV	R6,#200
DEL2:
	MOV	R7,#0FFH
DEL1:
	DJNZ	R7,DEL1
	DJNZ	R6,DEL2
	DJNZ  R5,DEL3
	RET

DELAY1:
	MOV	R6,#60;=60,1.275mS
DEL7:
	MOV	R7,#0FFH
DEL8:
	DJNZ	R7,DEL8
	DJNZ	R6,DEL7
	RET
;===========================================
END