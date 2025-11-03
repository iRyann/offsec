---
title: "4-printf-04-write2"
challenge: "Neal"
difficulty: "5"
platform: "amd64/remote"
date: "2025-11-02"
tags: [binary, got_overwrite]
author: "Ryan Bouchou"
status: "in-progress"
---

# Titre lisible

**Résumé (1-2 lignes)**
Résumé court...

---

## Contexte

- Source : cours
- Environnement testé : Arch Linux, amd64
- Fichiers fournis : vuln, main.c

---

## Objectif

Récupérer le flag en obtenant un shell

---

## Outils

- gdb + gef
- pwntools (python3)
- checksec

---

## Analyse

### 1) Reconnaissance statique

On dispose de 150 octets, situés au niveau de `rsp` -- `main` n'ayant qu'une seule variable dans sa portée -- afin d'écrire l'adresse de `success` au niveau de `saved_rip`. On utilise `checksec` pour en apprendre plus sur les statégies de sécurité mise en oeuvre par le programme :

```zsh
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols      FORTIFY  Fortified       Fortifiable     FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   71 Symbols     No     0               1               4-printf-05-write3
```

Manifestement :

- **PIE** : En l'absence de PIE, les adresses du binaire sont fixes.
- **RELRO** : En mode _partiel_, la GOT est modifiable. À l'inverse, en mode _full_, on ne peut pas écraser celle-ci.
- **Canary** : Présent, mais pas génant dans le cadre de l'exploitation à venir puisqu'on ne fait pas de buffer overflow.

### 2) Analyse dynamique

En exécutant le programme avec GDB, on peut déterminer l'addresse de `success` avec la commande `info functions` : on obtient `0x0000000000401176  success`.
Ensuite, en faisant `disas main`, on note l'adresse d'une fonction de la GOT à réécrire qui intervient après `printf(buf)` (_e.g._ `puts`).

### 3) Exploit

Stratégie : format-string
On écrit l'adresse de retour à l'emplacement d'`rsp`, ie. du buffer, puis on utilise `%6$p` pour écrire à l'adresse spécifie par les huit premiers octets du buffer l'adresse de la fonction `success`. Pour ce faire, on écrit l'équivalent décimal d'espace avec `%<number-8>c`.

1. On leak l'adresse de retour :

```python
payload = b'%6$p'
io.sendlineafter(b'nom: ', payload)
io.interactive()
```

On note que `rsp`, et à fortiori `buf` se situe à `0x7f0070243625`  
2. On construit un payload de la façon suivante : `saved_rip_addr` + `%118c` + `%6$n` + `saved_rip_addr + 1` + `%17c` + `%6$n` + `saved_rip_addr + 2` + `%64c` + `%6$n`

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
