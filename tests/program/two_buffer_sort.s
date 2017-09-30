;; TWO BUFFER SORT
;; Purpose: Tests subroutine functionality by sorting the first half of the screen
;; buffer in the second half.

        ORG     $0400
        FCN     "EPISCOPUS"

        ORG     $0100
SORT    LDY     #$400+256       ; Second half of screen
        LDB     #0              ; 256 byte count
SOR010  PSHS    B               ; Save count
        LDX     #$400           ; Point to first half
        LDB     #0              ; 256 bytes
        BSR     SCANTY          ; Find lowest entry
        BSR     STORE           ; Store in second half
        BSR     MARK            ; Delete first buf entry
        PULS    B               ; Get count
        DECB                    ; Decrement
        BNE     SOR010          ; Loop if not done
        SWI

        ;; Scan table for smallest entry subroutine
        ;; Input: X: pointer to table
        ;;        B: size of table 1-255, 0=256
        ;; Output: X: pointer to smallest entry in table
        ;;         A: smallest entry
SCANTY  LDA     #$FF            ; Set up lowest
        STA     LOWEST          ; Set lowest
        LDA     ,X              ; Get first byte
        PSHS    X               ; Initialize stack
SCN010  LDA     ,X              ; Get byte
        CMPA    LOWEST          ; Compare with lowest
        BHS     SCN020          ; Go if C>=A
        STA     LOWEST          ; New lowest
        PULS    U               ; Previous pointer
        PSHS    X               ; Save this location
SCN020  LEAX    1,X             ; Point to next byte
        DECB                    ; Decrement count
        BNE     SCN010          ; Go if not end
        PULS    X               ; Get pointer
        LDA     LOWEST          ; Lowest in A
        RTS                     ; Return
LOWEST  FCB     0               ; Holds lowest at end

        ;; Store entry in A into (Y) location
STORE   STA     ,Y+             ; Store entry
        RTS                     ; Return

        ;; Mark old entry with -1
MARK    LDA     #$FF            ; -1
        STA     ,X              ; Store -1
        RTS                     ; Return
        END     $0100
