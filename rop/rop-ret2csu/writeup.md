## Analyse

### 1) Reconnaissance statique

On remarque que :

- `gets` va nous permettre d'écrire dans la stack,
- `scanf` quant elle nous permettra d'écrire dans `.bss`

```zsh
󰣇 offsec/rop/rop-ret2csu   main  !⇡1 ❯ readelf -r ./6-rop-05-ret2csu | grep execve
000000403fc8  000300000007 R_X86_64_JUMP_SLO 0000000000000000 execve@GLIBC_2.2.5 + 0
```

### 2) Analyse dynamique

```asm
Dump of assembler code for function __libc_csu_init:
   0x00000000004012d0 <+0>:	endbr64
   0x00000000004012d4 <+4>:	push   r15
   0x00000000004012d6 <+6>:	lea    r15,[rip+0x2ac3]        # 0x403da0
   0x00000000004012dd <+13>:	push   r14
   0x00000000004012df <+15>:	mov    r14,rdx
   0x00000000004012e2 <+18>:	push   r13
   0x00000000004012e4 <+20>:	mov    r13,rsi
   0x00000000004012e7 <+23>:	push   r12
   0x00000000004012e9 <+25>:	mov    r12d,edi
   0x00000000004012ec <+28>:	push   rbp
   0x00000000004012ed <+29>:	lea    rbp,[rip+0x2abc]        # 0x403db0
   0x00000000004012f4 <+36>:	push   rbx
   0x00000000004012f5 <+37>:	sub    rbp,r15
   0x00000000004012f8 <+40>:	sub    rsp,0x8
   0x00000000004012fc <+44>:	call   0x401000 <_init>
   0x0000000000401301 <+49>:	sar    rbp,0x3
   0x0000000000401305 <+53>:	je     0x401326 <__libc_csu_init+86>
   0x0000000000401307 <+55>:	xor    ebx,ebx
   0x0000000000401309 <+57>:	nop    DWORD PTR [rax+0x0]
   0x0000000000401310 <+64>:	mov    rdx,r14 ; g_caller <----------------------
   0x0000000000401313 <+67>:	mov    rsi,r13
   0x0000000000401316 <+70>:	mov    edi,r12d
   0x0000000000401319 <+73>:	call   QWORD PTR [r15+rbx*8] ; r15 -> execve@got -> execve@plt
   0x000000000040131d <+77>:	add    rbx,0x1
   0x0000000000401321 <+81>:	cmp    rbp,rbx
   0x0000000000401324 <+84>:	jne    0x401310 <__libc_csu_init+64>
   0x0000000000401326 <+86>:	add    rsp,0x8
   0x000000000040132a <+90>:	pop    rbx ; g_popper <---------------------------
   0x000000000040132b <+91>:	pop    rbp
   0x000000000040132c <+92>:	pop    r12 ; rdi
   0x000000000040132e <+94>:	pop    r13 ; rsi
   0x0000000000401330 <+96>:	pop    r14 ; rdx
   0x0000000000401332 <+98>:	pop    r15
   0x0000000000401334 <+100>:	ret
End of assembler dump.

```


```asm
gef➤  grep /bin/sh
[+] Searching '/bin/sh' in memory
[+] In '/usr/lib/libc.so.6'(0x7ffff7d95000-0x7ffff7e04000), permission=r--
  0x7ffff7dafebc - 0x7ffff7dafec3  →   "/bin/sh"
```

### 3) Exploit

Stratégie : ROP - ret2csu
1. Overflow via gets(name) → prendre le contrôle de RIP dans main et utiliser ret2csu avec :
    - rbx = 0
    - rbp = 1
    - r12 = &execve@got
    - r13 = (uintptr_t)"/bin/sh"
    - r14 = 0
    - r15 = 0 (envp = NULL)

2. Introduire `/bin/sh\0` dans `.bss`  
