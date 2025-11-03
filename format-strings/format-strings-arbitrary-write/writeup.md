---
title: "Format string arbitrary write"
challenge: "Neal — printf %n"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-05-05"
tags: [format-string, arbitrary-write]
author: "Ryan Bouchou"
status: "solved"
---

# Écriture arbitraire avec %n

**Résumé (1-2 lignes)**  
En contrôlant la chaîne de format, on exploite `%n` pour écrire la valeur 42 dans `userid` et déclencher `system("sh")` sans connaître le mot de passe.

---

## Contexte

- Source : module sur `%n` dans les format strings (Neal)
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Écrire la valeur `42` dans la variable globale `userid` à l’aide d’un format string contrôlé.

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

- `userid` est global et initialisé à `0`. 【F:format-strings/format-strings-arbitrary-write/main.c†L24-L44】
- `printf(buf);` est invoqué après un `scanf("%64s", buf);`, ce qui autorise n’importe quel spécificateur de format.
- Le 7ᵉ argument de `printf` correspond à `&userid` (voir initialisation `int *intptr = &userid;`).

### 2) Analyse dynamique

- Dans `gdb`, `b *main+0x9d` puis `x/8gx $rsp` montre que l’adresse de `userid` est poussée comme 7ᵉ argument (compter les 6 registres ABI puis la pile).
- Un padding de 42 caractères imprimés avant `%7$n` suffit pour écrire `42` (`0x2a`) dans `userid`.

### 3) Exploit

> Stratégie : construire exactement 42 octets écrits avant `%7$n`

- Le payload `b"a" * 42 + b"%7$n"` écrit `42` (longueur des `a`) à l’adresse pointée par le 7ᵉ argument, soit `userid`.

```python
userid_overrider = b"a" * 42
payload = userid_overrider + b"%7$n"
io.sendlineafter(b"payload: ", payload)
```

- L’écriture aboutit et `system("sh")` est invoqué. 【F:format-strings/format-strings-arbitrary-write/exploit.py†L38-L42】

---

## Résultat

- Shell interactif obtenu, flag : `CTF{c319425a582d4cd481ac5c5c933ce9b1}`.

## Root cause

Utilisation directe de `printf` sur une chaîne contrôlée et présence du pointeur `&userid` dans les arguments, permettant l’usage de `%n` pour une écriture arbitraire.

## Mitigation

- Remplacer `printf(buf);` par `printf("%s", buf);`.
- Éviter de passer des pointeurs sensibles dans la liste d’arguments lorsqu’une chaîne contrôlée est utilisée.

## Leçons apprises / next steps

- `%n` est puissant même sans `%hn/%hhn` : il suffit de choisir judicieusement la taille du padding.
- Prévoir une version étendue exploitant `%hn` pour écrire des valeurs 16 bits multiples.

## Commandes & références

- `gdb -q ./bin`, `b *main+0xa7`, `info registers` pour vérifier l’ordre des arguments
- [bases-hacking.org — Écrire en mémoire avec %n](https://bases-hacking.org/ecrire-memoire.html)

## Artefacts

- [`exploit.py`](./exploit.py)
- Notes de calcul du padding
