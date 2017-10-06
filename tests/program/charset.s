	;; CHAR SET
	;; Purpose: Display 256 first characters of AI / SG6 mode on default
	;; video screen of $0400
	;;

        ORG     $1000
        LDX     VIDBUF          ; Start of video buffer
        CLRA                    ; Start at character 0
LOOP1   STA     ,X+             ; Store char in video buffer, incr ptr
        INCA                    ; Next character
        BNE     LOOP1           ; Keep on going 255 more times, until A rolls over (to 0)
        LDB     #$60            ; Now clear the rest of the screen with space char
        CLRA
LOOP2   STB     ,X+             ; Store char in video buffer, incr ptr
        INCA                    ; Here A is just a loop counter
        BNE     LOOP2           ; Keep on going 255 more times, until A rolls over (to 0)
        SWI                     ; Done
VIDBUF  FDB     $0400
        END     $1000
