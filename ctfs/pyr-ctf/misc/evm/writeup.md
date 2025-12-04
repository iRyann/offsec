---
title: "EVM Bytecode"
challenge: "root-me/picoCTF"
difficulty: "Easy"
platform: "amd64/remote"
date: "YYYY-MM-DD"
tags: [binary, overflow]
author: "Ryan Bouchou"
status: "in-progress"
---

# Titre lisible

**Résumé (1-2 lignes)**
Résumé court...

---

## Contexte

- Source : root-me / cours / picoCTF
- Environnement testé : Ubuntu 22.04, amd64, glibc 2.35
- Fichiers fournis : vuln, main.c, libc.so

---

## Objectif

Récupérer la flag / obtenir un shell

---

## Outils

- gdb + gef / pwndbg
- pwntools (python3)
- readelf / objdump / strings / file

---

## Analyse

### 1) Reconnaissance statique

On convertit le bytecode en opcodes :

```evm
  0x0: PUSH0
    0x1: CALLVALUE
    0x2: PUSH2     0xbeef
    0x5: MUL
    0x6: PUSH2     0xff
    0x9: MOD
    0xa: ISZERO
    0xb: PUSH1     0x20
    0xd: JUMPI
    0xe: PUSH1     0x0
   0x10: SELFDESTRUCT
   0x11: CALLVALUE
   0x12: PUSH1     0xa
   0x14: ADD
   0x15: PUSH3     0xb8cf81
   0x19: EQ
   0x1a: PUSH1     0x30
   0x1c: JUMPI
   0x1d: PUSH1     0x1
   0x1f: SELFDESTRUCT
   0x20: STOP
```

### 2) Analyse dynamique

- breakpoints, comportement runtime

### 3) Exploit

Stratégie : ret2libc / overflow / format-string

Payload (extrait) :

```py
from pwn import *
context.update(arch='amd64', timeout=2)
p = process('./build/a.out')
p.sendline(b'...')
p.interactive()
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

- readelf -a binary

## Artefacts

- exploit.py, build/
