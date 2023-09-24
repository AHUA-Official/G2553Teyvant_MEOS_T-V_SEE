 $include (C8051F330.inc)

		G_LED   BIT P1.3;
		R_LED   BIT P1.2;P1.7;


		SCK     BIT P0.2;P0.4
		MISO    BIT P0.3;P0.5
		MOSI    BIT P0.4;P0.6
    CE      BIT P0.5;P0.2
		CSN     BIT P0.0;P0.3;收发模块SPI接口选择。低电平有效。
		IRQ     BIT P0.1;P1.2

		MARK       EQU 20H;接收、写入操作状态标记位。
		SIGN       EQU 21H;未用。
		STATUS     EQU 22H
		EDAD_DATA  EQU 23H
			 
		;30H,31H,32H,3H读数据地址

		HEAD1 EQU 70H;未用。
		HEAD2 EQU 71H;未用。
		HEAD3 EQU 72H;未用。
		HEAD4 EQU 73H;未用。
		HEAD5 EQU 74H;未用。
		HEAD6 EQU 75H;未用。

		D_LONG  EQU 38H;每段数据长度。
		R_DELA  EQU 39H;判数据发送完毕的延时寄存器。
		R_LONG  EQU 3AH;每段数据长度寄存器。
		R_NUM   EQU 3BH;未用。



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
    RETI
    ORG 006BH 
    RETI
    ORG 0073H 
    RETI
    ORG 0080H
START:
;------------------------------硬件配置。
    MOV PCA0MD,#00H;关看门狗。
		MOV SP,#80H;=================
    LCALL  Init_Device;配置(所有中断禁止).6.125M
    MOV IE,  #00h;关中断。
;-----------------------------

    CLR TI0
		CLR RI0
		CLR SPIF

		CLR CE
		SETB CSN
		SETB IRQ
;-----------------------------		
		CLR R_LED;上电指示。
		LCALL DELAY4;灯亮延时
KK1:
;+++++++++++++++++++++++++++++++++++++++++++++++++
    mov  SPI0CFG,   #040h;无线模块模式。
    mov  SPI0CN,    #001h;
    mov  SPI0CKR,   #000h;3.0625M
;+++++++++++++++++++++++++++++++++++++++++++++++++
    LCALL R_TXD;配置收发芯片。
		LCALL CLEAR_DATA
;-------------------------------------------------
;查询接收中断，读取数据。
;-------------------------------------------------
K3:
		;LCALL DELAY1
		CLR R_LED
    JB IRQ,$;------------$$$$$$$$$$$$$$$$$$$$$
		SETB R_LED

    LCALL RECIVE_WRITE

		MOV A,49H
		JNZ K15
		MOV A,4BH
		JNZ K15
		LJMP K3

;-------------------------------------------------
;=================================================   
;********************子程序模块*******************	
;===========================================
;===============数据接收写入================

RECIVE_WRITE:

		LJMP REC14

;----------------------接收数据/写卡。循环等待 "接收中断"和"写卡"信号。
REC13:
		CLR CSN
		MOV SPI0DAT,#017H;读FIFO_STATUS。
		JNB SPIF,$
		CLR SPIF
		MOV SPI0DAT,#017H;读FIFO_STATUS。
		JNB SPIF,$
		CLR SPIF
		MOV A,SPI0DAT
		SETB CSN
		JB ACC.0,REC14;=1空；=0有数据。
		LJMP REC1

REC14:
    JNB IRQ,REC1;$$$$$$$$$$$$$$$$$$$$$
    JB TF2H,REC6;T2 丢失数据时填充XRAM定时器。
    JNB TF0,REC14;T0 结束监视定时器。
    LJMP REC12
REC1:
    ;CPL G_LED
		LCALL READ_32BYTES;*****************
		CLR TR2
		CLR TF2H
		JB MARK.3,REC3
		;----------------开启填充定时器。
    MOV TMR2L,#00H;10.7mS
		MOV TMR2H,#00H
		SETB TR2

		LJMP REC3
;-------------------------------------------
REC12:
    CLR TF0
    DJNZ R_DELA,REC14
    SETB MARK.7
    LJMP REC7
REC6:
    CLR TR2
		CLR TF2H

    LCALL FULL_DATA
;==============================================================
REC3:
		JNB MARK.3,REC7;=1, 开始写SD卡标志。

		CLR CSN
		MOV SPI0DAT,#0E2H;冲洗接收缓冲区。
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		LJMP REC8
REC7:
    JNB MARK.7,REC13;--------数据传送"完毕"。
		SETB MARK.5
		RET
;===============================================================
REC8:
    ;JNB MISO,REC5;=1已经为本次写入准备好。
		CLR MARK.3;清除 "写卡"信号。
REC10:
;+++++++++++++++++++++++++++++++++++++++++++++++++
    mov  SPI0CFG,   #070h;SD卡模式。
    mov  SPI0CN,    #001h;
    ;mov  SPI0CKR,   #00Eh;200K
    mov  SPI0CKR,   #000h;3.0625M
;+++++++++++++++++++++++++++++++++++++++++++++++++
		LCALL WRITE_ONE_BLOCK 
		CPL G_LED
    MOV R_DELA,#60;复位延时接收参数。
;+++++++++++++++++++++++++++++++++++++++++++++++++
    mov  SPI0CFG,   #040h;无线模块模式。
    mov  SPI0CN,    #001h;
    mov  SPI0CKR,   #000h;3.0625M
;+++++++++++++++++++++++++++++++++++++++++++++++++

		;JB MARK.4,REC11;数据写入完成（MISO=1?）。

		;LJMP REC2

