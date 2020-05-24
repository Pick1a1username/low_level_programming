; %macro colon 2
;   %2:
;     dq 0
;     dq %1
;     dq %3
; %endmacro

%define lw 0

%macro colon 2
%%lw: dq lw 
db %1, 0
xt_ %+ %2:

%define lw %%lw
%endmacro


; section .data
; 
; colon "third word", third_word
; db "third word explanation", 0
; colon "second word", second_word
; db "second word explanation", 0
; colon "first word", first_word 
; db "first word explanation", 0

