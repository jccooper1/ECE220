; Add this test code to the start of your file (just after .ORIG).
; I'd put it in another file, but we can't use the PRINT_DECIMAL and 
; STRLEN labels outside of the mp1.asm file (at least, not easily).

; Read the comments in this file to understand what it's doing and
; for ways that you can use this test code.  You can also just run
; it and diff the output with the output produced by our 'gold'
; (bug-free!) version.
;
; After assembling mp1 with lc3as, execute the test script by typing
;    lc3sim -s script1 > your_output
; (look at the script--it just loads mp1 with a file command, then
; continues execution; when the LC-3 halts, the script is finished,
; so the simulator halts).
;
; You can then type
;    diff your_output out1
; to compare your code's output with ours.
;
	.ORIG      x3000
	; feeling lazy, so I'm going to set all of the bits to the same value
	LD	R0,BITS
	ADD	R2,R0,#0
	ADD	R3,R0,#0
	ADD	R4,R0,#0
	ADD	R5,R0,#0
	ADD	R6,R0,#0

	; let's try PRINT_DECIMAL ... "1230"
	LD	R1,EXAMPLE

	; set a breakpoint here in the debugger, then use 'next' to
	; execute your subroutine and see what happens to the registers;
	; they're not supposed to change (except for R7)...
	JSR	PRINT_DECIMAL

	; we're short on human time to test your code, so we'll do 
	; something like the following instead (feel free to replicate)...
	LD	R7,BITS
	NOT	R7,R7
	ADD	R7,R7,#1
	ADD	R0,R0,R7
	BRz	R0_OK
	LEA	R0,R0_BAD
	PUTS
R0_OK	

	; this trap changes register values, so it's not sufficient
	; to check that all of the registers are unchanged; HALT may
	; also lead to confusion because the register values differ
	; for other reasons (R7 differences, for example).
	HALT

BITS	.FILL	xABCD	; something unusual
EXAMPLE .FILL   #1230
R0_BAD	.STRINGZ "PRINT_DECIMAL changes R0!\n"

; your code should go here ... don't forget .ORIG and .END

               

                
				JSR		STRLEN
				JSR		PRINT_DECIMAL

;save the original R0-R7(except the R1)
saveRs			ST		R0,saveR0
				ST		R2,saveR2
				ST		R3,saveR3
				ST		R4,saveR4
				ST		R5,saveR5
				ST		R6,saveR6
				ST 		R7,saveR7
				RET
loadRs			LD		R0,saveR0
				LD		R2,saveR2
				LD		R3,saveR3
				LD		R4,saveR4
				LD		R5,saveR5
				LD		R6,saveR6
				LD 		R7,saveR7	
				RET				
STRLEN			AND 	R1,R1,#0	;initialize the R1
				ST		R0,saveR0
				ST		R6,saveR6
checking		LDR		R6,R0,#0	;find the string in R0
				BRz		checked		;if it's 0 checked
				ADD		R1,R1,#1
				ADD		R0,R0,#1	;count the ASCII 
				BRnzp	checking
checked			LD		R0,saveR0
				LD		R6,saveR6
				RET
;R2:power of tens
;R3 the value of some digit from 0 to 9
;R4 the position of digit from 0 to 5
;R5 prepared empty space


PRINT_DECIMAL	ADD		R1,R1,#0	;check if it is 0
				BRz		spzero
				BRnp	here
spzero			LD		R0,acsiizero
				ST		R7,saveR7
				TRAP	x21			;print the R0
				LD		R7,saveR7
				BRnzp	finish
here			ST 		R7,saveR7
				JSR		saveRs
				LD		R7,saveR7
				LEA		R2,tens
				LEA     R5, prespace
				LEA     R0, prespace
				AND		R4,R4,0		;initialize the R4 0
				ADD		R4,R4,5		;start from 10000
checkdigit		AND		R3,R3,0		;make R3 0
				LDR		R6,R2,0		;R6 become tens
				NOT		R6,R6
				ADD		R6,R6,#1
count			ADD		R1,R1,R6	;R1-R6
				BRn		checkeddigit
				ADD		R3,R3,#1
				BRnzp	count		
checkeddigit	NOT		R6,R6
				ADD		R6,R6,#1
				ADD		R1,R1,R6	;get the original R1
				LD		R6,acsiizero
				ADD		R6,R5,R6
				STR		R6,R5,#0
				ADD		R5,R5,#1	;update the address
				ADD		R4,R4,#-1	;update the digitposition
				ADD		R2,R2,#1	;update the tens
				ADD		R4,R4,#0
				BRp		checkdigit	;continue the loop	until R4=0
				LD		R6,acsiizero
				NOT		R6,R6
				ADD		R6,R6,#1
checkzero		LDR		R3,R0,#0
				ADD		R3,R3,R6
				BRp		printeach
				ADD		R0,R0,#1
				BRnzp	checkzero
printeach		ST		R7,saveR7
				TRAP 	x22
				LD		R7,saveR7
finish			ST		R7,saveR7
				JSR		loadRs
				RET

saveR0		.FILL	x0000

saveR2		.FILL	x0000
saveR3		.FILL	x0000
saveR4		.FILL	x0000
saveR5		.FILL	x0000
saveR6		.FILL	x0000
saveR7		.FILL	x0000
acsiizero	.FILL	x0030
tens		.FILL	x2710
			.FILL	x03E8
			.FILL	x0064
			.FILL	x000A
			.FILL	x0001

prespace	.BLKW	x0006
			.END