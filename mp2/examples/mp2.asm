			.ORIG x3000
;At start print the first two lines
;R0 is used to print the string
				JSR FIND_M_N
				LD	R0,STRING_M	;print the first line of value in m
				PUTS
				LEA	R0,LINE_ONE	;print ->
				PUTS
				LD	R0,STRING_N	;print the	first line of value in n
				PUTS
				LD	R0,ANO_LINE	;another line
				OUT
				LEA	R0,LINE_TWO	;print the second line
				PUTS
				LDI	R1,distance	;The 2s complement of distance->R1
				JSR	PRINT_DECIMAL	;print the distance in demical way
				LD	R0,ANO_LINE	;another line
				OUT
				JSR	saveRs
				JSR PRETTY_PRINT
				JSR	loadRs
				HALT


FIND_M_N		ST	R7,saveR7
				JSR	saveRs
				LD 	R0,STRING_M	;find what is m
				JSR	STRLEN	
				ADD	R1,R1,#1	;m+1
				LD	R0,FINAL_M	;R0<-M(x38E0)
				STR	R1,R0,#0	
				LD	R0,STRING_N	;find what is n
				JSR	STRLEN	
				ADD	R1,R1,#1	;n+1
				LD	R0,FINAL_N	;R0<-M(x38E1)
				STR	R1,R0,#0	
				JSR	loadRs
				LD	R7,saveR7
				RET
;save the original R0-R6
saveRs          ST      R0,saveR0
                ST      R1,saveR1
                ST      R2,saveR2
                ST      R3,saveR3
                ST      R4,saveR4
                ST      R5,saveR5
                ST      R6,saveR6
                RET
loadRs          LD      R0,saveR0
                LD      R1,saveR1
                LD      R2,saveR2
                LD      R3,saveR3
                LD      R4,saveR4
                LD      R5,saveR5
                LD      R6,saveR6
                RET
                
STRLEN			AND		R1,R1,#0	;initialize the R1
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
;this part is for PRETTY_PRINT
;R0 is m       
;R1 is n		it also used to count the number of stack and calculate the 3mn
;R2 calculate the m*n to find the address from x4000 to x4000+3mn-1
;R3 is the table from x4000 to x4000+3mn-2
;R4 is the operations (like insertion and so on) from the value in the table
;R5 is the offset
;R6 is stackpointer starting from x8000
;R7 is for jump to the main program and subroutine program
PRETTY_PRINT	ST	R7,saveR7
				LDI	R0,FINAL_M	;m->R0
				LDI	R1,FINAL_N	;n->R1
				LD 	R6,stack	;R6=x8000
				AND	R2,R2,#0	;initialize R2
multiple		ADD	R2,R2,R0	
				ADD	R1,R1,#-1	;R2=m*n
				BRp	multiple	
				AND R1,R1,#0	;make R1=0
				ADD	R1,R2,R2	;R1=2m*n
				ADD	R2,R1,R2	;R2=3m*n
				LD	R3,TABLE	;R3=x4000
				ADD	R3,R2,R3	;R3=x4000+3mn find the last entry of the table
				ADD	R3,R3,#-2	;R3=x4000+3mn-2 find address of the offset of each bit
				AND R1,R1,#0	;initialize R1
LOOP			LDR	R5,R3,#0	;R5 is the offset
				BRz middleadjust
				LDR	R4,R3,#1	;get the operations
				ADD	R6,R6,#-1	;stack minus one from x8000
				STR	R4,R6,#0	;push the operations into stack
				ADD	R1,R1,#1	;count the stack
				ADD	R3,R3,R5	;next bits
				BRnzp	LOOP
middleadjust	AND	R3,R3,#0
				ADD	R3,R1,R3	;save the R1 into R3
				AND	R5,R5,#0	;initialize the R5
				ADD	R5,R6,R5	;save the R6 into R5				
				LD	R2,STRING_M	;R2=x3800

