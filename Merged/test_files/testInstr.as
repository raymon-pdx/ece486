/ Program : testInstr.pal
/ Date : March 3, 2002
/
/ Desc : This program will test instructions
/
/-------------------------------------------
/
/ Code Section
/
*0200			/ start at address 0200
Main, 	cla cll 	/ clear AC and Link
	tad A 		/ add A to Accumulator
	tad B 		/ add B
	dca C 		/ store sum at C
	cla cll     / Clear AC and Link again
	tad D       / add 16 to ac
	and B       / and D and B
	dca A       / store D&B to memory
	jms JUMP    / jump to subroutine
	tad C       / load 5 into ac
	hlt 		/ Halt program
	jmp Main	/ To continue - goto Main
	
JUMP, 0 
	cma         / complement the accumulator
	iac         / increment to cause link = 1
	sma         / link set so skip next instruction
	tad B       / ac = 3 should be skipped
	tad C       / ac = 5
	rar         / rotate right
	ral         / rotate left
	cia         / complement and increment ac
	jmp i JUMP  / return from subroutine
/
/ Data Section
/
*0250 			/ place data at address 0250
A, 	2 		/ A equals 2
B, 	3 		/ B equals 3
C, 	0       / C equals 0
D, 16       / D equals 16
$Main 			/ End of Program; Main is entry point
