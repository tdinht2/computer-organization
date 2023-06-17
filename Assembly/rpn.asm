;; =============================================================
;; CS 2110 - Fall 2020
;; Project 3 - RPN Calculator 
;; =============================================================
;; Name: Tien Tran
;; =============================================================


.orig x3000
HALT

;; DO NOT add anything before this line
.end
;; !!!! PLEASE READ !!!!!
;; - The specification for each subroutine can be found before the corresponding
;;   label for the subroutine. It contains information about the input register(s),
;;   output register(s), and the operation that needs to be carried out
;; 
;; - For some subroutines, we ask that you follow the pseudocode provided in the pdf.
;;   You are welcome to deviate from it, but we won't be able to help you in case you
;;   run into issues with the autograder 
;; 
;; - To test your subroutines, we highly recommend that you to use complx before
;;	 running the autograder. Details on how to run subroutines in complx can be found
;;   in the project pdf. We have covered this in lab too, so feel free to rewatch one
;;   of the lab recordings on this. Doing it this way will save you a TON of time.


;; Performs addition between two integers `a` and `b`
;;
;; input:		R0 = a, R1 = b
;; output:		R2 = a + b
	;; YOUR IMPLEMENTATION HERE
.orig x3010
my_add
ADD R2, R0, R1		;Add a + b and store in R2
RET
.end

;; Performs subtraction between two integers `a` and `b`
;;
;; input:		R0 = a, R1 = b
;; output:		R2 = a - b

	;; YOUR IMPLEMENTATION HERE
.orig x3020
my_sub
NOT R1, R1
ADD R1, R1, 1		;Convert b to -b
ADD R2, R0, R1		;Add a + (-b) and store in R2
ADD R1, R1, -1
NOT R1, R1			;change (-b) back to b to preserve R1's original value
RET
.end

;; Performs multiplication between two integers `a` and `b`
;; 
;; input:		R0 = a, R1 = b
;; output:		R2 = a * b
;; 
;; !!!!! IMPORTANT !!!!!
;; FOLLOW THE PSEDUOCODE PROVIDED IN THE PROJECT PDF

	;; YOUR IMPLEMENTATION HERE
.orig x3030
my_mul
AND R2, R2, 0		;Set result to 0
AND R3, R3, 0		;Set negate equal to 0
ADD R1, R1, 0
BRzp WHILE			;Checks if counter is not negative
ADD R1, R1, -1
NOT R1, R1			;Changes counter to -counter
ADD R3, R3, 1		;set negate equal to 1
WHILE
ADD R1, R1, 0
BRnz END			;Checks if counter is > 0
ADD R2, R2, R0
ADD R1, R1, -1		;Adds a to total sum by b amount of times
BR WHILE
END
ADD R3, R3, 0
BRnz STORE
NOT R2, R2
ADD R2, R2, 1		;if negate > 0, then turn RESULT to negative
STORE
RET
.end




;; Performs **integer** division between two integers `a` and `b`
;; 
;; input:		R0 = a, R1 = b
;; output:		R2 = a / b
;; examples:	
;;				 4   /   2	=  2
;;				 16  /	 4	=  4
;;				 95  /	 5	=  19 
;;
;;				 8  /   3	=  2	(not 2.6667)
;;				-8  /   3	= -2	(not -2.6667)
;;				 8  /  -3	= -2	(not -2.6667)
;;
;;				 5  /  10	=  0	(not 0.5)
;;				 5  / -10	=  0	(not -0.5)
;;				-5  / -10	=  0	(not -0.5)
;;
;; !!!!! IMPORTANT !!!!!
;; FOLLOW THE PSEDUOCODE PROVIDED IN THE PROJECT PDF 

	;; YOUR IMPLEMENTATION HERE
