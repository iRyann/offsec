---
title: "Titre du challenge"
challenge: "root-me/picoCTF"
difficulty: "Easy"
platform: "amd64/remote"
date: "YYYY-MM-DD"
tags: [binary,overflow]
author: "Ryan Bouchou"
status: "in-progress"
---

# Titre lisible

**Résumé (1-2 lignes)**
Résumé court...

---

## Contexte

- Source : root-me / cours / picoCTF
- Environnement testé : Ubuntu 22.04, amd64, glibc 2.35
- Fichiers fournis : vuln, main.c, libc.so

---

## Objectif

Récupérer la flag / obtenir un shell

---

## Outils

- gdb + gef / pwndbg
- pwntools (python3)
- readelf / objdump / strings / file

---

## Analyse

### 1) Reconnaissance statique

En observant le code c, on remarque deux primitives :

- `gets(user)` -> écriture illimitée dans `.data` à partir de `user (0x404060)`.
- `fgets(pass, BUF_SZ, ...)` -> overflow de 8 → écrase saved RBP puis saved RIP de `login`. 

La section `.data`, inscriptible, pour y déployer notre de ROP chains :
```asm
gef➤  vmmap 
0x0000000000404000 0x0000000000405000 0x0000000000003000 rw- /home/ryan/dev/lab/offsec/rop/rop-stack-pivot/6-rop-04-stack-pivot
```


### 2) Analyse dynamique

Le buffer `pass` de la fonction `login()` se situe à 8 octets de `rbp` :

```asm
0x4011b2 488d45f8            <login+003c>     lea    rax, [rbp-0x8]
0x4011b6 be20000000          <login+0040>     mov    esi, 0x20
0x4011bb 4889c7              <login+0045>     mov    rdi, rax
0x4011be e89dfeffff          <login+0048>     call   0x401060 <fgets@plt>
```

> _Remarque_ : On s'en doutait vu qu'on ne réserve la place que pour 8 octets et 
qu'il s'agit de la seule et unique variable déclarée
au sein de la fonction `login()` (outre l'argument `char* user` qui est passé par copie).

### 3) Exploit

Stratégie : ROP Stack pivot 



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
