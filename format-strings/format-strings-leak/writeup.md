---
title: "Format string leak"
challenge: "Neal — printf leak"
difficulty: "Easy"
platform: "amd64/local"
date: "2024-04-20"
tags: [format-string, leak]
author: "Ryan Bouchou"
status: "solved"
---

# Lecture d'un secret via format string

**Résumé (1-2 lignes)**  
L'entrée utilisateur est passée directement comme chaîne de format à `printf`, ce qui permet de dévoiler la valeur aléatoire `secret` sur la pile avec quelques `%p`.

---

## Contexte

- Source : cours d'introduction aux format strings (Neal)
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Divulguer `secret` pour passer la vérification et obtenir un shell via `system("sh")`.

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

- `secret` est un `long` stocké dans la frame de `main` avant la lecture de `username`. 【F:format-strings/format-strings-leak/main.c†L43-L53】
- `printf(buf);` est invoqué sans format sécurisé : chaque `%` injecté lira les registres puis la pile selon la System V ABI.
- Après la fuite, `secret` est comparé à l’entrée via `scanf("%ld", &input);` et un shell est lancé en cas d’égalité.

### 2) Analyse dynamique

- En `gdb`, les commandes `b printf` et `x/10gx $rsp` confirment que `secret` se trouve à `RSP+0x40` lors de l’appel à `printf`.
- Les premiers `%p` affichent les registres (`RSI`, `RDX`, ...), mais dès le 8ᵉ `%p`, la valeur de `secret` apparaît dans la sortie standard.

### 3) Exploit

> Stratégie : fuite par `%p` puis conversion signée

- On envoie sept `%p ` pour atteindre l’offset où `secret` est stocké.
- On parse le 8ᵉ mot et on convertit la valeur hexadécimale en entier signé avant de la renvoyer au programme.

```python
p.sendlineafter(b"username: ", b"%p " * 7)
secret = signed(int(p.recvline().split()[7], 16))
p.sendlineafter(b"secret: ", str(secret).encode())
```

- La session devient interactive et le shell est accessible. 【F:format-strings/format-strings-leak/solution.py†L9-L25】

---

## Résultat

- Flag récupéré via le shell (`CTF{...}`).

## Root cause

Passage direct d’une donnée contrôlée par l’utilisateur à `printf`, sans chaîne de format fixe, ouvrant la voie à des lectures arbitraires de la pile.

## Mitigation

- Utiliser `printf("%s", buf);` ou `puts`.
- Filtrer les caractères `%` dans l’entrée utilisateur ou recourir à `snprintf` avec format verrouillé.

## Leçons apprises / next steps

- Cartographier précisément la pile permet de cibler rapidement l’offset utile.
- Penser à convertir les fuites en entier signé pour éviter les erreurs de comparaison.

## Commandes & références

- `gdb -q ./bin`, `b *main+0x5c`, `x/16gx $rsp`
- [StackOverflow — rôle des registres FS/GS](https://stackoverflow.com/questions/10810203/what-is-the-fs-gs-register-intended-for)

## Artefacts

- [`exploit.py`](./exploit.py)
- [`solution.py`](./solution.py)
- Transcription de session GDB montrant la fuite
