.INCLUDE "usb1286def.inc"
.EQU	LCD_DPRT = PORTB
.EQU	LCD_DDDR = DDRB
.EQU	LCD_DPIN = PINB
.EQU	LCD_CPRT = PORTD
.EQU	LCD_CDDR = DDRD
.EQU	LCD_CPIN = PIND
.EQU	LCD_RS = 6
.EQU	LCD_RW = 4
.EQU	LCD_EN = 5

.ORG $00

	;Initialize stack pointer
	LDI 	R16, LOW(RAMEND)
	OUT 	SPL, R16
	LDI 	R16, HIGH(RAMEND)
	OUT 	SPH, R16
	
	;Set the DDRx
	LDI 	R16, $FF
	OUT 	DDRD, R16
	OUT 	DDRB, R16
	COM 	R16
	OUT 	DDRE, R16

	;Main program code
	CALL 	LCD_init		; Initialize LCD 
	CALL 	READ_DIP		; Get inverted value from PINE
	CALL 	RANDOM			; Get first random number
	MOV 	R1, R0			; Move random to R1
	CALL 	DISPLAY_RANDOM	; Display first string, get second number, display

HERE: RJMP HERE

DATA1: .DB 'F', 'I', 'R', 'S', 'T', ':', $00
DATA2: .DB 'S', 'E', 'C', 'O', 'N', 'D', ':', $00
	
READ_DIP:
	IN 	R16, PINE		; Get value off of PINE
	COM 	R16			; Invert it
	MOV 	R0, R16		; Move it from R16 to R0
	RET				; Return

RANDOM:
	LSL 	R0			; Logic shift left for R0
	MOV 	R16, R0		; Copy R0 into R16
	MOV 	R17, R0		; Copy R0 into R17

	ANDI 	R16, $80		; AND immediate value of $80
	LSR 	R16			; Logic shift right seven times
	LSR 	R16
	LSR 	R16
	LSR 	R16
	LSR 	R16
	LSR 	R16
	LSR 	R16

	ANDI 	R17, $40		; AND immediate value of $40
	LSR 	R17			; Logic shift right six times
	LSR 	R17
	LSR 	R17
	LSR 	R17
	LSR 	R17
	LSR 	R17

	EOR 	R16, R17		; EOR the two bits
	OR 	R0, R16		; Save the value into R0

	MOV 	R16, R0		; Copy new R0 into R16
	LDI 	R17, $80		; Load R17 with $80
CHECK:
	EOR 	R16, R17		; EOR R16 and R17 until bit 7 is clear
	SBRS 	R16, 7			; Wait until bit 7 is clear
	RJMP 	PASS			; When clear, finish
	RJMP 	CHECK			; Loop until bit is clear
PASS:
	MOV 	R0, R16		; Copy R16 into R0
	RET 				; Return

DISPLAY_RANDOM:
	LDI	ZL, LOW(DATA1<<1)	; Load first string into Z pointer
	LDI	ZH, HIGH(DATA1<<1)	
	CALL 	LCD_string		; Print string out on LCD
	CALL 	PRINT_BINARY_STRING	; Print first number after that
	LDI 	R16, $C0		; Move cursor to second line
	CALL 	LCD_cmd		
	CALL 	RANDOM			; Generate second random number
	LDI 	ZH, HIGH(DATA2<<1)	; Load Z pointer with second string
	LDI 	ZL, LOW(DATA2<<1)	
	CALL 	LCD_string		; Print out second line
	CALL 	PRINT_BINARY_STRING	; Print second number
	RET				; Return
; Make this into another procedure: 'PRINT_BINARY_STRING(R0)'
PRINT_BINARY_STRING:
	MOV 	R18, R0		; Copy R0 into R18
	LDI 	R17, 8			; Load counter with 8
