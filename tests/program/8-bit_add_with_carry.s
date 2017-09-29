;; EIGHT BIT ADD WITH CARRY
;; Purpose: Add two four byte integers (in $3000 and $3004) and store the result
;;  in $3000.

        ORG     $3000
        FCB     $00,$80,$27,$FF
        FCB     $83,$A0,$7A,$11

        ORG     $0100
MPADDS  LDX     #$3003  ; Point to OP1+3 bytes
        LDY     #$3007  ; Point to OP2+3 bytes
        LDB     #4      ; Loop counter
        ANDCC   #$FE    ; Clear carry
MPA010  LDA     ,X      ; Get OP1
        ADCA    ,Y      ; Add in OP2
        STA     ,X      ; Store result
        LEAX    -1,X    ; Decrement OP1 pointer
        LEAY    -1,Y    ; Decrement OP2 pointer
        DECB            ; Decrement count
        BNE     MPA010  ; Loop if not 4
        SWI
        END     $0100
