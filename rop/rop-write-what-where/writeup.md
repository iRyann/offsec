---
title: "Titre du challenge"
challenge: "root-me/picoCTF"
difficulty: "Easy"
platform: "amd64/remote"
date: "YYYY-MM-DD"
tags: [binary, overflow]
author: "Ryan Bouchou"
status: "in-progress"
---

## Analyse

### 1) Reconnaissance statique

- Contraite : payload en 160 octets
- Avantage : présence de `system` dans le programme
- Inconvénient : absence de la chaîne `sh\0` -> on doit la former

### 2) Analyse dynamique

### 3) Exploit

Stratégie : ROP

1. On récupère le `sh` à partir de la chaîne `ls -lah`.
   - `addr_s = 0x0000000000402017`
   - `addr_h = 0x000000000040201c`
2. On détermine un espace inscriptible : `addr_sh =0x2ce6011` -- In '[heap]'(0x2ce6000-0x2d08000), permission=r-
   - Si on est malin, on saisit uniquement "s", ce qui
     nous donne la chaine "s\n\0" -> un seul octet à réécrire situé en `0x2ce6012`.
     Pour ce faire, il faut remplacer -- ou plutôt transformer `0x0a` en `0x69`.
   - En remarquant la présence du gadget suivant :
   ```asm
   0x000000000040113b : add byte ptr [rcx], al ; pop rbp ; ret
   ```
   instancier `al` à `0x5e = 94` nous permet de compléter
   l'octet en question pour obtenir `h`.
   - On trouve un gadget permettant d'affecter `94` à `al` :
   ```asm
   0x00000000004011a3 : pop rax ; pop rbx ; pop rcx ; ret
   ```
3. On exploite un gadget permettant d'écrire à l'adresse susmentionnée : `g_rite_byte = `
4. On appelle system:
   - `g_system = 0x40119d`
   ```asm
   g_rdi = 0x0000000000401253 # pop rdi ; ret
   ```

---

## Résultat

- Flag : CTF{...}

## Root cause

Explication courte du bug

## Mitigation

- corrections proposées

## Leçons apprises / next steps

- pistes d'amélioration

## Commandes & références

- readelf -a binary

## Artefacts

- exploit.py, build/