.orig x3060
my_div
AND R2, R2, 0		;set result to 0
AND R3, R3, 0		;set negate to 0
ADD R0, R0, 0
BRzp POSA			;checks if A is not negative
ADD R3, R3, -1		;set negate = negate - 1
ADD R0, R0, -1
NOT R0, R0			;convert A to positive
POSA
ADD R1, R1, 0
BRzp POSB			;checks if B is not negative
ADD R3, R3, 1		;set negate = negate + 1
ADD R1, R1, -1
NOT R1, R1			;convert B to positive
POSB
NOT R1, R1
ADD R1, R1, 1		;convert B to negative
WHILELOOP
ADD R4, R0, R1		;performs A-B to check if A >= B
BRn ENDD			;if check fails, end loop
ADD R2, R2, 1		;set result = result + 1
ADD R0, R0, R1		;set A = A - B
BR WHILELOOP		;goes back to start of WHILELOOP
ENDD
ADD R3, R3, 0		;checks if R3 is != 0
BRz FINISH
ADD R2, R2, 0		;checks if result is positive or negative
BRn NEG
NOT R2, R2
ADD R2, R2, 1		;converts result from positive to negative
BR FINISH
NEG
ADD R2, R2, -1
NOT R2, R2			;converts result from negative to positive
FINISH
RET
.end




;; Evaluates an arithmetic expression that is in RPN form
;;
;; ## inputs			
;; R0 = Base address of a string that contains an arithmetic
;; 	 expression in postfix notation 
;; 
;; ## outputs
;; R1 = The result of computing the arithmetic expression
;; 
;; ## input constraints
;;
;; - Your postfix expression will only consist of **postive single digit** integers
;;   and operators. It won't contain illegal characters, parentheses, negative
;;   numbers, multiple-digit numbers, floating point numbers, or heck, even complex
;;   numbers. Just pure good old **positive single digit** integers!
;; 
;; - You will always get a valid postix expression. 
;; 
;; ## implementation details
;;
;; Let's say we want to calculate (1-(5*8+2)). Since our calculator works on
;; postfix notation, we will need to convert this infix expression to postfix.
;; Doing this will give us (1 5 8 * 2 + -). The string version of this expression
;; is "158*2+-". Note that there are no spaces between the digits. Remember, your
;; expression will only consist of **postive single digit** integers, so you don't
;; have to worry about supporting negative integers, or multiple-digit integers.
;; 
;; The subroutine `rpn` will receive the base address of the string "158*2+-".
;; For example, if your string is stored starting at memory location 0x4000, `rpn`
;; will receive 0x4000 through the R0 register.
;; 
;; You will now need to evaluate this expression by iterating over the string, and
;; carrying out the operations outlined in the "Background" section of the project
;; pdf
;; 
;; Finally, once you are done, you will need to save the final result in the
;; register R1.
;;
;; ## hints
;; - There are some useful labels at the bottom of this file. For example, we have
;;   given you a label that refers to the ASCII value of '+'. You will find this
;;   useful when trying to determine which operation to carry out. We have also
;;   provided you the ASCII value of the character '0'.
;; - There are some test postfix strings starting at x4000. You can access these via
;;	 EXP_LOC, and can use these strings when trying to develop the calculator initially
;; - Figure out how to run subroutines in complx. It will save you a TON of time
;;   during debugging!

	;; YOUR IMPLEMENTATION HERE	