NEXT_BIT:
	SBRS 	R18, 7			
	CALL 	SEND_ZERO		; If bit 7 is zero, print it
	SBRC 	R18, 7
	CALL 	SEND_ONE		; If bit 7 is one, print it
	LSL 	R18			; Shift over one for next number
	DEC 	R17			; Decrement counter
	CPI 	R17, 0			; Compare R17 with zero
	BRNE 	NEXT_BIT		; Wait until zero
	RET				; Return
SEND_ONE:
	LDI 	R16, $31		; Load ‘1’ in ASCII
	CALL 	LCD_char		; Send it to LCD
	RET				; Return
SEND_ZERO:
	LDI 	R16, $30		; Load ‘0’ in ASCII
	CALL 	LCD_char		; Send it to LCD
	RET				; Return

LCD_init:
		LDI 		R16, 0xFF
		OUT 		DDRD, R16
		OUT 		DDRB, R16

		CALL 		DELAY_15ms
        	CBI 		PORTD,5		; Set E pin initial state.
		CBI 		PORTD,4		; Set R/W pin initial state.
			 
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
		LDI		R16, 0x01		; Send $01 to clear LCD
		CALL		LCD_cmd		; CALL LCD_cmd
		LDI		R16, 15		; Delay 15 milliseconds
		CALL		DELAY			; CALL delay
		RET					; Return

LCD_cmd:
       	OUT		LCD_DPRT, R16		; Send value on R16 to PORTD
		CBI		LCD_CPRT, LCD_RS	; Clear Read bit on PORTC
		CALL	 	DELAY_5us		; Delay
		SBI		LCD_CPRT, LCD_EN	; Set Enable pin on PORTC
		CALL		DELAY_5us		; Delay
		CBI		LCD_CPRT, LCD_EN	; Clear Enable pin on PORTC
		CALL		DELAY_100us		; Delay
		RET					; Return

LCD_data:
        	OUT		LCD_DPRT, R16		; Send value on R16 to PORTD
		SBI		LCD_CPRT, LCD_RS	; Set Read pin on PORTC
		CALL 		DELAY_5us		; Delay
		SBI		LCD_CPRT, LCD_EN	; Set Enable pin on PORTC
		CALL		DELAY_5us		; Delay
		CBI		LCD_CPRT, LCD_EN	; Clear Enable pin on PORTC
		CALL		DELAY_100us		; Delay
		RET					; Return

LCD_char:
       	CALL		LCD_data		; Call LCD_data
		RET					; Return

LCD_string:

LO:		LPM		R16, Z+		; Load value at Z into R16, increment
		CPI		R16, 0			; Wait for zero bit
		BREQ   	DONE			; Wait until zero
		CALL		LCD_char		; Send to LCD_char
		RJMP   	LO			; Continue
DONE:
		RET					; Return


;---------
DELAY_5us:
       	NOP				; No operation
		RET				; Return

DELAY_100us:
       	PUSH 	R17			; Save value on R17
		LDI	R17, 20		; Load counter
L1:		CALL	DELAY_5us		; CALL delay
		DEC	R17			; Decrement counter
		BRNE	L1			; Wait until zero
		POP	R17
		RET				; Return

DELAY_5ms:
       	PUSH	R17			; Save value on R17
		LDI	R17, 100		; Set delay counter 
L2:		CALL	DELAY_100us
		DEC	R17
		BRNE	L2
		POP	R17
		RET

DELAY_15ms:     
		LDI 	R17,100		; Set delay counter 1
LP2:		LDI	R18,150		; Set delay counter 2

LP1:		NOP				; No operation
		NOP				; No operation
		DEC 	R18			; Decrement counter
		BRNE 	LP1			; Wait until zero
		DEC 	R17			; Decrement counter
		BRNE 	LP2			; Wait until zero
		RET				; Return

DELAY:
        	CALL 	DELAY_5ms		; Call DELAY
		DEC 	R16			; Decrement counter
		CPI 	R16, 0			; Wait until zero
		BRNE 	DELAY
		RET				; Return
.EXIT
