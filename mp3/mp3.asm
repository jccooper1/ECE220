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
				JSR	INIT_WORK
				JSR	CALC_DISTANCE
				JSR	PRINT_DECIMAL	;print the distance in demical way
				LD	R0,ANO_LINE	;another line
				OUT
				JSR	saveRs
				JSR PRETTY_PRINT
				JSR	loadRs
				HALT
;start from here comes the parts of INIT_WORK and CALC_DISTANCE
;register table for INIT_WORK
;		part1							part2										part3
;R0		not used					| get operation of insertion cost	  	  | get operation of deletion cost
;R1 is offset R1=0					| R1=0 is the operation value of insertion| R1=1 is the operation value of deletion
;R2	is predecessor type R2=-1		| R2=-3M to find the address of offset	  | R2=-3 to to find the address of offset
;R3	table of x4000					| still the table						  | still the table
;R4 	not used					| R4=N									  | R4=M
;R5		not used					| R5=3M									  | R5=3
;R6		not used					| count the insertion 					  |count the deletion

INIT_WORK	ST	R7,saveR7
			LD	R3,TABLE 	;R3=x4000
			AND	R1,R1,#0	;initialize the R1
			AND	R2,R2,#0	;initialize the R2
			ADD	R2,R2,#-1	;R2=-1
			STR	R1,R3,#0	;0->m(x4000)
			STR	R1,R3,#1	;0->m(x4001)
			STR	R2,R3,#2	;-1->m(x4002) part1 finished

;part2 goes here, part2 is to find the entries in column0
			LDI	R0,addrINS	;R0 is the cost of insertion
			LDI	R2,FINAL_M	;R2 is value of M
			AND	R4,R4,#0	;make R4=0
			ADD	R4,R2,R4	;R4=M
			ADD	R4,R4,R4	;R4=2M
			ADD	R2,R2,R4	;R2=3M
			AND	R5,R5,#0	;R5=0
			ADD	R5,R5,R2	;R5=3M
			NOT	R2,R2		
			ADD	R2,R2,#1	;R2=-3M
			ST	R2,offsetI	;save -3M
			ADD	R4,R2,#-3	;save -3m-3
			ST	R4,offsetSM
			LDI	R4,FINAL_N	;R4=N
LOOPcol0	ADD	R4,R4,#-1	
			BRz col0done	
			ADD	R6,R6,R0	;operation:k*cost of insertion
			ADD	R3,R3,R5	;x4000+3M*k
			STR	R6,R3,#0	;k*cost->m(x4000+3M*k)
			STR R2,R3,#1	;-3M->m(x4000+3M*k+1)
			STR	R1,R3,#2	;0->m(x4000+3M*k+2)
			BRnzp	LOOPcol0
;part3 goes here,part3 is to find the entries in row0
col0done	LDI	R0,addrDELE	;R0 is the cost of deletion

			LDI	R4,FINAL_M
			ADD	R4,R4,#-1	;check if M-1 is zero
			BRz	row0done
			BR	one



one			AND	R1,R1,#0	;make R1=0
			ADD	R1,R1,#1	;R1=1
			AND	R6,R6,#0	;initialize the R6
			AND	R2,R2,#0
			ADD	R2,R2,#-3	;R2=-3
			ST	R2,offsetD	;save -3
			AND R5,R5,#0
			ADD	R5,R5,#3	;R5=3
			LDI	R4,FINAL_M	;R4=M
			LD	R3,TABLE
LOOProw0	ADD	R4,R4,#-1
			BRz	row0done
			ADD	R6,R6,R0	;operation:k*cost of deletion
			ADD	R3,R3,R5	;x4000+3k
			STR	R6,R3,#0	;k*cost->m(x4000+3k)
			STR	R2,R3,#1	;-3->m(x4000+3k+1)
			STR	R1,R3,#2	;1->m(x4000+3k+2)
			BRnzp	LOOProw0
row0done	LD	R7,saveR7
			RET				

;register table for CALC_DISTANCE
;
;R0 based on the comments
;R1 counts the row N-1
;R2 based one the comments 
;R3 is still the table 
;R4 the offset of s/m
;R5 compare the characters in the two string
;R6 still count the total cost
;R7 counts the column M-1

