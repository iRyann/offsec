---
title: "ret2shellcode avec canary"
challenge: "Formation interne"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-03-22"
tags: [shellcode, canary, stack]
author: "Ryan Bouchou"
status: "solved"
---

# Bypasser un canary statique

**Résumé (1-2 lignes)**  
Le programme protège `saved RIP` avec un canary constant `0xCAFE...`; en le recopiant avant d’écraser l’adresse de retour, on exécute un shellcode placé dans le buffer.

---

## Contexte

- Source : atelier interne ret2shellcode + canary
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Contourner un canary connu et déclencher un shell en exécutant un shellcode depuis la pile.

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

- `custom_canary` vaut `0xcafeCAFEcafeCAFE` et est vérifié juste après la lecture du buffer avec `gets`. 【F:shellcodes/shellcodes-ret2sc-canary/main.c†L7-L18】
- Le buffer (15 octets) est trop petit pour contenir à la fois le canary et le shellcode ; l’overflow se poursuit donc au-delà de `buffer`.
- NX est désactivé, la pile reste exécutable.

### 2) Analyse dynamique

- `printf("DEBUG: buffer is at %p\n\n", buffer);` divulgue l’adresse du buffer.
- Avec `gdb`, on mesure : `canary_offset = 15` (distance buffer → canary) et `ret_offset = 39` (buffer → adresse de retour).
- Le shellcode est injecté juste après l’adresse de retour (offset `ret_offset + 8`).

### 3) Exploit

> Stratégie : réinscrire le canary constant puis détourner `saved RIP`

- On reprogramme le canary avec sa valeur originale avant d’écraser l’adresse de retour.
- L’adresse de retour est remplacée par `buf_addr + ret_offset + 8`, soit l’emplacement du shellcode.

```python
payload = flat({
    canary_offset: 0xCAFECAFECAFECAFE,
    ret_offset: shellcode_addr,
    ret_offset + 8: asm(shellcraft.sh()),
})
```

- Envoi du payload → `gets` recopie le canary valide, l’exécution revient sur le shellcode et un shell s’ouvre. 【F:shellcodes/shellcodes-ret2sc-canary/solution.py†L9-L33】

---

## Résultat

- Shell interactif opérationnel.

## Root cause

Lecture non bornée (`gets`) + canary constant divulgué indirectement (car connu dans le binaire) et absence de NX.

## Mitigation

- Remplacer `gets` par `fgets` et régénérer un canary aléatoire à chaque exécution (`__stack_chk_fail`).
- Activer NX pour empêcher l’exécution de la pile.

## Leçons apprises / next steps

- Même un canary présent n’arrête pas l’overflow s’il est constant.
- Explorer une version avec ASLR et canary aléatoire nécessitant un leak préalable.

## Commandes & références

- `gdb -q ./bin`, `p/x custom_canary`, `distance buffer saved_rip`
- Notes `sc.nasm` pour un shellcode custom

## Artefacts

- [`exploit.py`](./exploit.py)
- [`solution.py`](./solution.py)
- [`sc.nasm`](./sc.nasm)
