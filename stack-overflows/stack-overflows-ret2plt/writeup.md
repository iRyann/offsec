---
title: "ret2plt"
challenge: "Formation interne"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-02-20"
tags: [stack-overflow, rop]
author: "Ryan Bouchou"
status: "solved"
---

# Rebondir via la PLT

**Résumé (1-2 lignes)**  
Plutôt que d’écrire un shellcode, on redirige l’adresse de retour sur `login()` via un gadget `ret` pour réaligner la pile avant l’appel.

---

## Contexte

- Source : exercice « ret2plt »
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Exploiter `gets` pour déclencher `login()` malgré NX et ASLR partiel.

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

- `login()` appelle `system("/bin/sh");`. 【F:stack-overflows/stack-overflows-ret2plt/main.c†L24-L34】
- `gets(buffer)` sur 64 octets de pile sans canary : overflow possible.
- NX est actif, d’où la nécessité de réutiliser du code existant plutôt qu’un shellcode.

### 2) Analyse dynamique

- `pattern create 200` → `pattern search` : offset de 72 octets entre `buffer` et `saved RIP`.
- `ROP(exe).ret.address` fournit un gadget `ret` nécessaire pour réaligner la pile avant d’appeler `login` (sinon crash dû à `movdqa`).
- `exe.sym["login"]` donne l’adresse de la fonction cible.

### 3) Exploit

> Stratégie : `padding + ret + login`

```python
payload = flat({
    0x6161617461616173: (
        rop.ret.address,
        exe.sym["login"],
    ),
})
```

- On écrit d’abord l’adresse du gadget `ret`, puis celle de `login`. L’appel final à `printf` déclenche `login()` et ouvre un shell. 【F:stack-overflows/stack-overflows-ret2plt/solution.py†L9-L31】

---

## Résultat

- Shell `/bin/sh` accessible.

## Root cause

Absence de canary et lecture non bornée malgré NX.

## Mitigation

- Remplacer `gets` par `fgets`.
- Activer PIE et FULL RELRO pour rendre les adresses moins prévisibles et la GOT immuable.

## Leçons apprises / next steps

- Penser à l’alignement de la pile avec des gadgets `ret` lorsque des instructions SSE apparaissent (`movdqa`).
- Étendre l’exploit pour appeler d’autres fonctions (ROP plus long : `system("/bin/sh")`, `setuid(0)`, etc.).

## Commandes & références

- `ROPgadget --binary ./bin | grep "ret"`
- `gdb -q ./bin`, `pattern create 200`, `pattern search`

## Artefacts

- [`solution.py`](./solution.py)
- Notes d’alignement de la pile
