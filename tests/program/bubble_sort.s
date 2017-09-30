;; BUBBLE SORT
;; Purpose: Sort characters at the $0400 which is incidentally also the visual
;; display address for the default alphanumerics mode of the Coco.
;;

        ORG     $0400
        FCN     "EPISCOPUS"

        ORG     $0100
BUBSRT  CLR     PASSNO          ; Set pass # to 0
BUB010  LDX     #$400           ; Point to screen
        LDY     #0              ; Set change flag to 0
BUB020  LDA     ,X+             ; Get first entry
        CMPA    ,X              ; Test next
        BLS     BUB030          ; Go if a<=b
        LDB     ,X              ; Get second entry
        STB     -1,X            ; Swap b to a
        STA     ,X              ; Swap a to b
        LDY     #1              ; Set "change"
BUB030  CMPX    #$400+511       ; Test for screen end
        BNE     BUB020          ; Go if not one pass
        INC     PASSNO          ; Increment pass #
        CMPY    #0              ; Test change flag
        BNE     BUB010          ; Go if change occurred
        SWI
PASSNO  FCB     0               ; Pass #
        END     $0100
