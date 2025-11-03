---
title: "Format string multi-write"
challenge: "Neal — printf GOT overwrite"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-05-15"
tags: [format-string, got-overwrite]
author: "Ryan Bouchou"
status: "solved"
---

# Écrasement de GOT par écritures multiples

**Résumé (1-2 lignes)**  
En plaçant une adresse dans le buffer puis en utilisant `%n`, on écrase l’entrée GOT de `puts` avec `success()` pour lancer `system("sh")` dès le prochain appel.

---

## Contexte

- Source : exercice avancé sur `%n` et la GOT (Neal)
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Rediriger `puts@GOT` vers `success()` afin qu’un appel ultérieur exécute un shell.

---

## Outils

- **GDB + GEF** — traçage dynamique, suivi des registres et de la mémoire.
- **pwntools** — génération de payloads, orchestrations multi-phase et automatisation.
- **NASM** — assemblage de shellcodes personnalisés ou instrumentation précise.
- **Python 3** — scripts d'automatisation, tooling auxiliaire, parsing.
- **C** — code vulnérable, harness minimal, reproductions.
- Autres utilitaires : checksec, file, strings, objdump, angr, etc.

---

## Analyse

### 1) Reconnaissance statique

- `buf` (150 octets) est lu via `scanf("%149s", buf);`, offrant suffisamment d’espace pour placer des pointeurs et du formatage. 【F:format-strings/format-strings-multiwrite/main.c†L38-L47】
- `success()` exécute `system("sh");`, c’est donc la cible à déclencher.
- RELRO est partiel : les entrées GOT restent modifiables, notamment `puts@GOT` appelée implicitement par `printf`.

### 2) Analyse dynamique

- Avec `gdb`, on observe que `buf` débute à `RSP+0x10` et devient le 6ᵉ argument de `printf`, donc `%6$lln` permet d’écrire à l’adresse stockée au début de `buf`.
- `elf.got["puts"]` est résolue à l’exécution ; en l’écrasant par `elf.sym["success"]`, tout appel ultérieur à `puts` (comme celui de `printf`) basculera sur `success`.

### 3) Exploit

> Stratégie : utiliser `fmtstr_payload` pour générer les écritures nécessaires

- On construit le payload via `fmtstr_payload(6, {exe.got["puts"]: exe.sym["success"]})`.
- Ce payload prépare des écritures partielles alignées sur 8 octets pour réécrire l’adresse complète.

```python
payload = fmtstr_payload(6, {exe.got["puts"]: exe.sym["success"]})
io.sendlineafter(b"nom: ", payload)
```

- Après l’envoi, le `printf(buf);` se termine, puis `printf(", bienvenue !\n");` appelle `puts`, désormais détourné vers `success()`, ce qui ouvre le shell. 【F:format-strings/format-strings-multiwrite/solution.py†L9-L18】

---

## Résultat

- Shell interactif, flag récupéré (`CTF{...}`).

## Root cause

La combinaison d’un buffer contrôlé sur la pile, d’un `printf` non sécurisé et d’une GOT réinscriptible permet des écritures arbitraires via `%n`.

## Mitigation

- Activer `FULL RELRO` pour rendre la GOT immuable.
- Utiliser `printf("%s", buf);` ou limiter la longueur de l’entrée.

## Leçons apprises / next steps

- `fmtstr_payload` simplifie grandement les écritures multi-octets ; néanmoins, comprendre la mécanique manuelle reste crucial.
- Tester des variantes écrivant `system` dans d’autres entrées GOT (`exit`, `printf`, etc.).

## Commandes & références

- `checksec --file ./bin`
- `gdb -q ./bin`, `b *main+0x8e`, `x/gx &puts@GOT`

## Artefacts

- [`exploit.py`](./exploit.py)
- [`solution.py`](./solution.py)
- Table d’offsets générée par `fmtstr_payload`
