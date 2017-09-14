;; TABLE OF SQUARES
;; Calculate the square of the contents of memory location 0041 from a table
;; and place the square in memory location 0042. Assume that memory location 0041
;; contains a number between 0 and 7 inclusive that is, 0 < (0041) < 7. The table
;; occupies memory locations 0050 through 0057.

        org     $41
        fcb     3
        org     $50
        fcb     0,1,4,9,16,25,36,47

        org     $0
        ldb     $41
        ldx     #$50
        lda     b,x
        sta     $42
        end
