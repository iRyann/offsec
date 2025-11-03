---
title: "Burger shellcode"
challenge: "Prépa 8 — Burger"
difficulty: "Hard"
platform: "amd64/local"
date: "2024-02-28"
tags: [shellcode, canary, control-flow]
author: "Ryan Bouchou"
status: "solved"
---

# Recomposer un shellcode en deux morceaux

**Résumé (1-2 lignes)**  
Le buffer est morcelé par un canary : on divise le shellcode en deux parties contiguës, on réécrit le canary puis on retourne sur la seconde moitié afin d’exécuter `/bin/sh`.

---

## Contexte

- Source : parcours « Prépa exploitation » (niveau Burger)
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Injecter un shellcode fonctionnel alors qu’un canary statique `0xdeadbeefdeadbeef` coupe le buffer en deux segments.

---

## Outils

- `gdb` pour mesurer les offsets (buffer → canary → `saved RIP`)
- `pwntools` pour assembler les deux fragments et automatiser le calcul du saut relatif
- Scripts NASM (`sc1.nasm`, `sh.nasm`) pour tester plusieurs variantes

---

## Analyse

### 1) Reconnaissance statique

- `buffer[15]` est rempli via `getstr`; après la lecture, deux vérifications : `isadmin == 256` (jamais atteignable) puis comparaison du canary. 【F:shellcodes/shellcodes-burger/main.c†L7-L21】
- Le `printf` initial fournit l’adresse exacte du buffer.
- Comme dans la version « canary », l’adresse de retour se situe `39` octets après le début du buffer.

### 2) Analyse dynamique

- `gdb` confirme que seuls 15 octets sont disponibles avant le canary, insuffisant pour un shellcode complet.
- L’espace entre la fin du canary et `saved RIP` est de 16 octets contigus, assez pour une seconde portion de shellcode.
- On peut donc : écrire la première moitié (setup registres) avant le canary, recopier la valeur du canary, puis placer la seconde moitié juste après.

### 3) Exploit

> Stratégie : deux morceaux + saut relatif court

- `piece1` (15 B) prépare les registres (`xor rsi, rsi`, `push rsi`, etc.).
- `piece2` (16 B) charge `"//bin/sh"` avec `movabs`, puis effectue un `jmp rel8` arrière pour retomber sur la première moitié.
- Le payload final :

```python
payload  = piece1
payload += b"\x90" * (canary_offset - len(piece1))
payload += p64(0xDEADBEEFDEADBEEF)
payload += piece2
payload  = payload.ljust(saved_rip_offset, b"\x90")
payload += p64(piece2_addr)
```

- `piece2_addr` pointe sur le début de la seconde moitié ; le saut relatif ramène l’exécution sur `piece1`, et le shell s’ouvre. 【F:shellcodes/shellcodes-burger/exploit.py†L19-L68】

---

## Résultat

- Flag : `{02da1bb071fc47d6525fbcc218212329}` obtenu via le shell.

## Root cause

Lecture non bornée et pile exécutable : malgré la présence d’un canary, l’absence de randomisation et de NX permet de composer un shellcode multi-segments.

## Mitigation

- Utiliser un canary aléatoire régénéré à chaque exécution.
- Activer NX pour bloquer l’exécution sur la pile, supprimer le `printf` de debug.

## Leçons apprises / next steps

- Segmenter un shellcode est une option viable lorsque le buffer est fragmenté.
- Explorer des shellcodes auto-modifiants pour réduire la taille du premier segment.

## Commandes & références

- `gdb -q ./8-prepa-01-burger`, `pattern create`, `distance` pour les offsets
- Shellcodes x86-64 : [shell-storm](http://shell-storm.org/shellcode/)

## Artefacts

- [`exploit.py`](./exploit.py)
- [`sc1.nasm`](./sc1.nasm)
- [`sh.nasm`](./sh.nasm)
- [`temp.py`](./temp.py) pour les tests rapides
