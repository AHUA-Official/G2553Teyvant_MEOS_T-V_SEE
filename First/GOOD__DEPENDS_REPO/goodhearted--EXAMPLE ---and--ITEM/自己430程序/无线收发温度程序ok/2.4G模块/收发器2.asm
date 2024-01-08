 $include (C8051F330.inc)

		G_LED   BIT P1.3;
		R_LED   BIT P1.2;P1.7;


		SCK     BIT P0.2;P0.4
		MISO    BIT P0.3;P0.5
		MOSI    BIT P0.4;P0.6
    CE      BIT P0.5;P0.2
		CSN     BIT P0.0;P0.3;�շ�ģ��SPI�ӿ�ѡ�񡣵͵�ƽ��Ч��
		IRQ     BIT P0.1;P1.2

		MARK       EQU 20H;���ա�д�����״̬���λ��
		SIGN       EQU 21H;δ�á�
		STATUS     EQU 22H
		EDAD_DATA  EQU 23H
			 
		;30H,31H,32H,3H�����ݵ�ַ

		HEAD1 EQU 70H;δ�á�
		HEAD2 EQU 71H;δ�á�
		HEAD3 EQU 72H;δ�á�
		HEAD4 EQU 73H;δ�á�
		HEAD5 EQU 74H;δ�á�
		HEAD6 EQU 75H;δ�á�

		D_LONG  EQU 38H;ÿ�����ݳ��ȡ�
		R_DELA  EQU 39H;�����ݷ�����ϵ���ʱ�Ĵ�����
		R_LONG  EQU 3AH;ÿ�����ݳ��ȼĴ�����
		R_NUM   EQU 3BH;δ�á�



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
;------------------------------Ӳ�����á�
    MOV PCA0MD,#00H;�ؿ��Ź���
		MOV SP,#80H;=================
    LCALL  Init_Device;����(�����жϽ�ֹ).6.125M
    MOV IE,  #00h;���жϡ�
;-----------------------------

    CLR TI0
		CLR RI0
		CLR SPIF

		CLR CE
		SETB CSN
		SETB IRQ
;-----------------------------		
		CLR R_LED;�ϵ�ָʾ��
		LCALL DELAY4;������ʱ
KK1:
;+++++++++++++++++++++++++++++++++++++++++++++++++
    mov  SPI0CFG,   #040h;����ģ��ģʽ��
    mov  SPI0CN,    #001h;
    mov  SPI0CKR,   #000h;3.0625M
;+++++++++++++++++++++++++++++++++++++++++++++++++
    LCALL R_TXD;�����շ�оƬ��
		LCALL CLEAR_DATA
;-------------------------------------------------
;��ѯ�����жϣ���ȡ���ݡ�
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
;********************�ӳ���ģ��*******************	
;===========================================
;===============���ݽ���д��================

RECIVE_WRITE:

		LJMP REC14

;----------------------��������/д����ѭ���ȴ� "�����ж�"��"д��"�źš�
REC13:
		CLR CSN
		MOV SPI0DAT,#017H;��FIFO_STATUS��
		JNB SPIF,$
		CLR SPIF
		MOV SPI0DAT,#017H;��FIFO_STATUS��
		JNB SPIF,$
		CLR SPIF
		MOV A,SPI0DAT
		SETB CSN
		JB ACC.0,REC14;=1�գ�=0�����ݡ�
		LJMP REC1

REC14:
    JNB IRQ,REC1;$$$$$$$$$$$$$$$$$$$$$
    JB TF2H,REC6;T2 ��ʧ����ʱ���XRAM��ʱ����
    JNB TF0,REC14;T0 �������Ӷ�ʱ����
    LJMP REC12
REC1:
    ;CPL G_LED
		LCALL READ_32BYTES;*****************
		CLR TR2
		CLR TF2H
		JB MARK.3,REC3
		;----------------������䶨ʱ����
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
		JNB MARK.3,REC7;=1, ��ʼдSD����־��

		CLR CSN
		MOV SPI0DAT,#0E2H;��ϴ���ջ�������
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		LJMP REC8
REC7:
    JNB MARK.7,REC13;--------���ݴ���"���"��
		SETB MARK.5
		RET
;===============================================================
REC8:
    ;JNB MISO,REC5;=1�Ѿ�Ϊ����д��׼���á�
		CLR MARK.3;��� "д��"�źš�
REC10:
;+++++++++++++++++++++++++++++++++++++++++++++++++
    mov  SPI0CFG,   #070h;SD��ģʽ��
    mov  SPI0CN,    #001h;
    ;mov  SPI0CKR,   #00Eh;200K
    mov  SPI0CKR,   #000h;3.0625M
;+++++++++++++++++++++++++++++++++++++++++++++++++
		LCALL WRITE_ONE_BLOCK 
		CPL G_LED
    MOV R_DELA,#60;��λ��ʱ���ղ�����
;+++++++++++++++++++++++++++++++++++++++++++++++++
    mov  SPI0CFG,   #040h;����ģ��ģʽ��
    mov  SPI0CN,    #001h;
    mov  SPI0CKR,   #000h;3.0625M
