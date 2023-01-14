;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"Lab1.c"
	list	p=12f675
	radix dec
	include "p12f675.inc"
;--------------------------------------------------------
; config word(s)
;--------------------------------------------------------
	__config 0x3fd4
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_ANSEL
	extern	_TRISIO
	extern	_GPIO
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_delay

	global PSAVE
	global SSAVE
	global WSAVE
	global STK12
	global STK11
	global STK10
	global STK09
	global STK08
	global STK07
	global STK06
	global STK05
	global STK04
	global STK03
	global STK02
	global STK01
	global STK00

sharebank udata_ovr 0x0020
PSAVE	res 1
SSAVE	res 1
WSAVE	res 1
STK12	res 1
STK11	res 1
STK10	res 1
STK09	res 1
STK08	res 1
STK07	res 1
STK06	res 1
STK05	res 1
STK04	res 1
STK03	res 1
STK02	res 1
STK01	res 1
STK00	res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_Lab1_0	udata
r0x1008	res	1
r0x1009	res	1
r0x100A	res	1
r0x100B	res	1
r0x1001	res	1
r0x1000	res	1
r0x1002	res	1
r0x1003	res	1
r0x1004	res	1
r0x1005	res	1
r0x1006	res	1
r0x1007	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------
;--------------------------------------------------------
; initialized absolute data
;--------------------------------------------------------
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
STARTUP	code 0x0000
	nop
	pagesel __sdcc_gsinit_startup
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_Lab1	code
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;5 compiler assigned registers:
;   r0x1008
;   r0x1009
;   r0x100A
;   r0x100B
;   STK00
;; Starting pCode block
S_Lab1__main	code
_main:
; 2 exit points
;	.line	10; "Lab1.c"	ANSEL=0b00000000;
	BANKSEL	_ANSEL
	CLRF	_ANSEL
;	.line	11; "Lab1.c"	TRISIO = 0b00100000; //Poner todos los pines como salidas
	MOVLW	0x20
	MOVWF	_TRISIO
;	.line	12; "Lab1.c"	GPIO = 0x00000000; //Poner pines en bajo
	BANKSEL	_GPIO
	CLRF	_GPIO
;	.line	15; "Lab1.c"	int timer = 0;
	CLRF	r0x1008
	CLRF	r0x1009
_00122_DS_:
;	.line	20; "Lab1.c"	timer = timer+1;
	INCF	r0x1008,F
	BTFSC	STATUS,2
	INCF	r0x1009,F
