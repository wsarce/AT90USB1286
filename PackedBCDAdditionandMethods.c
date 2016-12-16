.INCLUDE "usb1286def.inc"
.ORG 0x00

	;Initialize stack
	LDI R16, LOW (RAMEND)		
	OUT SPL, R16
	LDI R16, HIGH (RAMEND)
	OUT SPH, R16

	;Main Program Code

	LDI ZL, LOW (MyPackedBCD_Digits<<1)
	LDI ZH, HIGH (MyPackedBCD_Digits<<1)
	LDI R16, 10	
	CALL SUM_PACKED_BCD_BYTES

LO: RJMP LO						; Keep off the streets

;Data to be stored then added
MyPackedBCD_Digits:	.DB	0x04, 0x08, 0x15, 0x16, 0x23, 0x42, 0x24, 0x32, 0x61, 0x51

;Procedure
SUM_PACKED_BCD_BYTES:
	LDI R17, 0					; Start off 0
	MOV R23, R16					; Make R23 the new counter
JUMP: 	
	CPI R23, 0					; Is counter = 0?
	BREQ END					; If it is then Branch to the end
	LPM R16, Z+					; Load from program memory into r16
	DEC R23					; Otherwise decrement counter
	CALL ADD_PACKED_BCD				; Add values together
	MOV R17, R16					; Move addition to next value
	
	RJMP JUMP	
END:
	MOV R30, R16
	MOV R31, R24
RET

;Procedure 
ADD_PACKED_BCD:
	CLC
	MOV	R20, R16				; Copy value of R16 to R20
	MOV	R21, R17				; Copy value of R17 to R21
	ANDI	R20, 0x0F				; Mask the upper nibble
	ANDI	R21, 0x0F				; Mask the upper nibble
	ADD	R20, R21				; Add two lower nibbles
	ADD	R20, R18				; Add 'carry'
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
	SWAP	R16					; Swap the nibbles in R16
	OR	R16, R20				; Add the resultants
	RET	

