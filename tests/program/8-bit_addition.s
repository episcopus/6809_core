;; 8-BIT ADDITION
;; Purpose: Add the contents of memory locations $5000 and $5001, and place the
;; result in memory location $5002.

        org     $5000
        fcb     $38,$2B

        org     $2000
        lda     $5000
        adda    $5001
        sta     $5002
        swi
        end     $2000
