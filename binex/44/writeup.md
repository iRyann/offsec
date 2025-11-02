---
title: "4-printf-04-write2"
challenge: "Neal"
difficulty: "4"
platform: "amd64/remote"
date: "2025-11-02"
tags: [binary]
author: "Ryan Bouchou"
status: "in-progress"
---

# Titre lisible

**Résumé (1-2 lignes)**
Résumé court...

---

## Contexte

- Source : cours
- Environnement testé : Arch Linux, amd64, glibc 2.35
- Fichiers fournis : vuln, main.c

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

L'idée est d'écrire `"sh"` à la place de l'adresse de `safe_command` comme valeur de la variable `ls`.

### 2) Analyse dynamique

On remarque que `rsp` pointe sur `ls`.

### 3) Exploit

> [!TIP] Stratégie : format-string
> On sait que `%<nombre>c` permet d'écrie `nombre` + 1 caractères, et `%N$p` permet d'écire le nombre d'octets écrit par `printf` à l'emplacement du $X$ ème argument de cette dernière.

En particulier, on sait que les codes ASCII de _s_ et _h_ correspondent respectivement à 73 et 68. Ainsi, comme la représenation hexadécimale en little endian `0x68 0x73` correspond à l'entier 26739 en décimal, je pense au payload suivant :

```python
payload = b"%26738c%6$n"
```
L'injection de ce dernier retourne : `sh: rh: command not found`.
On remarque :
    1. On obtient un shell en dépit de la commande `ls` et non pas écriture de `sh` à l'adresse voulue, manifestement...
    2. Le spécificateur de format `%<nombre>c` à écrit `nombre` octets, en dépit des `nombre` + 1 attendus.

## Payload (extrait) :
Par conséquent, le payload devient : `b"%26739c%6$n"` 
## Résultat

- Flag : CTF{8592622519e646e69cf7189c48accc8e}

## Root cause


## Mitigation

- corrections proposées

## Leçons apprises / next steps


## Commandes & références


## Artefacts

- exploit.py
