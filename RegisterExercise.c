.INCLUDE "usb1286def.inc"
 
.ORG $0
 
           	LDI R20, $99                  	; Loads $99 into R20
           	MOV R0, R20                 	; Moves value of R20 into R0
           	MOV R12, R0                 	; Moves value of R0 into R12
           	MOV R31, R12               		; Moves value of R12 into R31
 
Loop:
 
           	RJMP Loop                         ; Creates infinite loop
