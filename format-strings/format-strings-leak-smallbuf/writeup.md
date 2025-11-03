---
title: "Format string leak (small buffer)"
challenge: "Neal — printf leak small buffer"
difficulty: "Medium"
platform: "amd64/local"
date: "2024-04-27"
tags: [format-string, canary, leak]
author: "Ryan Bouchou"
status: "solved"
---

# Double fuite : secret et canary

**Résumé (1-2 lignes)**  
Le buffer réduit empêche de spammer les `%p`, mais la notation `%N$p` permet quand même de lire `secret` et le canary avant de les renvoyer pour déclencher `system("sh")`.

---

## Contexte

- Source : suite d’exercices format string (Neal)
- Environnement testé : Arch Linux (amd64)
- Fichiers fournis : [`main.c`](./main.c), binaire compilé correspondant

---

## Objectif

Récupérer la valeur du `secret` puis celle du stack canary pour franchir les deux vérifications successives.

---

## Outils

- `gdb`/`gef` pour repérer l’offset du canary
- `pwntools` pour orchestrer les fuites et les conversions
- `fmtstr_payload` / formatage manuel pour `%N$p`

---

## Analyse

### 1) Reconnaissance statique

- Le username est lu avec `fgets(buf, 12, stdin)` : seulement 11 caractères utiles plus `\0`. 【F:format-strings/format-strings-leak-smallbuf/main.c†L35-L52】
- `printf(buf);` affiche directement la chaîne contrôlée.
- Après validation du `secret`, le programme demande explicitement la valeur du canary stocké dans `CANARY` (alias `*(long*)(buf+0xc)`).

### 2) Analyse dynamique

- Via `gdb`, on observe que `secret` est stocké à `RSP+0x10`, soit le 8ᵉ argument pour `printf`, et que le canary est à `RSP+0x28`, soit le 11ᵉ argument.
- L’utilisation de la notation `%11$p %8$p` malgré la taille limitée du buffer permet de cibler directement les arguments voulus sans dépasser 11 caractères (hors espaces).

### 3) Exploit

> Stratégie : fuite ciblée + réinjection des valeurs signées

- On envoie `b"%11$p %8$p"` pour récupérer d’un coup le canary puis `secret`.
- On convertit chaque fuite en entier signé avant de les transmettre aux deux `scanf` successifs.

```python
p.sendlineafter(b"username: ", b"%11$p %8$p")
_, canary, secret = p.recvline().strip().split()
canary = signed(int(canary, 16))
secret = signed(int(secret, 16))
```

- Les deux valeurs correctes débloquent finalement le shell. 【F:format-strings/format-strings-leak-smallbuf/solution.py†L9-L23】

---

## Résultat

- Shell interactif accessible ; flag exfiltré.

## Root cause

Absence de chaîne de format fixe pour `printf`, combinée à la possibilité de sélectionner les arguments (`%N$p`), permettant de contourner la limite du buffer.

## Mitigation

- Ne jamais passer d’entrée utilisateur comme format (`printf("%s", buf);`).
- Désactiver la prise en charge de `%n`/`%p` via un wrapper ou valider que la chaîne ne contient pas `%`.

## Leçons apprises / next steps

- Même avec un buffer minuscule, `%N$p` permet des fuites précises.
- Documenter systématiquement l’offset des objets sensibles (canary) dans les exercices suivants.

## Commandes & références

- `gdb -q ./bin`, `b *main+0x67`, `info frame`
- Documentation `printf(3)` sur la notation positionnelle

## Artefacts

- [`exploit.py`](./exploit.py)
- [`solution.py`](./solution.py)
- Tableau des offsets (notes personnelles)