.orig x3100
rpn
LD R6, STACK
AND R1, R1, 0				;Set R1 to 0
LOOP
LDR R3, R0, 0				;loads string character at address into R3
BRz ENDOF                   ;if character was 0, jump to ENDOF because 0 indicates end of string
LD R4, PLUS					;Load ASCII Value of + into R4
NOT R4, R4
ADD R4, R4, 1
ADD R4, R3, R4				;perform string character - ASCII value
BRnp NOTPLUS				;if R4 = 0, then the string character is equal to +
ADD R6, R6, -1
STR R0, R6, 0				;push the current address of the string onto the stack
ADD R6, R6, -1
STR R1, R6, 0				;pushes the current value of the expression
LDR R1, R6, 2				;Loads the first number off the stack into R1
LDR R0, R6, 3				;Loads the second number off the stack into R0
ADD R6, R6, -1
STR R7, R6, 0
JSR my_add                  ;jumps to subroutine my_add
LDR R7, R6, 0
ADD R6, R6, 1
LDR R1, R6, 0
ADD R6, R6, 1				;restores R1 by popping the stack
LDR R0, R6, 0
ADD R6, R6, 1				;restores R0 by popping the stack
ADD R6, R6, 2				;pops the two numbers that were used in the subroutine off the stack
ADD R6, R6, -1
STR R2, R6, 0				;push the result of the subroutine onto the stack
BR FINISHR
NOTPLUS
LD R4, MINUS				;Load ASCII Value of - into R4
NOT R4, R4
ADD R4, R4, 1
ADD R4, R3, R4				;perform string character - ASCII value
BRnp NOTMINUS				;if R4 = 0, then the string character is equal to -
ADD R6, R6, -1
STR R0, R6, 0				;push the current address of the string onto the stack
ADD R6, R6, -1
STR R1, R6, 0				;pushes the current value of the expression
LDR R1, R6, 2				;Loads the first number off the stack into R1
LDR R0, R6, 3				;Loads the second number off the stack into R0
ADD R6, R6, -1
STR R7, R6, 0
JSR my_sub                  ;jumps to subroutine my_sub
LDR R7, R6, 0
ADD R6, R6, 1
LDR R1, R6, 0
ADD R6, R6, 1				;restores R1 by popping the stack
LDR R0, R6, 0
ADD R6, R6, 1				;restores R0 by popping the stack
ADD R6, R6, 2				;pops the two numbers that were used in the subroutine off the stack
ADD R6, R6, -1
STR R2, R6, 0				;push the result of the subroutine onto the stack
BR FINISHR
NOTMINUS
LD R4, MULT					;Load ASCII Value of * into R4
NOT R4, R4
ADD R4, R4, 1
ADD R4, R3, R4				;perform string character - ASCII value
BRnp NOTMULT				;if R4 = 0, then the string character is equal to *
ADD R6, R6, -1
STR R0, R6, 0				;push the current address of the string onto the stack
ADD R6, R6, -1
STR R1, R6, 0				;pushes the current value of the expression
LDR R1, R6, 2				;Loads the first number off the stack into R1
LDR R0, R6, 3				;Loads the second number off the stack into R0
ADD R6, R6, -1
STR R7, R6, 0
JSR my_mul                  ;jumps to subroutine my_mul
LDR R7, R6, 0
ADD R6, R6, 1
LDR R1, R6, 0
ADD R6, R6, 1				;restores R1 by popping the stack
LDR R0, R6, 0
ADD R6, R6, 1				;restores R0 by popping the stack
ADD R6, R6, 2				;pops the two numbers that were used in the subroutine off the stack
ADD R6, R6, -1
STR R2, R6, 0				;push the result of the subroutine onto the stack
BR FINISHR
NOTMULT
LD R4, DIVIDE				;Load ASCII Value of / into R4
NOT R4, R4
ADD R4, R4, 1
ADD R4, R3, R4				;perform string character - ASCII value
BRnp OPEND					;if R4 = 0, then the string character is equal to /
ADD R6, R6, -1
STR R0, R6, 0				;push the current address of the string onto the stack
ADD R6, R6, -1
STR R1, R6, 0				;pushes the current value of the expression
LDR R1, R6, 2				;Loads the first number off the stack into R1
LDR R0, R6, 3				;Loads the second number off the stack into R0
ADD R6, R6, -1
STR R7, R6, 0
JSR my_div                  ;jumps to subroutine my_div
LDR R7, R6, 0
ADD R6, R6, 1
LDR R1, R6, 0
ADD R6, R6, 1				;restores R1 by popping the stack
LDR R0, R6, 0
ADD R6, R6, 1				;restores R0 by popping the stack
ADD R6, R6, 2				;pops the two numbers that were used in the subroutine off the stack
ADD R6, R6, -1
STR R2, R6, 0				;push the result of the subroutine onto the stack
BR FINISHR
OPEND
ADD R3, R3, -16
ADD R3, R3, -16
ADD R3, R3, -16				;Subtract 48 from the ASCII value to convert to integer value
ADD R6, R6, -1				;Since the string character is not an operator, it must be a number
STR R3, R6, 0				;pushes the number onto the stack
FINISHR
ADD R0, R0, 1				;increment STRING address by 1 to move on to the next character
BR LOOP						;loops back to beginning
ENDOF
ADD R1, R2, 0				;sets R1 to the finished value of the expression.
RET
;; some useful labels 

;; ASCII value for important characters
PLUS	.fill 43	;; +
MINUS	.fill 45	;; -
MULT	.fill 42	;; *
DIVIDE	.fill 47	;; /
ZERO	.fill x30	;; 0

;; initial stack pointer
STACK	.fill xf000
EXP_LOC .fill x4000
.end

.orig x4000
.stringz "34+"		;; expected output => R1 = 7
;; EXP	.stringz "54-3+"	;; expected output => R1 = 2 
;; EXP	.stringz "58*42/-"	;; expected output => R1 = 38 
.end