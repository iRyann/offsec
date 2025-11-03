---
title: "ret2shellcode sans octet nul"
challenge: "Formation interne"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-03-18"
tags: [shellcode, stack, bof]
author: "Ryan Bouchou"
status: "solved"
---

# Injecter un shellcode compact

**Résumé (1-2 lignes)**  
Le binaire affiche l’adresse du buffer puis compare `isadmin` à `256`; en injectant un shellcode sans octet nul et en réécrivant l’adresse de retour, on obtient un shell malgré l’espace restreint.

---

## Contexte

- Source : exercice maison sur le retour vers shellcode
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Exécuter un shellcode placé dans `buffer[15]` en contournant la contrainte d’absence de `\x00` dans l’entrée.

---

## Outils

- `gdb`/`pwndbg` pour mesurer l’offset jusqu’au `saved RIP`
- `pwntools` pour assembler le shellcode (`asm(shellcraft.sh())` adapté) et manipuler la fuite d’adresse
- `hexdump` pour valider l’absence de `\x00`

---

## Analyse

### 1) Reconnaissance statique

- `buffer` est un tableau de 15 octets, rempli via `getstr` qui s’arrête uniquement sur `\n`. 【F:shellcodes/shellcodes-ret2sc-no-null/main.c†L5-L18】
- `printf("DEBUG: buffer is at %p\n\n", buffer);` divulgue l’adresse exacte à sauter.
- `isadmin` doit rester `0` : aucune vérification supplémentaire après la lecture.

### 2) Analyse dynamique

- `gdb` révèle un offset de 24 octets entre le début du buffer et `saved RIP`.
- La pile est exécutable (NX désactivé), donc on peut rediriger l’exécution vers le buffer sans gadgets supplémentaires.

### 3) Exploit

> Stratégie : placer un shellcode minimal puis écraser l’adresse de retour

- Shellcode utilisé : nettoyage des registres, écriture de `"//bin/sh"` sur la pile, `syscall` 59. Deux `pop rax` sont ajoutés pour gagner de l’espace.
- Le payload contient le shellcode suivi d’un padding puis de l’adresse du buffer renversée (`little endian`).

```python
payload = flat({
    0: asm(shellcode),
    "ahaa": unhex(ptr_repr)[::-1],  # adresse du buffer fournie par le binaire
})
```

- Après envoi, la redirection vers le buffer déclenche le shell. 【F:shellcodes/shellcodes-ret2sc-no-null/solution.py†L9-L32】

---

## Résultat

- Shell interactif obtenu (`$ id` exécutable).

## Root cause

Buffer overflow classique : lecture sans limite (`getstr`) et exécution autorisée sur la pile.

## Mitigation

- Remplacer `getstr` par `fgets` avec vérification de taille.
- Activer NX/ASLR et éviter de divulguer des adresses (`printf` de debug).

## Leçons apprises / next steps

- Toujours vérifier la taille du shellcode par rapport au buffer disponible.
- Tester une variante avec un shellcode polymorphe qui règle `isadmin` avant d’appeler `system`.

## Commandes & références

- `gdb -q ./bin`, `pattern create 200`, `pattern search` pour l’offset
- Cheatsheet shellcodes 64 bits (skape, shell-storm)

## Artefacts

- [`exploit.py`](./exploit.py)
- [`solution.py`](./solution.py)
- Shellcode assemblé (`build/shellcode.bin`)
