;; =============================================================
;; CS 2110 - Fall 2020
;; Project 3 - Alternating Sum
;; =============================================================
;; Name: Tien Tran
;; =============================================================

.orig x3000
LD R0, ARRAY			;load address of ARRAY into R0
LD R1, LENGTH			;load length of ARRAY into R1
AND R2, R2, 0
AND R4, R4, 0
LOOP
ADD R1, R1, 0			;Checks if there is entries in the array left
BRz END                 ;if not, jumps to end of loop
LDR R3, R0, 0			;entry of array is loaded into R3
ADD R2, R2, 0			;R2 is used to check for array index. If R2 = 0, then even index. If R2 = 1, then odd index.
BRp ODD
ADD R2, R2, 1			;Sets R2 to 1 so that in the next loop we know that the array index is odd.
BR FINISH
ODD
NOT R3, R3
ADD R3, R3, 1			;turns the entry of the array to negative
AND R2, R2, 0			;sets R2 to 0 so that in the next loop we know that the array index is even.
FINISH
ADD R4, R4, R3			;Adds the entry of array to the total sum. If the array index is odd, it adds the negative entry of the array.
ADD R0, R0, 1			;increments array index by 1
ADD R1, R1, -1			;decrements array length by 1
BR LOOP                 ;goes back to beginning of loop
END
ST R4, RESULT			;stores total sum in RESULT
HALT

LENGTH      .fill 4
ARRAY       .fill x7000
RESULT      .blkw 1

.end


.orig x7000
    .fill 5
    .fill 3
    .fill 7
    .fill 2
.end
