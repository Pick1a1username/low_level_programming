; The following commands' results differ:
; $  nasm -E defining_in_cla.asm
; $  nasm -E -d flag defining_in_cla.asm

%ifdef flag

hellostring: db "Hello", 0
%endif
