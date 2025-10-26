---
title: "stack-ret2shell-canary"
challenge: "ctf"
difficulty: "3"
platform: "amd64/remote"
date: "YYYY-MM-DD"
tags: [binary, canary]
author: "Ryan Bouchou"
status: "in-progress"
---

# Titre lisible

**Résumé (1-2 lignes)**

---

## Contexte

- Source : cours
- Environnement testé : Arch Linux, amd64, glibc
- Fichiers fournis : main.c

---

## Objectif

Obtenir un shell

---

## Outils

- gdb + gef
- pwntools (python3)
- objdump

---

## Analyse

### 1) Reconnaissance statique

En désassemblant le programme :

- On constate la présence d'un canary `0xcafecafecafe` situé à `$rbp - 0x10`.
- Le buffer est situé à `$rbp - 0x1f`.

Ainsi, ceci nous laisse des champs d'écriture de :

- $15$ octets à partir de l'adresse du buffer,
- $16$ octets entre le canary et la savegarde du pointeur d'instruction.

Concernant la méthode de capture de l'input, on constate que le programme utilise `gets`, ainsi, nous n'avons pas de contraintes concernant la présence ou non de *null byte* au sein de notre payload.  

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
