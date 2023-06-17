;; =============================================================
;; CS 2110 - Fall 2020
;; Project 3 - Compare
;; =============================================================
;; Name: Tien Tran
;; =============================================================

.orig x3000
LD R1, A
LD R2, B
ADD R1, R1, 0	
BRn NEG1				;code below handles cases where A is positive
ADD R2, R2, 0
BRn NEG2                ;checks if B is negative
NOT R0, R2				;performs A-B in order to compare the different results
ADD R0, R0, 1
ADD R0, R0, R1
BRz EQUAL
BRn R1LESS				
AND R1, R1, 0			;if B is less, store 1 in RESULT
ADD R1, R1, 1
ST R1, RESULT			
BR END
EQUAL
AND R1, R1, 0			;if A-B=0, then store 0 in RESULT
ST R1, RESULT
BR END
R1LESS					;if A is less, then store -1 in RESULT
AND R1, R1, 0
ADD R1, R1, -1
ST R1, RESULT
BR END
NEG2					;if B is negative and A is positive, then A > B so store 1
AND R1, R1, 0
ADD R1, R1, 1
ST R1, RESULT
BR END
NEG1					;branches to cases where A is negative
ADD R2, R2, 0
BRn NEGB				;checks if B is negative when A is negative
AND R1, R1, 0			;if B is positive and A is negative, A < B so store -1
ADD R1, R1, -1
ST R1, RESULT
BR END
NEGB
NOT R0, R2				;performs A-B in order to compare the different results
ADD R0, R0, 1
ADD R0, R0, R1
BRz EQUAL
BRn R1LESS
AND R1, R1, 0			;if B is less, store 1 in RESULT
ADD R1, R1, 1
ST R1, RESULT
BR END
END
HALT

A       .fill 3
B       .fill -2
RESULT  .blkw 1
.end
