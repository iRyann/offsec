global _start

section .text
_start:
  mov rdi, "//bin/sh"
  shr rdi, 8
  jmp rsi
