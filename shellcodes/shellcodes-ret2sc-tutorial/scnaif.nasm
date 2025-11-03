xor rax, rax ; pas besoin de xor rax, car mov écrase les octets de poids fort
; mais utile si mov al
mov rax, 59 ; execve
imul rax, rsi ; stupide
mov rdi, "sh\0\0" ; mov avec chaine en dur ? + on peut pas déléguer à l'ass. de faire exister la chaine ne mémoire. On doit le faire nous même... À détailler. Et traditionnellement on met /bin/sh
syscall ; 
