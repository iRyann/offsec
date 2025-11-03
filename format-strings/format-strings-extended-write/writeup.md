---
title: "Format string extended write"
challenge: "Neal — printf padding"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-05-08"
tags: [format-string, arbitrary-write]
author: "Ryan Bouchou"
status: "solved"
---

# Remplacer `ls` par `sh` avec `%n`

**Résumé (1-2 lignes)**  
En combinant le padding de `printf` et `%n`, on remplace la chaîne `safe_command` par `sh` malgré un buffer d’entrée très court, ce qui force `system(ls)` à ouvrir un shell.

---

## Contexte

- Source : exercice `printf` avancé (Neal)
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Écraser la chaîne `safe_command[] = "ls -lah *";` pour exécuter `/bin/sh` lors de l’appel final à `system`.

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

- `safe_command` est un tableau global, et `ls` pointe dessus avant l’appel à `system(ls);`. 【F:format-strings/format-strings-extended-write/main.c†L13-L30】
- La chaîne saisie est lue avec `scanf("%14s", buf);` : pas d’espace possible, mais `%n` reste exploitable.
- Lors du `printf(buf);`, le 6ᵉ argument de `printf` correspond à `ls` (`safe_command`), ce qui autorise `%6$n`.

### 2) Analyse dynamique

- Dans `gdb`, `x/s safe_command` confirme que l’adresse reste constante pendant l’exécution et est accessible en écriture.
- `printf("%6$p")` révèle que le 6ᵉ argument pointe bien sur `safe_command`.

### 3) Exploit

> Stratégie : écrire la valeur décimale de `u16(b"sh")` à l’aide du padding

- `u16(b"sh")` donne `0x6873` soit `26739` en décimal.
- On construit `"%26739c%6$n"` : `printf` écrit 26739 caractères (un espace suivi de 26738 espaces) puis `%6$n` stocke cette valeur sur deux octets à l’adresse de `safe_command`.

```python
sh_int = u16(b"sh")
payload = f"%{sh_int}c%6$n".encode()
```

- Après l’envoi, `system(ls)` exécute bien `/bin/sh`. 【F:format-strings/format-strings-extended-write/solution.py†L9-L15】

---

## Résultat

- Flag obtenu : `CTF{8592622519e646e69cf7189c48accc8e}`.

## Root cause

Combinaison d’un format string contrôlé et d’un pointeur vers une chaîne modifiable passé comme argument à `printf`, permettant d’écrire des valeurs arbitraires avec `%n` malgré un buffer restreint.

## Mitigation

- Interdire `%n` via un filtre ou utiliser `printf("%s", buf);`.
- Ne pas conserver de pointeurs vers des commandes sensibles dans la liste d’arguments d’un `printf` non maîtrisé.

## Leçons apprises / next steps

- Le padding `%<N>c` est une alternative compacte lorsqu’un buffer limite la longueur du payload.
- Prévoir une version 64 bits (`%ln`) pour écraser l’intégralité d’un pointeur.

## Commandes & références

- `gdb -q ./bin`, `b *main+0x124`, `x/s safe_command`
- Documentation `printf` sur les conversions `%n` et la notation de champ

## Artefacts

- [`exploit.py`](./exploit.py)
- [`solution.py`](./solution.py)
- Notes de calcul sur les valeurs `u16`
