---
title: "Titre du challenge"
challenge: "Neal — Nom du challenge"
difficulty: "Easy/Medium/Hard"
platform: "amd64/local"
date: "YYYY-MM-DD"
tags: [binary]
author: "Votre Nom"
status: "in-progress"
---

# Titre lisible

**Résumé (1-2 lignes)**  
Brève description du scénario et de l'approche retenue.

---

## Contexte

- Source : Neal (module, polycopié, série)
- Environnement testé : Arch Linux (amd64), version de glibc
- Fichiers fournis : `main.c`, binaire compilé correspondant

---

## Objectif

Que faut-il obtenir (flag, shell, mot de passe, etc.).

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

Points marquants vus dans le code source, l'ASM, les protections activées…

### 2) Analyse dynamique

Breakpoint, inspection mémoire, observations runtime.

### 3) Exploit

Stratégie retenue et payload final (ou extrait significatif).

---

## Résultat

- Flag / shell obtenu…

## Root cause

Description synthétique du bug exploité.

## Mitigation

Mesures de correction / bonnes pratiques.

## Leçons apprises / next steps

Idées pour approfondir ou améliorer l'exploit.

## Commandes & références

- Commandes utiles, documentation, articles.

## Artefacts

- Scripts ou ressources complémentaires (exploit.py, solution.py, captures, etc.)