checkline3st	ADD	R1,R1,#-1	
				BRn	checkline3
				LDR	R4,R6,#0	;push out the value in R6
				ADD	R6,R6,#1	;increment the R6
				ADD	R4,R4,#0	
				BRz	SHORTLINE
				LDR	R0,R2,#0	
				OUT
				ADD	R2,R2,#1	;print the next character
				BRnzp	checkline3st	
SHORTLINE		LD	R0,dash	
				OUT
				BRnzp	checkline3st
checkline3		LD	R0,ANO_LINE	;the next line
				OUT
				ADD	R1,R3,#0	;get the original R1
				ADD	R6,R5,#0	;get the original R5


				LD	R2,STRING_N				
checkline4st	ADD	R1,R1,#-1	
				BRn	checkline4
				LDR	R4,R6,#0	;push out the value in R6
				ADD	R6,R6,#1	;increment the R6
				ADD	R4,R4,#-1	
				BRz	SHORTLINE1
				LDR	R0,R2,#0	
				OUT
				ADD	R2,R2,#1	;print the next character
				BRnzp	checkline4st
SHORTLINE1		LD	R0,dash	
				OUT
				BRnzp	checkline4st
checkline4		LD	R0,ANO_LINE	;the next line
				OUT
				LD	R7,saveR7
				RET

;this is PRINT_DECIMAL part copied from mp1
;R2:power of tens
;R3 prepared empty space 
;R4 the position of digit from 0 to 5
;R5 count from 0 to 9
PRINT_DECIMAL	ST	R7, saveR7
		ADD	R1, R1, #0
		BRz	spzero
		BRnp	here
spzero	LD	R0, acsiizero
		OUT
		BRnzp	becky	;becky means this program is finished
here	JSR	saveRs
		LEA	R2, tens
		LEA	R3, prespace
		LEA	R0, prespace
		AND	R4, R4, #0
		ADD	R4, R4, #5
checking1	AND	R5, R5, #0
		LDR	R6, R2, #0
		NOT	R6, R6
		ADD	R6, R6, #1
digitcount	ADD	R1, R1, R6
		BRn	Digit
		ADD	R5, R5, #1
		BRnzp	digitcount
Digit	NOT	R6, R6
		ADD	R6, R6, #1
		ADD	R1, R1, R6
		LD	R6, acsiizero
		ADD	R6, R5, R6
		STR	R6, R3, #0
		ADD	R3, R3, #1
		ADD	R2, R2, #1
		ADD	R4, R4, #-1
		BRz	printnum
		BRp	checking1

printnum	LD	R6, acsiizero
			NOT	R6,R6
			ADD	R6,R6,#1
PrintHeader	LDR	R5, R0, #0
		ADD	R5, R5, R6
		BRp	PrintFinal
		ADD	R0, R0, #1
		BRnzp	PrintHeader
PrintFinal	PUTS
		JSR	loadRs
becky	LD	R7, saveR7
		RET



saveR0		.FILL	x0000
saveR1		.FILL	x0000
saveR2		.FILL	x0000
saveR3		.FILL	x0000
saveR4		.FILL	x0000
saveR5		.FILL	x0000
saveR6		.FILL	x0000
saveR7		.FILL	x0000
STRING_M	.FILL	x3800
STRING_N	.FILL	x3840
FINAL_M		.FILL	x38E0
FINAL_N		.FILL	x38E1
LINE_ONE	.STRINGZ	" -> "
LINE_TWO	.STRINGZ	"Levenshtein distance = "
ANO_LINE	.STRINGZ	"\n"
dash		.STRINGZ	"-"
TABLE 		.FILL	x4000
distance	.FILL	x38C0
prespace	.BLKW	x000A
stack		.FILL	x8000			
acsiizero	.FILL	x0030
tens		.FILL	x2710
			.FILL	x03E8
			.FILL	x0064
			.FILL	x000A
			.FILL	x0001


			.END










