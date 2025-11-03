---
title: "Heap data overflow"
challenge: "Neal — Heap and BSS overflow 1"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-05-12"
tags: [heap, overflow, fread]
author: "Ryan Bouchou"
status: "solved"
---

# Dépassement de données sur le tas

**Résumé (1-2 lignes)**  
Une lecture `fread` mal paramétrée autorise l'écriture de 1024 octets dans un buffer global de 32 octets, ce qui permet de forcer `is_admin` à `1` et d'obtenir un shell.

---

## Contexte

- Source : polycopié « Heap and BSS overflow 1 » (Neal)
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Déclencher `system("sh")` en activant le flag administrateur sans connaître le mot de passe.

---

## Outils

- `gdb` + `gef` pour inspecter la mémoire globale
- `pwntools` pour générer le bourrage et piloter la connexion distante
- `hexdump`/`xxd` afin de vérifier le contenu du payload

---

## Analyse

### 1) Reconnaissance statique

- `buf` est un buffer global de 32 octets initialisé à `"admin"` et réutilisé par `main`. 【F:heap/heap-data-overflow/main.c†L7-L22】
- `has_access()` conserve un `static int is_admin`, donc la variable réside également dans la section `.bss` à proximité immédiate de `buf`.
- L’appel litigieux `fread(buf, sizeof(buf), BUF_LEN, stdin);` lit `32 × 32 = 1024` octets dans le buffer de 32 octets et déborde sur `is_admin` (et au-delà).

### 2) Analyse dynamique

- En posant un break sur `get_username()` et en inspectant `&is_admin` avec `p &is_admin`, on confirme que la variable suit le buffer dans la `.bss`.
- L’exécution avec un payload de 1024 octets rempli de `0x01` montre que `is_admin` passe à `1` et que le programme atteint le bloc `system("sh")`.

### 3) Exploit

> Stratégie : overflow d’une donnée statique via `fread`

- Le payload doit commencer par `"admin\0"` afin de satisfaire `strcmp(buf, "admin")` après la lecture.
- Le reste du buffer est rempli par `0x00000001` répété pour écraser `is_admin` avec `1`.

```python
payload = flat(
    {
        0: b"admin\0",
    },
    length=1024,
    filler=p32(1),
)
```

- Une fois envoyé, le programme spawn un shell interactif. 【F:heap/heap-data-overflow/solution.py†L8-L28】

---

## Résultat

- Accès shell obtenu sur la cible locale/distante.

## Root cause

Usage incorrect de `fread` : la taille d’élément (`sizeof(buf)`) est confondue avec la taille du buffer, provoquant un débordement massif dans la section `.bss`.

## Mitigation

- Remplacer l’appel par `fread(buf, 1, BUF_LEN, stdin);` ou `fgets`, et valider la longueur de l’entrée.
- Éviter de stocker des flags de sécurité adjacents à des buffers manipulés par l’utilisateur.

## Leçons apprises / next steps

- Toujours vérifier la signature de `fread`/`fwrite` : `fread(ptr, size, nmemb, stream)`.
- Garder les variables de contrôle (droits, compteurs) séparées des buffers exposés.

## Commandes & références

- `gdb -q ./bin` puis `b get_username`, `p &is_admin`
- [Neal – Heap and BSS overflow 1 (support PDF)](https://repository.root-me.org/Exploitation%20-%20Syst%C3%A8me/Unix/EN%20-%20Heap%20and%20BSS%20overflow%201.pdf)

## Artefacts

- [`exploit.py`](./exploit.py)
- [`solution.py`](./solution.py)
- Capture GDB des adresses de la `.bss`
