;; SHIFT LEFT 1 BIT
;; Purpose: Shift the contents of memory location 0040 left one bit and place
;; the result in memory location 0041. Clear bit position 0.

        org     $5000
        fcb     $6F

        org     $2000
        ldb     $5000
        aslb
        stb     $5001
        end     $2000
