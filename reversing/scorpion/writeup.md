---
title: "Scorpio"
analyst: "Ryan Bouchou"
date: 2025-12-10
target: "./scorpion"
hashes:
  sha256: 14fb7bd84aa6e5a01f8b87f908ff799b035da0135a71a6f8991ddcf34b180277
---

# Reverse Engineering Report

## 1. Contexte & objectifs

### 1.1 Cible

- Binaire: ./scorpion
- Arch: amd64
- Protections:

### 1.2 Objectif

Comprendre mécanisme X

### 1.3 Limites

Aucune.

---

## 2. Environnement

- OS: Arch Linux
- Tools: Ghidra, gdb+pwndbg, rizin, pwntools

---

## 3. Reconnaissance

### 3.1 Propriétés générales

```bash
file ./binary
checksec --file=./binary
strings ./binary | head -20
```

### 3.2 Imports/Exports

crypto, socket, ptrace...

### 3.3 Chaînes intéressantes

URLs, messages d'erreur...

---

## 4. Analyse statique

### 4.1 Entry point

main @ 0x004000b0

### 4.2 Fonctions critiques

check_password, decrypt_data...

---

## 5. Analyse dynamique

### 5.1 Tests

Scénarios + résultats

### 5.2 Validation hypothèses

HYP → TEST → RES

---

## 6. Protections & anti-RE

Anti-debug, packing...

---

## 7. Modèle reconstruit

Flow d'exécution, structures...

---

## 8. Points sensibles

Surfaces d'entrée, parsing...

---

## 9. Journal chronologique

Timeline détaillée

---

## 10. Synthèse

### Compris

- Point 1

### Partiellement compris

- Point 2

### Non résolu

- Point 3

### Recommandations

- Suggestions

---

## Annexes

### Scripts utilisés

`````python
# scripts d'analyse
````w
`````
