---
title: "ret2main"
challenge: "Formation interne"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-02-12"
tags: [stack-overflow, ret2main]
author: "Ryan Bouchou"
status: "in-progress"
---

# Revenir dans `show_alphabet`

**Résumé (1-2 lignes)**  
L’idée est de faire fuiter une adresse intéressante, puis de relancer `show_alphabet()` pour réutiliser l’information lors d’une seconde exécution du code vulnérable.

---

## Contexte

- Source : exercice « ret2main »
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Réorienter `saved RIP` vers `show_alphabet()` (ou `main`) après avoir obtenu une fuite, afin d’exécuter une seconde phase avec les adresses connues.

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

- `show_alphabet()` affiche un message puis quitte le programme. 【F:stack-overflows/stack-overflows-ret2main/main.c†L17-L34】
- `getstr` lit sans borne, rendant possible un overflow sur la frame de `show_alphabet`.
- Aucun canary, NX désactivé.

### 2) Analyse dynamique

- `gdb` : l’offset entre `buffer` (lettre) et `saved RIP` est de 72 octets (`0x48`).
- Première étape : envoyer un motif pour confirmer l’offset ; deuxième étape envisagée : injecter l’adresse de `main` ou `show_alphabet` (`objdump -d ./bin | grep show_alphabet`).

### 3) Exploit

> Plan d’action

1. Fuite : utiliser `printf("Voici l'alphabet...")` pour récupérer `alphabet` ou des adresses de la section `.rodata`.
2. Rejouer : écraser `saved RIP` avec `main` (`0x4011c0` par exemple) pour relancer le flux et exploiter l’information fuitée (ROP, shellcode…).
3. À finaliser : rédaction d’un script `pwntools` qui automatise les deux tours.

---

## Résultat

- En cours : la fuite fonctionne, la seconde phase reste à script-er.

## Root cause

Lecture illimitée (`getstr`) et absence de PIE/canary.

## Mitigation

- Remplacer `getstr` par `fgets`.
- Ajouter un compteur d’itérations ou terminer le programme proprement sans boucle vulnérable.

## Leçons apprises / next steps

- Documenter précisément l’adresse cible (`main`, `show_alphabet`) et vérifier l’alignement.
- Enchaîner avec une ROP chain pour aller plus loin qu’un simple retour.

## Commandes & références

- `objdump -d ./bin | grep show_alphabet`
- `gdb -q ./bin`, `b show_alphabet`, `x/20gx $rsp`

## Artefacts

- [`exploit.py`](./exploit.py) (template à compléter)
