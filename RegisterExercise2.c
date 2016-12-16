.INCLUDE "usb1286def.inc"
 
.ORG $0
 
           	LDI R18, 1                       	; Loads 1 into R18
           	LDI R19, 2                       	; Loads 2 into R19
           	ADD R18, R19 				; Adds R19 to R18
           	LDI R19, 3                       	; Loads 3 into R19
           	ADD R18, R19 				; Adds R19 to R18
           	LDI R19, 4                       	; Loads 4 into R19
           	ADD R18, R19 				; Adds R19 to R18
           	LDI R19, 5                       	; Loads 5 into R19
           	ADD R18, R19 				; Adds R19 to R18
           	LDI R19, 6                       	; Loads 6 into R19
           	ADD R18, R19 				; Adds R19 to R18
           	LDI R19, 7                       	; Loads 7 into R19
           	ADD R18, R19 				; Adds R19 to R18
           	LDI R19, 8                       	; Loads 8 into R19
           	ADD R18, R19 				; Adds R19 to R18
           	MOV R16, R18				; Moves value of R18 into R16
 	
Loop:
           	
           	RJMP Loop                     	; Creates infinite loop

