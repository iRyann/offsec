---
title: "ret2syscall"
challenge: "neal"
difficulty: "Easy"
platform: "amd64/remote"
date: "2025-12-04"
tags: [binary, overflow, rop]
author: "Ryan Bouchou"
status: "in-progress"
---

# Titre lisible

**Résumé (1-2 lignes)**
Résumé court...

---

## Contexte

- Source : cours
- Environnement testé : Arch, amd64, glibc 2.35
- Fichiers fournis : vuln, main.c

---

## Objectif

Récupérer le flag / obtenir un shell

---

## Outils

- gdb + gef / pwndbg
- pwntools (python3)
- readelf / objdump / strings / file

---

## Analyse

### 1) Reconnaissance statique

Du côté des mécanismes de protection usuels :

```zsh
󰣇 offsec/rop/rop-ret2syscall   main  ✘? ❯ checksec --file=6-rop-02-ret2syscall                                                                                                              ℂ -gcc  19:16
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH	Symbols		FORTIFY	Fortified	Fortifiable	FILE
Full RELRO      No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   72 Symbols	  No	0		3		6-rop-02-ret2syscall
```

Exploitation :

- Full RELRO implique que la GOT est en lecture seule, de fait, pas de ret2plt envisageable.
- NX enabled implique que la stack ne permet pas de loger un shellcode exécutable.
- Les symbôles ne sont pas strippé. Fortifiable est à 3.

> [!help] Question
> Le binaire est facilement lisible. F3 ?
> Quelles sont les implications dans la mise en place d'un stratégie ROP ?

Ainsi, le ROP semble, sans surprise, tout indiqué.

### 2) Analyse dynamique

```zsh
gef➤  grep /bin/sh
[+] Searching '/bin/sh' in memory
[+] In '/home/ryan/dev/lab/offsec/rop/rop-ret2syscall/6-rop-02-ret2syscall'(0x402000-0x403000), permission=r--
  0x402013 - 0x40201a  →   "/bin/sh"
```

Le buffer se situe à `$rbp - 0x70`.

### 3) Exploit

Stratégie : ROP

1. Initialiser les registres
   Dans le cadre d'un `execve`, l'état des registres au moment du `syscall` doit être le suivant :
   - `rax=3B`
   - `rdi->"/bin/sh\0"`
   - `rsi=0`
   - `rdx=0`

On détermine les gadgets :

```zsh
󰣇 offsec/rop/rop-ret2syscall   main  ✘? ❯ ROPgadget --binary 6-rop-02-ret2syscall | grep rdi
0x00000000004012e3 : pop rdi ; ret
```

Similairement, on obtient :

- `0x00000000004012e1 : pop rsi ; pop r15 ; ret`
- `0x00000000004011d6 : xor edx, edx ; ret`

2. Procéder au `syscall`

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
