# Offsec – Apprentissage en sécurité offensive

> _Travaux personnels de recherche et d’expérimentation sur la sécurité offensive, la rétro-ingénierie et l’exploitation binaire._

---

## Objectif

Ce dépôt documente mon **cheminement d’apprentissage** dans le domaine du **pentest et du reverse engineering**, au fil des cours,
CTF et projets personnels. J’y explore les fondamentaux de l’exploitation binaire, de la cryptanalyse et de l’analyse de programmes
vulnérables.

## Structure du dépôt

| Dossier                                      | Description                                                            | Langages       |
| -------------------------------------------- | ---------------------------------------------------------------------- | -------------- |
| [`/stack-overflows/`](./stack-overflows)     | Exercices dédiés aux dépassements de pile et retours de fonctions      | C, Python      |
| [`/format-strings/`](./format-strings)       | Défis d’exploitation de chaînes de format                             | C, Python      |
| [`/shellcodes/`](./shellcodes)               | Génération et injection de shellcodes personnalisés                    | C, Python, ASM |
| [`/heap/`](./heap)                           | Vulnérabilités liées au tas et aux segments de données                 | C, Python      |
| [`/misc/`](./misc)                           | Challenges variés (automatisation, logique, conversions, etc.)         | C, Python      |
| [`/reversing/`](./reversing)                 | Défis de rétro-ingénierie et de déobfuscation                          | Python, C      |
| [`/highlights/`](./highlights)               | Placeholders pour résumés de challenges marquants                      | Markdown       |
| [`/templates/`](./templates)                 | Gabarits de scripts et documentation                                   | Markdown       |
| `/notes/` _(à venir)_                        | Notes personnelles, mémo GDB/GEF, astuces RE                           | Markdown       |

## TODO

- [ ] Ajouter une section `/notes/` (cheatsheets, concepts)
- [ ] Mettre en place un `progress.md`
- [ ] Préparer un dossier _“research”_
- [ ] Concevoir un script afin de setup rapidement l'environnement de travail (TMUX,...)

---

## Environnement technique

- **OS** : GNU/Linux
- **Outils** : GDB + GEF, pwntools, radare2, imhex, Ghidra
- **Langages** : C, Python, NASM, Zsh
