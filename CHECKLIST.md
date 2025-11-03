# Checklist - Nouveau CTF

## 1. Répertoire de travail
- [ ] Dossier dédié sous la catégorie adaptée (*e.g.* `stack-overflows/`, `heap/`, `format-strings/`, `shellcodes/`, `reversing/`, etc.).
- [ ] Structure :
  - `writeup.md` pour rendre compte de la démarche exploratoire et de l'exploitation.
  - `src(/)` pour le code source original (C, NASM, Python…).
  - `exploit(/)` pour les scripts `pwntools`, ou outils auxiliaires.
  - `artefacts(/)` (facultatif) pour les captures, diagrammes ASCII, outputs GDB.

## 2. Documentation
- [ ] Sections `Contexte`, `Objectif`, `Analyse`, `Résultat`, `Root cause`, `Mitigation`, `Leçons apprises`.
- [ ] Commandes clés (`gdb`, `gef`, `pwntools`, `nasm`, `python`).
- [ ] Dump (stack, heap, registre) lorsque pertinent.
- [ ] Extraits de code source C/NASM essentiels et options de compilations lorsque nécessaire (`gcc`, `nasm`, `make`).

## 3. Artefacts
- [ ] Script `pwntools` reproductible (`exploit.py`).
