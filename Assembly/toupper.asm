;; =============================================================
;; CS 2110 - Fall 2020
;; Project 3 - To Uppercase
;; =============================================================
;; Name: Tien Tran
;; =============================================================

.orig x3000
LD R0, STRING				;loads address of STRING into R0
LD R1, LOWER_A
LD R2, LOWER_Z
LOOP
LDR R3, R0, 0				;loads STRING character at address into R3
BRz END                     ;if character was 0, jump to END because 0 indicates end of STRING
ADD R4, R3, R1				;checks if character is not less than 97, making sure it is in the lowercase ASCII table values
BRn REP						;if less than 97, skip function because it is not a lowercase letter
ADD R4, R3, R2				;checks if character is not greater than 122, making sure it is in the lowercase ASCII table values
BRp REP						;if greater than 122, skip function because it is not a lowercase letter
ADD R3, R3, -16
ADD R3, R3, -16				;subtract ASCII value of character by 32 to convert to uppercase value
STR R3, R0, 0				;store uppercase character at previous STRING character address
REP
ADD R0, R0, 1				;increment STRING address by 1 to move on to the next character
BR LOOP						;loops back to beginning
END
LD R0, STRING               ;loads address of start of STRING back into R0
PUTS						;prints uppercase STRING to console
HALT

TOUPPER     .fill xdf
LOWER_A     .fill -97
LOWER_Z     .fill -122

STRING      .fill x4000
RESULT      .blkw 1
.end

.orig x4000
.stringz "hAhA"
.end
