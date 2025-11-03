---
title: "ret2func"
challenge: "Formation interne"
difficulty: "Easy"
platform: "amd64/local"
date: "2024-01-30"
tags: [stack-overflow, ret2func]
author: "Ryan Bouchou"
status: "solved"
---

# Rebondir sur `login()`

**Résumé (1-2 lignes)**  
Un dépassement de pile permet d’écraser l’adresse de retour de `main` et de la remplacer par `login()`, qui exécute `/bin/sh`.

---

## Contexte

- Source : atelier « ret2func »
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Écraser `saved RIP` avec l’adresse de `login()`.

---

## Outils

- `gdb`/`gef` pour mesurer l’offset et récupérer l’adresse de `login`
- `pwntools` pour fabriquer le payload et automatiser l’attaque
- `checksec` pour confirmer l’absence de canary/PIE

---

## Analyse

### 1) Reconnaissance statique

- `main` lit 64 octets dans `buffer` via `gets`, sans protection. 【F:stack-overflows/stack-overflows-ret2func/main.c†L34-L44】
- `login()` appelle directement `execve("/bin/sh", NULL, NULL);`.
- Le binaire n’a ni PIE ni canary, les adresses sont fixes.

### 2) Analyse dynamique

- `pattern create 200` puis `pattern search` indique un offset de 72 octets (`0x48`) entre le début du buffer et `saved RIP`.
- `info functions login` fournit l’adresse `login@plt`. Comme le binaire n’est pas PIE, la valeur est constante (`exe.sym["login"]`).

### 3) Exploit

> Stratégie : padding de 72 octets suivi de l’adresse de `login`

```python
payload = flat({
    "saaa": exe.sym["login"],
})
```

- `flat` remplit automatiquement le padding puis place l’adresse de `login`. L’envoi déclenche immédiatement le shell. 【F:stack-overflows/stack-overflows-ret2func/solution.py†L9-L28】

---

## Résultat

- Shell `/bin/sh` disponible.

## Root cause

Usage de `gets` sur un buffer de pile sans canary ni PIE.

## Mitigation

- Remplacer `gets` par `fgets` et activer `-fstack-protector` + PIE.
- Limiter les permissions du binaire (pas de `execve` accessible).

## Leçons apprises / next steps

- Vérifier la nécessité d’un `ret` de réalignement lorsque `login` exige un alignement 16 octets.
- Étendre l’exploit pour déclencher une `ROP chain` plus complexe.

## Commandes & références

- `gdb -q ./bin`, `pattern create 200`, `pattern search` pour l’offset
- `info functions` pour lister les symboles

## Artefacts

- [`exploit.py`](./exploit.py)
- [`solution.py`](./solution.py)
- Capture GDB des offsets
