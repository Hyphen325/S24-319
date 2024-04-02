//****************** ECE319K_Lab1.s ***************
// Your solution to Lab 1 in assembly code
// Author: Noah Graf
// Last Modified: 1/18/24
// Spring 2024
        .data
        .align 2
// Declare global variables here if needed


// with the .space assembly directive

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "NTG393" // replace ZZZ123 with your EID here
        .align 2
        .global Phase
Phase:  .long 10 // 0 for info, 1-6 for debug, 10 for grade

        .global Lab1
// Input: R0 points to the list
// Return: R0 as specified in Lab 1 assignment and terminal window
// According to AAPCS, you must save/restore R4-R7
// If your function calls another function, you must save/restore LR
Lab1: PUSH {R4-R7,LR}
       // your solution goes here
       LDR R1, =EID//addr of eid
       MOVS R5, #0//R5 is the index of the eid currently being checked

Check: LDR R3, [R0]//Loads value at R0 to see if program is at the end of the list
       CMP R3, #0
       BEQ Fail

       //Compare function
       PUSH {R5}//Saves current index
       MOVS R5, #0//sets R5 to 0 so it can act as a counter to select what letter
       BL Comp
       POP {R5}
       //
       CMP R2, #0//checks the output of the compare function
       BEQ SUC
       ADDS R0, #8//incriments all counter and index
       ADDS R5, #1
       B Check

Fail: MOVS R0, #0//sets R0 to -1
	  SUBS R0, R0, #1
	  B END
SUC:
	  MOVS R0, R5//sets R0 to the current index
	  B END


END:  POP  {R4-R7,PC} // return

//compare eid function
Comp:
	  LDRB R3, [R1,R5]//loads letter of your eid into r3
	  LDR R2, [R0]
	  LDRB R2, [R2,R5]//loads letter of first eid in list
	  CMP R2, R3
	  BNE NE
	  CMP R2, #0//checks to see if EIDs are at end of character
	  BEQ YE
	  ADDS R5, #1 //incriments counter
	  B Comp
NE:   MOVS R2, #1
	  BX LR
YE:   MOVS R2, #0//Sets R2 to 0 to indicate equality
	  BX LR
        .end
