;; 8-BIT DATA TRANSFER
;; Purpose: Move the contents of memory location $5000 to memory location $5001.

        org     $5000
        fcb     $69

        org     $2000
        lda     $5000
        sta     $5001
        end     $2000
