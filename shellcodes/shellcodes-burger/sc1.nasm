global _start

section .text
_start:
  mov rdi, "/sh"
  shr rdi, 8
  jmp rsi