CALC_DISTANCE	ST	R7,saveR7
				LD	R3,TABLE	;R3 is the table
				LDI	R7,FINAL_M	;R7=M
				ADD	R1,R7,R7	;R1=2M
				ADD	R1,R7,R1	;R1=3M
				ADD	R1,R1,#3	;R1=3M+3
				ADD	R3,R3,R1	;R3=x4000+3M+3 start from (1,1)
				LDI	R1,FINAL_N	;R1=N
				ADD	R1,R1,#-1	;R1=N-1
				BRz	calfinished
				ADD	R7,R7,#-1	;R7=M-1
				BRz	calfinished
				
entry			LD	R4,offsetSM	;R4=-3m-3
				ADD	R4,R4,R3	
				LDR	R4,R4,#0					
				ADD	R6,R4,#0	;this is best distance cost at this moment
				LD	R2,String1	;R2=x3800
				LDR	R5,R2,#0	;get the character of string M
				ADD	R2,R2,#1	;next character
				ST	R2,String1
				LD	R2,String2	;R2=x3840
				LDR	R2,R2,#0	;R2<-m(x3840)
				NOT	R2,R2		
				ADD	R2,R2,#1	
				ADD	R5,R5,R2	;check if the characters match
				BRz	match
				LD	R5,addrSUB	
				LDR	R5,R5,#0	;get the cost of substitute and match
				ADD	R6,R6,R5	;add the substitute if don't match the best cost at this moment
match			LD	R0,offsetSM
				STR	R0,R3,#1
				AND	R0,R0,#0
				ADD	R0,R0,#2	;R0=2
				STR	R0,R3,#2	;fill the operation value
				LD	R4,offsetD	;R4=-3
				ADD	R4,R3,R4
				ADD	R4,R4,#0	;stupid instruction to solve the LC3 vscode warning	
				LDR	R4,R4,#0	;initialize the comparing distance
				LD	R2,addrDELE
				LDR	R2,R2,#0	;get the cost of deletion
				ADD	R4,R2,R4	;plus the deletion. the distance at the moment
				NOT	R5,R4	
				ADD	R5,R5,#1	;R5=-R4
				ADD	R5,R6,R5	;compare the value of distance
				BRnz	insertion
				ADD	R6,R4,#0	;the distance of R4 is smaller
				LD	R0,offsetD	;change it into deletion
				STR	R0,R3,#1		
				AND	R0,R0,#0
				ADD	R0,R0,#1	
				STR	R0,R3,#2	;get the deletion value 1 into rhe table
insertion		LD	R4,offsetI	
				ADD	R4,R3,R4	;the initial table is stored in R4
				LDR	R4,R4,#0	
				LD	R2,addrINS
				LDR	R2,R2,#0	;get the value of insertion -3M
				ADD	R4,R2,R4	;distance+insertion
				NOT	R5,R4
				ADD	R5,R5,#1	;R5=-R4
				ADD	R5,R6,R5	;comparing the value
				BRnz nextentry
				ADD	R6,R4,#0
				LD	R0,offsetI
				STR	R0,R3,#1
				AND	R0,R0,#0
				STR	R0,R3,#2
nextentry		STR	R6,R3,#0
				ADD	R3,R3,#3	;continue to next column
				ADD	R7,R7,#-1	;decrement column counter
				BRz nextrow
				BRp	entry

nextrow			LD	R2,STRING_M	;reset the R2
				ST	R2,String1	
				ADD	R3,R3,#3	;first entry in the next row
				LD	R5,String2
				ADD	R5,R5,#1	;next character
				ST	R5,String2
				LD	R7,FINAL_M
				LDR	R7,R7,#0
				ADD	R7,R7,#-1	;reset the R7
				ADD	R1,R1,#-1	;decrement the row
				BRz	calfinished
				BRp entry
calfinished			
				ADD	R1,R6,#0
				LD	R7,saveR7	;finished
				RET
String1		.FILL	x3800
String2		.FILL	x3840
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
offsetI		.FILL	x0000
offsetD		.FILL	x0000
offsetSM	.FILL	x0000
TABLE 		.FILL	x4000
distance	.FILL	x38C0
addrINS		.FILL	x3880
addrDELE	.FILL	x3881
addrSUB		.FILL	x3882
prespace	.BLKW	x000A
stack		.FILL	x8000			
acsiizero	.FILL	x0030
tens		.FILL	x2710
			.FILL	x03E8
			.FILL	x0064
			.FILL	x000A
			.FILL	x0001


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
;save the original R0-R6 forget the warning here I have already done the whole steps of call and save
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
                
STRLEN			ST		R0,saveR0
				ST		R6,saveR6
				AND		R1,R1,#0
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
		BR	here
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


			.END





