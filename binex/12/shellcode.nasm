xor rax, rax
mov rax, 59
imul rax, rsi
mov rdi, "sh\0\0"
syscall
