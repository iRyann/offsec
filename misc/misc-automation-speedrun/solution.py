from pwn import *

context.update(arch="amd64", timeout=0.5)

p = remote("ctf.example.org", 4002)

# solve 40 operations quickly before we get a shell
for x in range(40):
    p.recvuntil(b"Combien font")
    # extract phrase "N1 fois N2 ?", each word in a var
    a, op, b, _ = p.recvS().strip().split()
    a, b = int(a), int(b)
    # solve equation (addition or product)
    res = a + b if op == "plus" else a * b
    log.info(f"solved: {a} {op} {b} = {res}")
    # send result
    p.sendline(str(res).encode())

p.recvline()
p.sendline(b"cat flag.txt")
log.success("FLAG: " + p.recvS().strip())
