.INCLUDE "usb1286def.inc"
.ORG 0

	LDI R16, $00
	OUT DDRE, R16
	LDI R17, $FF
	OUT DDRC, R17
	OUT DDRA, R17

L1:

	IN R16, PINE
	OUT PORTC, R16
	OUT PORTA, R16
	MOV R20, R16
	MOV R21, R16
	RJMP L1

.EXIT
