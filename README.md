# Offsec – Apprentissage en sécurité offensive

> _Travaux personnels de recherche et d’expérimentation sur la sécurité offensive, la rétro-ingénierie et l’exploitation binaire._

---

## Objectif

Ce dépôt documente mon **cheminement d’apprentissage** dans le domaine du **pentest et du reverse engineering**, au fil des cours, CTF et projets personnels.  
J’y explore les fondamentaux de l’exploitation binaire, de la cryptanalyse et de l’analyse de programmes vulnérables.

## Structure du dépôt

| Dossier                  | Description                                                            | Langages       |
| ------------------------ | ---------------------------------------------------------------------- | -------------- |
| [`/binex/`](./binex)     | Exercices d’exploitation binaire (overflow, format string, shellcode…) | C, Python, ASM |
| [`/crackme/`](./crackme) | Défis de rétro-ingénierie et déobfuscation                             | Python, C      |
| `/notes/` _(à venir)_    | Notes personnelles, mémo GDB/GEF, astuces RE                           | Markdown       |

## TODO

- [ ] Ajouter une section `/notes/` (cheatsheets, concepts)
- [ ] Mettre en place un `progress.md`
- [ ] Préparer un dossier _“research”_
- [ ] Concevoir un script afin de setup rapidement l'environnement de travail (TMUX,...)

---

## Environnement technique

- **OS** : Arch Linux
- **Outils** : GDB + GEF, pwnlib, radare2, imhex, Ghidra
- **Langages** : C, Python, NASM, Zsh
