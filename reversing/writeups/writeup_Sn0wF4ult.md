---
title: "Sn0wF4lt"
challenge: "nil"
difficulty: "Hard"
platform: "amd64"
date: "2025-12-04"
tags: [binary, reverse]
author: "Ryan Bouchou"
status: "in-progress"
---

# Titre lisible

**Résumé**

---

## Contexte

- Source : Indéterminée
- Environnement testé : Kali
- Fichiers fournis : vuln

---

## Objectif

Récupérer le bon mot de passe

---

## Outils

- gdb + gef / pwndbg
- pwntools (python3)
- readelf / objdump / strings / file

---

## Analyse

### 1) Reconnaissance statique

On commence par la vérification classique des protections du binaire :

```zsh
┌──(bob㉿kali)-[~/lab/reverse/crackme-analysis/bin]
└─$ checksec --format=json --file=Sn0wF4ult | jq
{
  "Sn0wF4ult": {
    "relro": "no",
    "canary": "no",
    "nx": "yes",
    "pie": "no",
    "rpath": "no",
    "runpath": "no",
    "symbols": "no",
    "fortify_source": "no",
    "fortified": "0",
    "fortify-able": "0"
  }
}
```

```zsh
$ file Sn0wF4ult
Sn0wF4ult: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), statically linked, no section header
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
