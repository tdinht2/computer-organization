;; =============================================================
;; CS 2110 - Fall 2020
;; Project 3 - Modulus
;; =============================================================
;; Name: Tien Tran
;; =============================================================

.orig x3000
LD R1, A
LD R2, B
ADD R1, R1, 0
BRzp NOTNEG				;checks if A is negative
NOT R1, R1				;takes the absolute value if A is negative
ADD R1, R1, 1
NOTNEG
ADD R2, R2, 0
BRzp NOTNEG2			;checks if B is negative
NOT R2, R2				;takes the absolute value is B is negative
ADD R2, R2, 1
NOTNEG2
NOT R0, R2
ADD R0, R0, 1
CONDITION				;performs A-B to check if A >= B
ADD R2, R1, R0
BRn END                 ;if the check fails, then ends the loop
ADD R1, R1, R0			;sets R1 equal to A - B
BR CONDITION			;loops back to beginning of loop to check condition again
END
ST R1, RESULT			;stores finished value of A%B into RESULT
HALT

A       .fill 10
B       .fill 3
RESULT  .blkw 1
.end