REC11:
    INC 48H;已写扇区计数器。
		LCALL NEXT_ADRESS;求出下一扇区字节地址。
;#########################################################
;-------------------------本次接收完成------------------
REC2:
    JNB MARK.7,REC15;判完标志。
    ;CLR TI0
    ;MOV IE,#00h;关UART中断。

		SETB MARK.5
	;	SETB BLUE_LED
		RET
REC15:
    LJMP REC13
;#########################################################
;-------------------------------------------出错提示。
REC4:;MARK.4=0,上次数据未被接受。
    MOV IE,#00h;关UART中断。
		RET
REC5:;MISO=0,设备未准备好。
    MOV IE,#00h;关UART中断。
		RET

;===========================================
;================读取RX数据===============
;===========================================
READ_32BYTES:

    MOV R2,#20H

		CLR CSN
    MOV SPI0DAT,#061H;读RX数据(读状态寄存器)。
		JNB SPIF,$
		CLR SPIF
R32_1:
    MOV SPI0DAT,#00H;读RX数据。
		JNB SPIF,$
		CLR SPIF
		MOV A,SPI0DAT
		MOVX @DPTR,A
		INC DPTR;****************
		DJNZ R2,R32_1
 		SETB CSN  
		
		CLR CSN
    MOV SPI0DAT,#027H;写STATUS。
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#7EH;清除STATUS标志;
		JNB SPIF,$
		CLR SPIF
		SETB CSN		  

    ;MOV TMR2L,#00H;257mS
		;MOV TMR2H,#00H
;--------------------------------------------收到一个数据块。
		INC 49H;
;-------------------------
		;CPL BLUE_LED;反转指示灯。
		SETB MARK.0; =1,收到一段数据。
;-------------------------
		MOV A,DPH
		CJNE A,#02H,R32_2;检查是否写完1扇区。
		MOV DPTR,#0000H
		SETB MARK.3;开始写卡标志。
		CLR MARK.0; =1,收到一段数据。
		MOV 49H,#00H
		INC 4BH
R32_2:
		RET

		;MOV 48H,#00H;已写扇区计数器。
		;MOV 49H,#00H;最后一扇区数据块的个数。
		;MOV 4BH,#00H;共收到数据扇区数。
;===========================================
;================填充XRAM数据===============
;===========================================
FULL_DATA:
    MOV A,#00H
		MOVX @DPTR,A
		INC DPTR;****************
		MOV A,DPH
		CJNE A,#02H,FULL_DATA;检查是否写完1扇区。
		MOV DPTR,#0000H
		SETB MARK.3;开始写卡标志。
		CLR MARK.0; =1,收到一段数据。
		MOV 49H,#00H
		INC 4BH

		RET

		;MOV 48H,#00H;已写扇区计数器。
		;MOV 49H,#00H;最后一扇区数据块的个数。
		;MOV 4BH,#00H;共收到数据扇区数。	
;-------------------------------------------------
;初始化配置收发寄存器。
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
    MOV SPI0DAT,#026H;
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#007H;1Mbps,0dBm。
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		nop
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
;接收配置。
;-------------------------------------------------
		CLR CSN
    MOV SPI0DAT,#020H;写CONFIG。
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#00BH;上电，接收态。
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		SETB CE

		RET
;-------------------------------------------------
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

;***********************************************
;6.125MHz工作配置。
;***********************************************
Timer_Init:
    mov  TMOD,      #001h;T0 16位,T2 16位。
    mov  CKCON,     #011h;T0 SYSCLK/4;T2 SYSYCLK.
    ret
;SPI_Init:
;    mov  SPI0CFG,   #040h
;    mov  SPI0CN,    #001h
;    mov  SPI0CKR,   #00Eh
;    ret
Port_IO_Init:
    ; P0.0  -  Skipped,     Push-Pull,  Digital
    ; P0.1  -  Skipped,     Open-Drain, Digital
    ; P0.2  -  SCK  (SPI0), Push-Pull,  Digital
    ; P0.3  -  MISO (SPI0), Open-Drain, Digital
    ; P0.4  -  MOSI (SPI0), Push-Pull,  Digital
    ; P0.5  -  Skipped,     Push-Pull,  Digital
    ; P0.6  -  Skipped,     Push-Pull,  Digital
    ; P0.7  -  Unassigned,  Open-Drain, Digital

    ; P1.0  -  Unassigned,  Open-Drain, Digital
    ; P1.1  -  Unassigned,  Open-Drain, Digital
    ; P1.2  -  Skipped,     Open-Drain, Digital
    ; P1.3  -  Skipped,     Open-Drain, Digital
    ; P1.4  -  Unassigned,  Open-Drain, Digital
    ; P1.5  -  Unassigned,  Open-Drain, Digital
    ; P1.6  -  Unassigned,  Open-Drain, Digital
    ; P1.7  -  Unassigned,  Open-Drain, Digital

    mov  P0MDOUT,   #075h
    mov  P0SKIP,    #063h
    mov  P1SKIP,    #00Ch
    mov  XBR0,      #002h
    mov  XBR1,      #040h
    ret

Oscillator_Init:
    mov  OSCICN,    #081h;6.125M
    ret

; Initialization function for device,
; Call Init_Device from your main program
Init_Device:
    lcall Timer_Init
    ;lcall SPI_Init
    lcall Port_IO_Init
    lcall Oscillator_Init
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

DELAY9:
	MOV	R6,#150
DEL97:
	MOV	R7,#0FFH
DEL98:
	DJNZ	R7,DEL98
	DJNZ	R6,DEL97
	RET
;===========================================
END