;	.line	21; "Lab1.c"	if (timer == 6){
	MOVF	r0x1008,W
	XORLW	0x06
	BTFSS	STATUS,2
	GOTO	_00106_DS_
	MOVF	r0x1009,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00106_DS_
;	.line	22; "Lab1.c"	timer =0;
	CLRF	r0x1008
	CLRF	r0x1009
_00106_DS_:
;	.line	24; "Lab1.c"	if(GPIO==0b00100000){
	BANKSEL	_GPIO
	MOVF	_GPIO,W
	XORLW	0x20
	BTFSS	STATUS,2
	GOTO	_00122_DS_
;	.line	25; "Lab1.c"	cont = timer;
	MOVF	r0x1008,W
	MOVWF	r0x100A
	MOVF	r0x1009,W
	MOVWF	r0x100B
;	.line	26; "Lab1.c"	if(cont==1){
	MOVF	r0x1008,W
	XORLW	0x01
	BTFSS	STATUS,2
	GOTO	_00108_DS_
	MOVF	r0x1009,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00108_DS_
;	.line	27; "Lab1.c"	GPIO=0b00000001;
	MOVLW	0x01
	MOVWF	_GPIO
;	.line	28; "Lab1.c"	delay(time);
	MOVLW	0xfa
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	29; "Lab1.c"	GPIO=0b00000000;
	BANKSEL	_GPIO
	CLRF	_GPIO
_00108_DS_:
;	.line	31; "Lab1.c"	if(cont == 2){
	MOVF	r0x100A,W
	XORLW	0x02
	BTFSS	STATUS,2
	GOTO	_00110_DS_
	MOVF	r0x100B,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00110_DS_
;	.line	32; "Lab1.c"	GPIO=0b00000010;
	MOVLW	0x02
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	33; "Lab1.c"	delay(time);
	MOVLW	0xfa
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	34; "Lab1.c"	GPIO=0b00000000;
	BANKSEL	_GPIO
	CLRF	_GPIO
_00110_DS_:
;	.line	36; "Lab1.c"	if(cont == 3){
	MOVF	r0x100A,W
	XORLW	0x03
	BTFSS	STATUS,2
	GOTO	_00112_DS_
	MOVF	r0x100B,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00112_DS_
;	.line	37; "Lab1.c"	GPIO=0b00000100;
	MOVLW	0x04
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	38; "Lab1.c"	delay(time);
	MOVLW	0xfa
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	39; "Lab1.c"	GPIO=0b00000000;
	BANKSEL	_GPIO
	CLRF	_GPIO
_00112_DS_:
;	.line	41; "Lab1.c"	if(cont == 4){
	MOVF	r0x100A,W
	XORLW	0x04
	BTFSS	STATUS,2
	GOTO	_00114_DS_
	MOVF	r0x100B,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00114_DS_
;	.line	42; "Lab1.c"	GPIO=0b00000101;
	MOVLW	0x05
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	43; "Lab1.c"	delay(time);
	MOVLW	0xfa
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	44; "Lab1.c"	GPIO=0b00000000;
	BANKSEL	_GPIO
	CLRF	_GPIO
_00114_DS_:
;	.line	46; "Lab1.c"	if(cont == 5){
	MOVF	r0x100A,W
	XORLW	0x05
	BTFSS	STATUS,2
	GOTO	_00116_DS_
	MOVF	r0x100B,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00116_DS_
;	.line	47; "Lab1.c"	GPIO=0b00000110;
	MOVLW	0x06
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	48; "Lab1.c"	delay(time);
	MOVLW	0xfa
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	49; "Lab1.c"	GPIO=0b00000000;
	BANKSEL	_GPIO
	CLRF	_GPIO
_00116_DS_:
;	.line	51; "Lab1.c"	if(cont == 6){
	MOVF	r0x100A,W
	XORLW	0x06
	BTFSS	STATUS,2
	GOTO	_00122_DS_
	MOVF	r0x100B,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00122_DS_
;	.line	52; "Lab1.c"	GPIO=0b00000111;
	MOVLW	0x07
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	53; "Lab1.c"	delay(time);
	MOVLW	0xfa
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	54; "Lab1.c"	GPIO=0b00000000;
	BANKSEL	_GPIO
	CLRF	_GPIO
	GOTO	_00122_DS_
;	.line	59; "Lab1.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;9 compiler assigned registers:
;   r0x1000
;   STK00
;   r0x1001
;   r0x1002
;   r0x1003
;   r0x1004
;   r0x1005
;   r0x1006
;   r0x1007
;; Starting pCode block
S_Lab1__delay	code
_delay:
; 2 exit points
;	.line	61; "Lab1.c"	void delay(unsigned int tiempo)
	MOVWF	r0x1000
	MOVF	STK00,W
	MOVWF	r0x1001
;	.line	66; "Lab1.c"	for(i=0;i<tiempo;i++)
	CLRF	r0x1002
	CLRF	r0x1003
_00133_DS_:
	MOVF	r0x1000,W
	SUBWF	r0x1003,W
	BTFSS	STATUS,2
	GOTO	_00154_DS_
	MOVF	r0x1001,W
	SUBWF	r0x1002,W
_00154_DS_:
	BTFSC	STATUS,0
	GOTO	_00135_DS_
;;genSkipc:3307: created from rifx:0x7ffd4eaf2100
;	.line	67; "Lab1.c"	for(j=0;j<1275;j++);
	MOVLW	0xfb
	MOVWF	r0x1004
	MOVLW	0x04
	MOVWF	r0x1005
_00131_DS_:
	MOVLW	0xff
	ADDWF	r0x1004,W
	MOVWF	r0x1006
	MOVLW	0xff
	MOVWF	r0x1007
	MOVF	r0x1005,W
	BTFSC	STATUS,0
	INCFSZ	r0x1005,W
	ADDWF	r0x1007,F
	MOVF	r0x1006,W
	MOVWF	r0x1004
	MOVF	r0x1007,W
	MOVWF	r0x1005
	MOVF	r0x1007,W
	IORWF	r0x1006,W
	BTFSS	STATUS,2
	GOTO	_00131_DS_
;	.line	66; "Lab1.c"	for(i=0;i<tiempo;i++)
	INCF	r0x1002,F
	BTFSC	STATUS,2
	INCF	r0x1003,F
	GOTO	_00133_DS_
_00135_DS_:
;	.line	68; "Lab1.c"	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  158+   26 =   184 instructions (  420 byte)

	end
