---
title: "4-printf-03-write"
challenge: "Neal"
difficulty: "4"
platform: "amd64/remote"
date: "2025-11-02"
tags: [binary]
author: "Ryan Bouchou"
status: "achivied"
---

# Titre lisible

**Résumé (1-2 lignes)**
Résumé court...

---

## Contexte

- Source : cours
- Environnement testé : Arch Linux, amd64, glibc 2.35
- Fichiers fournis : main.c,

---

## Objectif

Récupérer la flag / obtenir un shell

---

## Outils

- gdb + gef
- pwntools (python3)

---

## Analyse

### 1) Reconnaissance statique

- commandes & observations

### 2) Analyse dynamique

- breakpoints, comportement runtime

### 3) Exploit

On exploite l'usage non contrôlé de `printf` en utilisant le spécificateur de format, et à fortiori d'écriture, `%n` en écrivant à la position voulue.

---

## Résultat

- Flag : CTF{c319425a582d4cd481ac5c5c933ce9b1}

## Root cause

Explication courte du bug

## Mitigation

- corrections proposées

## Leçons apprises / next steps

- pistes d'amélioration

## Commandes & références

## Artefacts

- exploit.py
