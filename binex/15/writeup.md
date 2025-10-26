---
title: "ret2sc main"
challenge: "root-me/picoCTF"
difficulty: "5"
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
