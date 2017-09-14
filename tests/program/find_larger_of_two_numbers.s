;; FIND LARGER OF TWO NUMBERS
;; Place the larger of the contents of memory locations 0040 and 0041 in memory
;; location 0042. Assume that memory locations 0040 and 0041 contain unsigned
;; binary numbers.

        org     $5000
        fcb     $3F,$2B

        org     $2000
        lda     $5000
        cmpa    $5001
        bhs     STRES
        lda     $5001
STRES   sta     $5002
        end     $2000
