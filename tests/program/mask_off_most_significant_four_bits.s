;; MASK OFF MOST SIGNIFICANT FOUR BITS
;; Place the least signicant four bits of memory location $5000 in the least
;; signicant four bits of memory location $5001. Clear the most significant four
;; bits of memory location 0041

        org     $5000
        fcb     $3D

        org     $2000
        lda     $5000
        anda    #%00001111      ; Mask off four MSB's
        sta     $5001
        swi
        end     $2000
