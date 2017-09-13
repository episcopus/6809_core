;; 8-BIT DATA TRANSFER
;; Purpose: Move the contents of memory location 0040 to memory location 0041.

        org     $5000
        fcb     $69

        org     $2000
        lda     $5000
        sta     $5001
        end     $2000
