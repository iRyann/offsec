# Offensive Security - Parcours en sécurité offensive

![Difficulty](https://img.shields.io/badge/Difficult%C3%A9-Interm%C3%A9diaire-orange?style=for-the-badge)
![Techniques](https://img.shields.io/badge/Techniques-Pwn%20%7C%20RE%20%7C%20Crypto-blueviolet?style=for-the-badge)
![Status](https://img.shields.io/badge/Statut-En%20cours%20%2F%20Work%20in%20progress-success?style=for-the-badge)

---

## Sommaire · Table of Contents
- [Objectif du dépôt](#objectif-du-dépôt--repository-goal)
- [Méthodologie d'exploitation ](#méthodologie-dexploitation--exploitation-workflow)
- [Projets mis en avant](#projets-mis-en-avant--highlight-projects)
- [Structure du dépôt](#structure-du-dépôt--repository-map)
- [Outils principaux](#outils-principaux--core-tooling)
- [Compétences développées](#compétences-développées--skills-spotlight)
- [Pistes de contribution](#pistes-de-contribution--contribution-checklist)

---

## Objectif du dépôt
Consolider les fondamentaux (mémoire, ABI, mitiger) en construisant des exploit proofs-of-concept et en capitalisant les retours d'expérience.

## Méthodologie d'exploitation
| Étape | Focus | Livrables |
| --- | --- | --- |
| 1. Analyse statique | Cartographie du binaire, identification des gadgets, étude des protections. | Checklist de protections, arbre de fonctions. |
| 2. Analyse dynamique | Instrumentation (GDB/GEF, rr), traçage des registres et de la mémoire. | Scripts pwntools, breakpoints annotés, traces de crash. |
| 3. Exploit | Mise en place du payload, automatisation, documentation finale. | Exploit final, write-up `/highlights`, scripts reproductibles. |

## Projets mis en avant
| Projet                     | Description & Lien                                                                                                                                     |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Ret2libc Real-World Chain  | ret2libc complet avec fuite dynamique. [Voir le résumé](./highlights/README.md#ret2libc-real-world-chain-stack-overflows-ret2main)          |
| Complex ROP Pivot          | Pivot `.bss` et ROP multi-phase. [Voir le résumé](./highlights/README.md#complex-rop-pivot-stack-overflows-ret2plt-advanced)                |
| Advanced Format String     | Multi-écriture `%hn` orchestrée. [Voir le résumé](./highlights/README.md#advanced-format-string-multi-write-format-strings-multiwrite)      |
| Custom Shellcode vs Canary | Shellcode polymorphique après fuite du canari. [Voir le résumé](./highlights/README.md#custom-shellcode-vs-canary-shellcodes-ret2sc-canary) |

## Structure du dépôt
| Dossier                                  | Description                            | Langages       |
| ---------------------------------------- | ----------------------------------------- | -------------- |
| [`/stack-overflows/`](./stack-overflows) | Exploitation de dépassements de pile.     | C, Python      |
| [`/format-strings/`](./format-strings)   | Vulnérabilités de chaînes de format.      | C, Python      |
| [`/shellcodes/`](./shellcodes)           | Construction de shellcodes personnalisés. | C, Python, ASM |
| [`/heap/`](./heap)                       | Scénarios de corruption de tas.           | C, Python      |
| [`/misc/`](./misc)                       | Challenges transverses et automatisation. | C, Python      |
| [`/reversing/`](./reversing)             | Études de binaires obfusqués.             | Python, C      |
| [`/highlights/`](./highlights)           | Synthèses des projets marquants.          | Markdown       |
| [`/templates/`](./templates)             | Gabarits de scripts et fiches méthodes.   | Markdown       |
| `/notes/` *(à venir)*                    | Ressources personnelles et mémos.         | Markdown       |


## Outils principaux
- GNU/Linux, tmux, GDB + GEF, pwntools, radare2, Ghidra...

## Technologies clés
- **GDB + GEF :** débogage dynamique, inspection mémoire et instrumentation de scénarios pwn ; dynamic debugging & memory inspection.
- **pwntools :** automatisation des exploits, génération de payloads, fuzzing ciblé ; exploit automation & payload orchestration.
- **NASM :** génération d'opcodes précis pour shellcodes et gadgets personnalisés ; handcrafted shellcode authoring.
- **Python 3 :** scripts d'automatisation, tooling auxiliaire et génération de données ; automation scripting backbone.
- **C :** base des binaires vulnérables, prototypes de vulnérabilités et harness de test ; vulnerable binary authoring.

## Compétences développées
- Exploitation mémoire (ret2libc, ROP, heap feng shui).
- Automatisation Python pour fuzzing ciblé.
- Rétro-ingénierie statique/dynamique et patching.
- Déploiement d'environnements reproductibles.
