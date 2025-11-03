---
title: "jjjjjjjjjjjjj"
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

- commandes & observations
- on voudrait jump sur system, mais à cause de la randomisation des adresses et du fait que notre buffer est sur la pile, on
  ne peut pas jump sur system.
- le buffer est en dessous de la stackframe de getstr, et en particulier de saved_rip ; donc on ne peut pas l'atteindre

### 2) Analyse dynamique

- breakpoints, comportement runtime
- On rédige le sc suivant :

```asm
global _start

section .text
_start:
  xor rax, rax
  imul rax, rsi
  mov al, 59
  mov rdi, sc
  syscall

section .data
  sc: db "/bin/sh"
```

Mais lors du `disas /r _start` on constate que ce dernier contient des _null bytes_ et adresses surtout lors du mov.

```zsh
Dump of assembler code for function _start:
   0x0000000000401000 <+0>:     48 31 c0                xor    rax,rax
   0x0000000000401003 <+3>:     48 0f af c6             imul   rax,rsi
   0x0000000000401007 <+7>:     b0 3b                   mov    al,0x3b
   0x0000000000401009 <+9>:     48 bf 00 20 40 00 00 00 00 00   movabs rdi,0x402000
   0x0000000000401013 <+19>:    0f 05                   syscall
End of assembler dump.
```

- On pourrait écrire bin sh puis le sc puis l'add, mais c'est trop gourmand en taille
- On peut le stocker sur la stack

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