;+++++++++++++++++++++++++++++++++++++++++++++++++

		;JB MARK.4,REC11;����д����ɣ�MISO=1?����

		;LJMP REC2

REC11:
    INC 48H;��д������������
		LCALL NEXT_ADRESS;�����һ�����ֽڵ�ַ��
;#########################################################
;-------------------------���ν������------------------
REC2:
    JNB MARK.7,REC15;�����־��
    ;CLR TI0
    ;MOV IE,#00h;��UART�жϡ�

		SETB MARK.5
	;	SETB BLUE_LED
		RET
REC15:
    LJMP REC13
;#########################################################
;-------------------------------------------������ʾ��
REC4:;MARK.4=0,�ϴ�����δ�����ܡ�
    MOV IE,#00h;��UART�жϡ�
		RET
REC5:;MISO=0,�豸δ׼���á�
    MOV IE,#00h;��UART�жϡ�
		RET

;===========================================
;================��ȡRX����===============
;===========================================
READ_32BYTES:

    MOV R2,#20H

		CLR CSN
    MOV SPI0DAT,#061H;��RX����(��״̬�Ĵ���)��
		JNB SPIF,$
		CLR SPIF
R32_1:
    MOV SPI0DAT,#00H;��RX���ݡ�
		JNB SPIF,$
		CLR SPIF
		MOV A,SPI0DAT
		MOVX @DPTR,A
		INC DPTR;****************
		DJNZ R2,R32_1
 		SETB CSN  
		
		CLR CSN
    MOV SPI0DAT,#027H;дSTATUS��
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#7EH;���STATUS��־;
		JNB SPIF,$
		CLR SPIF
		SETB CSN		  

    ;MOV TMR2L,#00H;257mS
		;MOV TMR2H,#00H
;--------------------------------------------�յ�һ�����ݿ顣
		INC 49H;
;-------------------------
		;CPL BLUE_LED;��תָʾ�ơ�
		SETB MARK.0; =1,�յ�һ�����ݡ�
;-------------------------
		MOV A,DPH
		CJNE A,#02H,R32_2;����Ƿ�д��1������
		MOV DPTR,#0000H
		SETB MARK.3;��ʼд����־��
		CLR MARK.0; =1,�յ�һ�����ݡ�
		MOV 49H,#00H
		INC 4BH
R32_2:
		RET

		;MOV 48H,#00H;��д������������
		;MOV 49H,#00H;���һ�������ݿ�ĸ�����
		;MOV 4BH,#00H;���յ�������������
;===========================================
;================���XRAM����===============
;===========================================
FULL_DATA:
    MOV A,#00H
		MOVX @DPTR,A
		INC DPTR;****************
		MOV A,DPH
		CJNE A,#02H,FULL_DATA;����Ƿ�д��1������
		MOV DPTR,#0000H
		SETB MARK.3;��ʼд����־��
		CLR MARK.0; =1,�յ�һ�����ݡ�
		MOV 49H,#00H
		INC 4BH

		RET

		;MOV 48H,#00H;��д������������
		;MOV 49H,#00H;���һ�������ݿ�ĸ�����
		;MOV 4BH,#00H;���յ�������������	
;-------------------------------------------------
;��ʼ�������շ��Ĵ�����
;-------------------------------------------------
R_TXD:
		CLR CSN
    MOV SPI0DAT,#031H;д���� 0ͨ�� �ֽ�����
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#020H;32bytes
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		nop

		CLR CSN 
    MOV SPI0DAT,#032H;д 1ͨ�� �ֽ�����
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
    MOV SPI0DAT,#007H;1Mbps,0dBm��
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		nop
		nop

		CLR CSN 
    MOV SPI0DAT,#02AH;д 0ͨ�� 5�ֽ�=����=��ַD8D8D8D8D8��
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
    MOV SPI0DAT,#030H;д  5�ֽ�=����=��ַD8D8D8D8D8��
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
    MOV SPI0DAT,#025H;�趨����Ƶ����
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#020H;32Ƶ��
		JNB SPIF,$
		CLR SPIF
		SETB CSN
		nop
;-------------------------------------------------
;�������á�
;-------------------------------------------------
		CLR CSN
    MOV SPI0DAT,#020H;дCONFIG��
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#00BH;�ϵ磬����̬��
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		SETB CE

		RET
;-------------------------------------------------
;-------------------------------------------------
;����ж�Դ���ͷ��仺������
;----------------------------------
CLEAR_DATA:
		CLR CSN
    MOV SPI0DAT,#027H;дSTATUS��
		JNB SPIF,$
		CLR SPIF
    MOV SPI0DAT,#7EH;���STATUS��־;
		JNB SPIF,$
		CLR SPIF
		SETB CSN
		NOP

		CLR CSN
		MOV SPI0DAT,#0E1H;��ϴ���仺������
		JNB SPIF,$
		CLR SPIF
		SETB CSN

		RET
;----------------------------------

;***********************************************
;6.125MHz�������á�
;***********************************************
Timer_Init:
    mov  TMOD,      #001h;T0 16λ,T2 16λ��
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
;===  ��ʱ�ӳ���  ===
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
DELAY3:;R5=3,ÿ����д�����ʱ�ȴ���
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