# Offsec – Offensive Security Learning Journey / Parcours en sécurité offensive

![Difficulty](https://img.shields.io/badge/Difficult%C3%A9-Interm%C3%A9diaire-orange?style=for-the-badge)
![Techniques](https://img.shields.io/badge/Techniques-Pwn%20%7C%20RE%20%7C%20Crypto-blueviolet?style=for-the-badge)
![Status](https://img.shields.io/badge/Statut-En%20cours%20%2F%20Work%20in%20progress-success?style=for-the-badge)

> **FR :** Dépôt personnel dédié à l'exploration de la sécurité offensive, de la rétro-ingénierie et de l'exploitation binaire.
>
> **EN:** Personal workspace for exploring offensive security, reverse engineering, and binary exploitation.

---

## Sommaire · Table of Contents

- [Profil / Profile](#profil--profile)
- [Objectif du dépôt / Repository Goal](#objectif-du-dépôt--repository-goal)
- [Méthodologie d'exploitation / Exploitation Workflow](#méthodologie-dexploitation--exploitation-workflow)
- [Projets mis en avant / Highlight Projects](#projets-mis-en-avant--highlight-projects)
- [Structure du dépôt / Repository Map](#structure-du-dépôt--repository-map)
- [Outils principaux / Core Tooling](#outils-principaux--core-tooling)
- [Compétences développées / Skills Spotlight](#compétences-développées--skills-spotlight)
- [Pistes de contribution / Contribution Checklist](#pistes-de-contribution--contribution-checklist)

---

## Profil / Profile
**FR :** Passionné par la compréhension bas niveau des systèmes, j'utilise ce dépôt comme laboratoire pour expérimenter les techniques d'exploitation modernes et documenter mes apprentissages.

**EN:** Fascinated by low-level internals, I treat this repository as a lab to experiment with modern exploitation techniques and keep structured learning notes.

---

## Objectif du dépôt / Repository Goal
- **FR :** Consolider les fondamentaux (mémoire, ABI, mitiger) en construisant des exploit proofs-of-concept et en capitalisant les retours d'expérience.
- **EN:** Solidify the fundamentals (memory internals, ABI, mitigations) by building proof-of-concept exploits and documenting lessons learned.

---

## Méthodologie d'exploitation / Exploitation Workflow
| Étape · Step | Focus FR | Focus EN | Livrables · Deliverables |
| --- | --- | --- | --- |
| 1. Analyse statique · Static analysis | Cartographie du binaire, identification des gadgets, étude des protections. | Map the binary, enumerate gadgets, review mitigations. | Notes Ghidra/rizin, arbres de fonctions, checklist de protections. |
| 2. Analyse dynamique · Dynamic analysis | Instrumentation (GDB/GEF, rr), traçage des registres et de la mémoire. | Instrumentation (GDB/GEF, rr), tracing registers & memory. | Scripts pwntools, breakpoints annotés, traces de crash. |
| 3. Exploit · Exploitation crafting | Mise en place du payload, automatisation, documentation finale. | Build the payload, automate, write the final report. | Exploit final, write-up `/highlights`, scripts reproductibles. |

---

## Projets mis en avant / Highlight Projects
| Projet · Project | Visuel · Visual | Description & Lien |
| --- | --- | --- |
| Stack Overflow "ret2libc" | ![ret2libc Visual](https://img.shields.io/badge/ret2libc-stack%20pivoting-critical?style=flat-square&logo=stackshare) | **FR :** Chaîne retour vers libc avec contournement de `NX`. **EN:** ret2libc chain bypassing NX. → [Voir le résumé](./highlights/README.md#stack-overflow-ret2libc) |
| Heap "Fastbin Frenzy" | ![Fastbin Visual](https://img.shields.io/badge/Fastbin-Frenzy-purple?style=flat-square&logo=linux) | **FR :** Corruption de tas via double free. **EN:** Double-free driven heap corruption. → [Voir le résumé](./highlights/README.md#heap-fastbin-frenzy) |
| Reversing "Obfuscation Lab" | ![Obfuscation Visual](https://img.shields.io/badge/RE-Obfuscation%20Lab-9cf?style=flat-square&logo=gnuemacs) | **FR :** Déobfuscation d'un crackme et reconstruction de logique. **EN:** Deobfuscating a crackme to rebuild original logic. → [Voir le résumé](./highlights/README.md#reversing-obfuscation-lab) |
| Crypto/Pwn Automation | ![Automation Visual](https://img.shields.io/badge/Automation-crypto%20solver-important?style=flat-square&logo=python) | **FR :** Pipeline pwntools pour signer et casser un schéma oracle. **EN:** Pwntools pipeline for oracle-based signing attack. → [Voir le résumé](./highlights/README.md#automation-oracle-attack) |

---

## Structure du dépôt / Repository Map
| Dossier | Description FR | EN Description | Langages |
| --- | --- | --- | --- |
| [`/stack-overflows/`](./stack-overflows) | Exploitation de dépassements de pile. | Stack overflow exploitation labs. | C, Python |
| [`/format-strings/`](./format-strings) | Vulnérabilités de chaînes de format. | Format string challenges. | C, Python |
| [`/shellcodes/`](./shellcodes) | Construction de shellcodes personnalisés. | Custom shellcode crafting. | C, Python, ASM |
| [`/heap/`](./heap) | Scénarios de corruption de tas. | Heap exploitation playground. | C, Python |
| [`/misc/`](./misc) | Challenges transverses et automatisation. | Miscellaneous automation challenges. | C, Python |
| [`/reversing/`](./reversing) | Études de binaires obfusqués. | Reverse engineering notebooks. | Python, C |
| [`/highlights/`](./highlights) | Synthèses des projets marquants. | Key project summaries. | Markdown |
| [`/templates/`](./templates) | Gabarits de scripts et fiches méthodes. | Script templates & cheat sheets. | Markdown |
| `/notes/` _(à venir)_ | Ressources personnelles et mémos. | Personal notes & cheat sheets. | Markdown |

---

## Outils principaux / Core Tooling
- **FR :** GNU/Linux, tmux, GDB + GEF, pwntools, rizin/radare2, Ghidra, Binary Ninja (communautaire), qemu-user.
- **EN:** GNU/Linux, tmux, GDB + GEF, pwntools, rizin/radare2, Ghidra, Binary Ninja (community), qemu-user.

---

## Compétences développées / Skills Spotlight
- **FR :**
  - Exploitation mémoire (ret2libc, ROP, heap feng shui).
  - Automatisation Python pour fuzzing ciblé.
  - Rétro-ingénierie statique/dynamique et patching.
  - Déploiement d'environnements reproductibles.
- **EN:**
  - Memory exploitation (ret2libc, ROP, heap feng shui).
  - Python automation for targeted fuzzing.
  - Static/dynamic reverse engineering & patching.
  - Reproducible lab environment deployment.

---

## Pistes de contribution / Contribution Checklist
- [ ] Ajouter la section `/notes/` avec mémos GDB/GEF et patterns de payloads.
- [ ] Documenter un pipeline de fuzzing intégré (AFL++, honggfuzz).
- [ ] Étendre `/highlights/` avec des write-ups détaillés et scripts reproductibles.
- [ ] Publier un script d'initialisation d'environnement (dotfiles, tmuxinator).
- [ ] Ajouter des captures d'écran et schémas supplémentaires pour chaque projet clé.

---

> _« Learn, exploit, document, repeat. »_
