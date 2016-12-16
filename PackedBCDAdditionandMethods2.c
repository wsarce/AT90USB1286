.INCLUDE "usb1286def.inc"
.ORG 0x00

RJMP OVER
CONVERT:.DB 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
OVER:
	;Initialize stack

	LDI R16, LOW (RAMEND)		
	OUT SPL, R16
	LDI R16, HIGH (RAMEND)
	OUT SPH, R16

	LDI R16, 0xFF
	OUT DDRA, R16
	OUT DDRD, R16
	LDI R16, 0
	LDI R17, 0x01
	LDI R18, 0x00

LOOP:
	CALL DELAY_AND_DISPLAY
	CALL ADD_PACKED_BCD
	LDI R17, 0x01
	RJMP LOOP

L:  RJMP L


DELAY_AND_DISPLAY:
	LDI ZL, LOW(CONVERT<<1)
	LDI ZH, HIGH(CONVERT<<1)			; Import string
	MOV R20, R16
	MOV R21, R16
	ANDI R21, $0F					; Mask upper nibbles
	SWAP R20
	ANDI R20, 0x0F				; Mask upper nibble 
	
	ADD ZL, R21
	LPM R23, Z					; Binary value needed to output
	SUB ZL, R21
	ADD ZL, R20
	LPM R24, Z					; Binary value needed to output
	SUB ZL, R20	

	LDI R28, 0x00					; Make register 28 a counter	
OUTER:	LDI R26, 0xFF
INNER:				
	SBI PORTD, 7					; Clear pin 7
	OUT PORTA, R24				; Port the second value to the 7 seg
	OUT PORTA, R28
	NOP
	NOP
	CBI PORTD, 7					; Set pin 7
	OUT PORTA, R23
	OUT PORTA, R28				; Port the first value to the 7 seg
	DEC R26					; Decrement the counter
	BRNE INNER
	DEC R27
	BRNE OUTER
RET


ADD_PACKED_BCD:
	CLC
	MOV	R20, R16				; Copy value of R16 to R20
	MOV	R21, R17				; Copy value of R17 to R21
	ANDI	R20, 0x0F				; Mask the upper nibble
	ANDI	R21, 0x0F				; Mask the upper nibble
	ADD	R20, R21				; Add two lower nibbles
	;ADD	R20, R18				; Add 'carry'
	CPI	R20, 0x0A				; Compare to 0x0A
	BRSH	ADJUST					; If same or higher than 0x0A, branch
	RJMP	CLEAR					; Else jump to clear the 'carry'
ADJUST:
	SUBI	R20, 0x0A				; Subtract 0x0A from answer
	SBR	R18, 1					; Set 'carry'
	RJMP	SECOND					; Jump out
CLEAR:
	CBR	R18, 1					; Clear carry
	RJMP	SECOND					; Jump out
SECOND:
	SWAP	R16					; Switch nibble position
	SWAP	R17					; Swtich nibble position
	ANDI	R16, 0x0F				; Mask the upper nibbles
	ANDI	R17, 0x0F				; Mask the upper nibbles
	ADD	R16, R17				; Add the two lower nibbles
	ADD	R16, R18				; Add the carry
	CPI	R16, 0x0A				; Compare 0x0A to determine branch
	BRSH	ADJ					; Branch if carry needs to be set
	RJMP	CCR					; Branch if no carry needed
ADJ:
	SUBI	R16, 0x0A				; Subtract 0x0A from answer
	SBR	R18, 1					; Set carry
	INC	R24
	RJMP	FINAL					; Jump out
CCR:
	CBR	R18, 1					; Clear the carry
	RJMP	FINAL					; Jump out
FINAL:
	SWAP	R16					; Swap the nibbles in register 16
	OR	R16, R20				; Add the resultants
	RET

