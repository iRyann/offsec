add al, byte ptr [rax]
add byte ptr ds:[rbx + 0x55a95d5b], bh
.byte 0xf0
.byte 0xea
add al, 0xeb
or al, byte ptr [rax]
add byte ptr [rax], bh
add byte ptr [rax], al
add byte ptr [rax], al
add byte ptr [rax], al
add bl, al
mov al, 0x3c
mov edi, esp
syscall
xchg cl, al
ret
stosd dword ptr [rdi], eax
outsb dx, byte ptr [rsi]
xlatb
jno 0x6f
add byte ptr [rcx], al
add byte ptr [rax], al
add byte ptr [rbp + 0x58c35f], al
add byte ptr [rax], al
add byte ptr [rax], al
add byte ptr [rax], al
add byte ptr [rax], al
.byte 0xf0
.byte 0xea
add al, 0xeb
or al, byte ptr [rax]
add byte ptr [rdi + 0x4b], cl
or cl, byte ptr [rbp + 0x5e5d5e05]
call 0x74
add byte ptr [rax], al
add byte ptr [rdi + 0x5c], cl
imul eax, dword ptr [rax], 0
jmp 0x69
.byte 0xf0
pop rsi
popcnt eax, ebx
dec al
jmp 0x6f
pop rax
lodsb al, byte ptr [rsi]
jmp rax
test dword ptr [rcx - 0x5b006726], ecx
and al, 8
