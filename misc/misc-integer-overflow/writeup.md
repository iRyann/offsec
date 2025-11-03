---
title: "Integer overflow"
challenge: "CWE-190 exercice"
difficulty: "Easy"
platform: "amd64/local"
date: "2024-01-05"
tags: [integer-overflow]
author: "Ryan Bouchou"
status: "solved"
---

# Forcer un wrap d’entier

**Résumé (1-2 lignes)**  
Le programme additionne trois fois un entier `int`; en fournissant `1431655766`, la somme déborde et revient à `2`, ce qui déclenche `system("sh")`.

---

## Contexte

- Source : démonstration de la CWE-190 (overflow arithmétique)
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Trouver une valeur d’entrée qui, après `num = num + num + num;`, vaut exactement `2` dans un entier 32 bits signé.

---

## Outils

- Calculatrice modulaire (Python, `pwntools`) pour tester les overflows
- `gdb` pour vérifier la valeur stockée dans `eax`

---

## Analyse

### 1) Reconnaissance statique

- La multiplication est remplacée par trois additions sur un `int`. 【F:misc/misc-integer-overflow/main.c†L8-L14】
- Si `num` dépasse `INT_MAX`, la valeur wrappe modulo `2^32`.

### 2) Analyse dynamique

- Avec Python : `n = 0x0100000002 >> 2` ? Non, on choisit `1431655766`; `(1431655766 * 3) & 0xFFFFFFFF == 2`.
- `gdb` confirme que `eax` vaut `0x00000002` juste avant la comparaison.

### 3) Exploit

> Stratégie : utiliser `1431655766`

```python
p.sendlineafter(b"code secret: ", b"1431655766")
```

- La valeur wrappe et satisfait la condition `if (num == 2)` ; le shell est lancé. 【F:misc/misc-integer-overflow/solution.py†L9-L19】

---

## Résultat

- Shell obtenu immédiatement après l’entrée.

## Root cause

Utilisation d’un `int` 32 bits sans vérifier le dépassement lors des additions successives.

## Mitigation

- Vérifier les opérations (`if (num > INT_MAX / 3)`) ou utiliser un type 64 bits.
- Ajouter des contrôles de cohérence sur l’entrée utilisateur.

## Leçons apprises / next steps

- Documenter d’autres valeurs qui wrapent vers 2 (`(2 + k * 2^32) / 3`).
- Tester la version 64 bits (`long`) pour montrer la différence.

## Commandes & références

- Python : `[(x * 3) & 0xffffffff for x in range(...)]`
- [MITRE CWE-190](https://cwe.mitre.org/data/definitions/190.html)

## Artefacts

- [`solution.py`](./solution.py)
- Notes de calcul modulo `2^32`
