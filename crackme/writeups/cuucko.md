# crackme

This repository contains the `cuckoo` crackme challenge. The embedded password
can be recovered by reversing the `scramble` routine in the binary:

```
out_byte = ((in_byte ^ 0x3d) + 0x42) & 0xff
```

The target byte sequence that `scramble` must produce resides at offset
`0x203a` in the `.rodata` section. Undoing the transformation byte-for-byte
reveals the expected input:

```
car8-n0t3__
```

To actually run the challenge you must first mark the ELF as executable (the
checked-in copy ships without the execute bit):

```
chmod +x cuckoo
```

With the permissions fixed, supplying the recovered password causes the program
to issue a very small success banner – but it **will not** appear on the
terminal. The custom `_start` routine opens `/tmp/l1` and duplicates the file
descriptor onto standard output before `main` ever executes, so any data written
to `STDOUT_FILENO` ends up in that file instead of your console. A successful
session therefore looks like this:

```
$ ./cuckoo car8-n0t3__
$ cat /tmp/l1
OK
```

The message is still written through `write(1, "OK\n", 3)` (see the
`call   1070 <strcmp@plt>` / `call   1030 <write@plt>` sequence in `main`), but
because of the early redirection you must read `/tmp/l1` to observe the
`OK` banner.

### Where `_start` rewires stdout

If you want to see the responsible instructions directly, `objdump -d -M intel
cuckoo` shows the following fragment inside `_start`:

```asm
    10cd:   movabs rdx,0xffce93d08f928bd0
    10d7:   push   rax
    10d8:   lea    rax,[rax+0x2]
    10dc:   push   rdx
    10dd:   mov    esi,0x42
    10e2:   add    rsp,0x10
    10e6:   xor    rdx,rdx
    10e9:   lea    rdi,[rsp-0x10]
    10ee:   dec    rdx
    10f1:   not    QWORD PTR [rsp-0x10]
    10f6:   call   rbx
    10f8:   mov    rdi,rax
    10fb:   xor    eax,eax
    10fe:   or     eax,0x21
    1101:   push   0x1
    1104:   pop    rsi
    1105:   call   rbx
```

The constant loaded at `0x10cd` is the bitwise inverse of the bytes for
`"/tmp/l1\0"`. The `not` instruction flips them back into place on the stack, so
the subsequent `call   rbx` issues `open("/tmp/l1", O_WRONLY|O_CREAT, 0xffffffffffffffff)`.
The second `call   rbx` runs with `eax = 0x21` (Linux x86_64 syscall `dup2`),
`rdi = <open result>`, and `rsi = 1`, thereby duplicating the returned file
descriptor onto `STDOUT_FILENO` before `main` ever runs.

## Verifying with pwntools

If you prefer to script the interaction, the repository now includes
`pwn_check.py`, a tiny pwntools harness that launches `cuckoo`, supplies the
unscrambled password, and asserts that the binary leaves `OK\n` inside
`/tmp/l1`. Install [`pwntools`](https://docs.pwntools.com/en/stable/install.html)
in your Python environment and run:

```
python pwn_check.py
```

The script reuses the same `unscramble` helper defined in `solve.py`, clears any
leftover `/tmp/l1`, launches the crackme, and then reads the redirected output
to confirm the presence of `OK\n`.
