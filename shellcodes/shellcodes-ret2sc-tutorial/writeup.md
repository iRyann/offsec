---
title: "ret2shellcode tutoriel"
challenge: "Formation interne"
difficulty: "Easy"
platform: "amd64/local"
date: "2024-02-10"
tags: [shellcode, tutorial, stack]
author: "Ryan Bouchou"
status: "in-progress"
---

# Comprendre ret2shellcode pas à pas

**Résumé (1-2 lignes)**  
Ce niveau introduit les notions de frames, de passage d’arguments et de dépassement de tampon afin de préparer l’écriture d’un shellcode puis de détourner `saved RIP` vers celui-ci.

---

## Contexte

- Source : tutoriel interne ret2shellcode
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Déterminer l’offset entre `buffer` et `saved RIP`, injecter un shellcode et renvoyer l’exécution dessus.

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

- `tuto()` alloue un buffer de 96 octets rempli d’un motif, puis affiche en détail le contenu de la pile et des registres. 【F:shellcodes/shellcodes-ret2sc-tutorial/main.c†L12-L82】
- `gets(buffer);` lit sans borne et permet d’écraser `saved RIP` après les 96 octets + padding + `saved RBP`.
- La fonction `caller()` puis `main()` fournissent des frames supplémentaires illustrant la pile chaînée.

### 2) Analyse dynamique

- `gdb -q ./bin`, `b gets`, `run` permet d’arrêter avant la lecture et de noter `&buffer`.
- Après avoir saisi un motif généré par `pattern create 200`, `pattern search` donne l’offset précis jusqu’à `saved RIP`.
- `x/20gx $rsp` confirme l’agencement affiché dans les messages pédagogiques.

### 3) Exploit

> Stratégie envisagée (à finaliser)

- Générer un shellcode 64 bits via `shellcraft.sh()` ou l’assembler depuis [`sc.nasm`](../shellcodes-ret2sc-canary/sc.nasm) si besoin.
- Construire un payload : `shellcode + padding + adresse_buffer` (écrite en little endian) pour rediriger l’exécution.
- Ajuster les registres si nécessaire (alignement 16 octets) avant le `ret`.

---

## Résultat

- Travail en cours : la chaîne d’exploitation reste à automatiser.

## Root cause

Absence de contrôle de taille dans `gets` et pile exécutable (`NX` désactivé), exposant directement l’adresse de retour.

## Mitigation

- Remplacer `gets` par `fgets`.
- Activer la protection `-fstack-protector` et NX.

## Leçons apprises / next steps

- Valider l’offset obtenu en construisant un prototype de payload (pattern + adresse fictive).
- Rédiger un script `pwntools` générant automatiquement le shellcode et injectant le payload.

## Commandes & références

- `pattern create 200`, `pattern search 0x41414141`
- Notes de cours : System V AMD64 ABI

## Artefacts

- À produire : script d’exploitation et shellcode documenté.
