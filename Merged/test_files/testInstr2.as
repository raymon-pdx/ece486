/ Program : testInstr2.as
/ Date : March 2, 2015
/
/ Desc : This program will test instructions
/        for a PDP8 ISA
/
/-------------------------------------------
/
/ Code Section
/
*0200			/ start at address 0200
Main, 	cla cll 	/ clear AC and Link
	/TEST AND
	tad REGMAX      / set 0xFFF in AC
	and B           / and with 0x002
	dca BIN         / store 2 in BIN
	cla cll         
	and B           / 0x000 AND 0x002
	dca BIN         / store 0 in BIN
	
    / TEST TAD
	cla cll         / clear link and AC
	tad REGMAX 		/ add MAX to AC
	tad A 		    / add 1 to overflow AC, link=1
	tad REGOFL      / overflow AC, link=0
	dca BIN         / store AC in BIN
	
	/ TEST ISZ  
	cla cll         / clear link and AC
	tad A           / AC=1
	isz REGMAX      / cause skip
    tad B           / AC=2 should be skipped
	dca BIN         / BIN=1
	                / test second case
	cla cll         / clear link and AC
	tad A           / AC=1
	isz C           / do not skip
    tad B           / AC=2+1 should be skipped
	dca BIN         / BIN=3
	
	/ TEST JMS and LMP
	cla cll         / clear link and AC
	tad REGOFL      / overflow AC to set LINK
	SMA             / skip next instruction
	jms JUMP2       / skip this instruction
	jms JUMP1       / jump to this subroutine
	jmp CONTINUE1   / skip over subroutines
	
JUMP1, 0
	and A           / store 1 in BIN of JUMP1 taken
	cla BIN
	jmp i JUMP1     / return from subroutine

JUMP2, 0	
	and B           / store 2 in BIN of JUMP2 taken
	cla BIN
	jmp i JUMP2     / return from subroutine
	
CONTINUE1, cla cll  / clear link and AC
	/ TEST CMA
	tad REGMAX      / set AC to 0xFFF
	cma             / AC should be 0x000
	tad REGOFL      / overflow AC to set LINK
	cma             / AC should be 0x000, LINK=1
	
	/ TEST CML
	cla cll         / clear link and AC
	cml             / set LINK=1
	cml             / set LINK=0
	
	/ TEST IAC
	iac             / increment AC from 0 to 1
	cla             / clear AC
	tad REGMAX      / AC=0xFFF
	iac             / AC should = 0x000, LINK = 0
	cla cll
	tad REGOFL      / AC=0xFFF, LINK=1
	iac             / increment AC, should be AC=0x000, LINK=1
	
	/ TEST ROTATING
	cla cll			/ clear link and AC
	tad D           / set AC=0x004
	rar             / rotate right 1 bit, AC=0x002
	ral             / rotate left 1 bit,  AC=0x004
	rtr             / rotate right 2 bits,AC=0x001
	rtl             / rotate left 2 bits, AC=0x004
	cla             / clear AC
	tad REGMAX      / set AC=0xFFF
	and HIGH        / set AC=0xF00
	ral             / rotate left, AC=0xE00, LN=1
	rtr             / right twice, AC=0x780, LN=0
	rtl             / left twice,  AC=0xE00, LN=1
	rar             / rotate right,AC=0xF00, LN=0
	
	/ TEST SMA, SZA, SNL SKIPPING INSTRUCTIONS
	cla cll			/ clear link and AC
	tad NEG         / set most significant bit to 1
	sma             / skip next instruction since AC is NEGATIVE
	tad A           / AC=1 if not skipped
	cla             / set AC=0
	sza             / AC=0 so skip next instruction
	tad B           / AC=2 if not skipped
	tad REGMAX      / set AC to 0xFFF, note: this is negative
	cml             / set link to 1
	snl             / skip next instruction since link != 0
	and C           / AC=3 if not skipped
	
	/ TEST SPA, SNA, SZL SKIPPING INSTRUCTIONS
	cla cll			/ clear link and AC
	tad D           / AC=4
	spa             / skip next instruction since AC positive
	tad A           / AC=4+1=5 if not skipped
	sna             / skip next instruction since AC non-zero
	tad B           / AC=4+2=6 if not skipped
	szl             / skip next instruction since link=0
	tad C           / AC=4+3=7 if not skipped
	
	/ TEST SKP
	skp             / skip next instruction
	tad D           / AC=4+4=8 if not skipped
	
	/ TEST OSR?
	
	// END PROGRAM
	hlt 		    / Halt program
	jmp Main	    / To continue - goto Main
	
/
/ Data Section
/
*0000 			/ place data at address 0000
A, 1 		    / A equals 1
B, 2 		    / B equals 2
C, 3            / C equals 3
D, 4            / D equals 4
BIN, 0          / used for storing results
REGMAX, 7777    / max storage of 12 bit reg
REGOFL, 17777   / overflow 12 bit reg
LOW, 17         / low byte mask for 12 bits
MID, 360        / mid byte mask for 12 bits
HIGH, 7400      / high byte mask for 12 bits
NEG, 4000       / set most significant bit
$Main 			/ End of Program; Main is entry point
