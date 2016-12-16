.INCLUDE "usb1286def.inc"
.ORG 0x00

RJMP	START

MSG1:	.DB		"Walker Arce", $00
MSG2:	.DB		"Graduated in 2015", $00

START:
.EQU	LCD_DPRT = PORTB
.EQU	LCD_DDDR = DDRB
.EQU	LCD_DPIN = PINB
.EQU	LCD_CPRT = PORTD
.EQU	LCD_CDDR = DDRD
.EQU	LCD_CPIN = PIND
.EQU	LCD_RS = 6
.EQU	LCD_RW = 4
.EQU	LCD_EN = 5

		;set up the stack
		LDI 		R25,HIGH(RAMEND)	
		OUT 		SPH,R25
		LDI 		R25,LOW(RAMEND)
		OUT 		SPL,R25

        	CALL		LCD_init
	   	LDI		XL, LOW( MSG1<< 1 )
		LDI		XH, HIGH( MSG1<< 1 )
        	CALL		LCD_string
		LDI 		R16, $C0
		CALL 		LCD_cmd
        	LDI		XL, LOW( MSG2 << 1 )
		LDI		XH, HIGH( MSG2 << 1 )
        	CALL		LCD_string
HERE:		RJMP		HERE

LCD_init:
		LDI 		R16, 0xFF
		OUT 		DDRD, R16
		OUT 		DDRB, R16

		CALL 		DELAY_15ms
        	CBI 		LCD_CPRT,5		; Set E pin initial state.
		CBI 		LCD_CPRT,4		; Set R/W pin initial state.
			 
		CALL		DELAY_5ms
		LDI		R16, 0x30
		CALL		LCD_cmd

		CALL 		DELAY_5ms		;Delay 4.1 milliseconds.
		LDI 		R16, 0x30
		CALL		LCD_cmd

		CALL 		DELAY_100us		; Delay 100us (1ms)
		LDI		R16, 0x30
		CALL		LCD_cmd

		LDI		R16, 0x3C
		CALL		LCD_cmd

		LDI		R16, 0x08
		CALL		LCD_cmd

		LDI		R16, 0x06
		CALL		LCD_cmd

		LDI		R16, 0x0F
		CALL 		LCD_cmd
		CALL		LCD_clear
		RET

LCD_clear:
		LDI		R16, 0x01
		CALL		LCD_cmd
		LDI		R16, 15
		CALL		DELAY
		RET

LCD_cmd:
       	OUT		LCD_DPRT, R16
		CBI		LCD_CPRT, LCD_RS
		CALL 		DELAY_5us
		SBI		LCD_CPRT, LCD_EN
		CALL		DELAY_5us
		CBI		LCD_CPRT, LCD_EN
		CALL		DELAY_100us
		RET

LCD_data:
        	OUT		LCD_DPRT, R16
		SBI		LCD_CPRT, LCD_RS
		CALL 		DELAY_5us
		SBI		LCD_CPRT, LCD_EN
		CALL		DELAY_5us
		CBI		LCD_CPRT, LCD_EN
		CALL		DELAY_100us
		RET

LCD_char:
        	CALL		LCD_data
		RET

LCD_string:
		MOV		ZL, XL
		MOV		ZH, XH
LO:		LPM		R16, Z+
		CPI		R16, 0
		BREQ   	DONE
		CALL		LCD_char
		RJMP   	LO
DONE:
		RET

;---------
DELAY_5us:
        	NOP
		RET

DELAY_100us:
        	PUSH 		R17
		LDI		R17, 20
L1:		CALL		DELAY_5us
		DEC		R17
		BRNE		L1
		POP		R17
		RET

DELAY_5ms:
        	PUSH		R17
		LDI		R17, 100
L2:		CALL		DELAY_100us
		DEC		R17
		BRNE		L2
		POP		R17
		RET

DELAY_15ms:     
		LDI 		R17,100
LP2:		LDI 		R18,150

LP1:		NOP
		NOP
		DEC 		R18
		BRNE 		LP1	
		DEC 		R17
		BRNE 		LP2
		RET

DELAY:
        	CALL 		DELAY_5ms
		DEC 		R16
		CPI 		R16, 0
		BRNE 		DELAY
		RET
