global _start
section .text
_start:
  mov rdi, "/bin/sh"
  push rdi
  mov rdi, rsp
  push 0 
  pop rax
  imul rsi, rax
  mov al, 0x3b
  syscall
