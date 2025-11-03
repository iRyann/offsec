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
| Projet · Project | Badges | Description & Lien |
| --- | --- | --- |
| Ret2libc Real-World Chain | ![Tech: ret2libc](https://img.shields.io/badge/Tech-ret2libc-critical?style=flat-square) ![Level: Avancé](https://img.shields.io/badge/Niveau-Avanc%C3%A9-red?style=flat-square) | **FR :** ret2libc complet avec fuite dynamique. **EN:** Full ret2libc with live leak. → [Voir le résumé](./highlights/README.md#ret2libc-real-world-chain-stack-overflows-ret2main) |
| Complex ROP Pivot | ![Tech: ROP](https://img.shields.io/badge/Tech-ROP-critical?style=flat-square) ![Focus: Stack Pivot](https://img.shields.io/badge/Focus-Stack%20Pivot-orange?style=flat-square) | **FR :** Pivot `.bss` et ROP multi-phase. **EN:** `.bss` pivot with multi-stage ROP. → [Voir le résumé](./highlights/README.md#complex-rop-pivot-stack-overflows-ret2plt-advanced) |
| Advanced Format String | ![Tech: Format String](https://img.shields.io/badge/Tech-Format%20String-ff69b4?style=flat-square) ![Focus: Multiwrite](https://img.shields.io/badge/Focus-Multiwrite-brown?style=flat-square) | **FR :** Multi-écriture `%hn` orchestrée. **EN:** Orchestrated `%hn` multi-write. → [Voir le résumé](./highlights/README.md#advanced-format-string-multi-write-format-strings-multiwrite) |
| Custom Shellcode vs Canary | ![Tech: Shellcode](https://img.shields.io/badge/Tech-Shellcode-blue?style=flat-square) ![Focus: Canary Bypass](https://img.shields.io/badge/Focus-Canary%20Bypass-brown?style=flat-square) | **FR :** Shellcode polymorphique après fuite du canari. **EN:** Polymorphic shellcode after canary leak. → [Voir le résumé](./highlights/README.md#custom-shellcode-vs-canary-shellcodes-ret2sc-canary) |

---

## Structure du dépôt / Repository Map
| Dossier | Description FR | EN Description | Langages | Badges |
| --- | --- | --- | --- | --- |
| [`/stack-overflows/`](./stack-overflows) | Exploitation de dépassements de pile. | Stack overflow exploitation labs. | C, Python | ![Tech: ROP](https://img.shields.io/badge/Tech-ROP-critical?style=flat-square) ![Level Mix](https://img.shields.io/badge/Niveaux-2%E2%80%935%E2%98%85-blueviolet?style=flat-square) |
| [`/format-strings/`](./format-strings) | Vulnérabilités de chaînes de format. | Format string challenges. | C, Python | ![Tech: Format String](https://img.shields.io/badge/Tech-Format%20String-ff69b4?style=flat-square) ![Focus: Writes](https://img.shields.io/badge/Focus-Writes-green?style=flat-square) |
| [`/shellcodes/`](./shellcodes) | Construction de shellcodes personnalisés. | Custom shellcode crafting. | C, Python, ASM | ![Tech: Shellcode](https://img.shields.io/badge/Tech-Shellcode-blue?style=flat-square) ![Focus: Constraints](https://img.shields.io/badge/Focus-Constraints-teal?style=flat-square) |
| [`/heap/`](./heap) | Scénarios de corruption de tas. | Heap exploitation playground. | C, Python | ![Tech: Heap](https://img.shields.io/badge/Tech-Heap-9cf?style=flat-square) ![Focus: Fastbin](https://img.shields.io/badge/Focus-Fastbin-purple?style=flat-square) |
| [`/misc/`](./misc) | Challenges transverses et automatisation. | Miscellaneous automation challenges. | C, Python | ![Focus: Automation](https://img.shields.io/badge/Focus-Automation-yellow?style=flat-square) ![Level Mix](https://img.shields.io/badge/Niveaux-2%E2%80%934%E2%98%85-orange?style=flat-square) |
| [`/reversing/`](./reversing) | Études de binaires obfusqués. | Reverse engineering notebooks. | Python, C | ![Tech: RE](https://img.shields.io/badge/Tech-Reversing-9cf?style=flat-square) ![Focus: VM](https://img.shields.io/badge/Focus-VM-green?style=flat-square) |
| [`/highlights/`](./highlights) | Synthèses des projets marquants. | Key project summaries. | Markdown | ![Meta: Writeups](https://img.shields.io/badge/Meta-Writeups-lightgrey?style=flat-square) |
| [`/templates/`](./templates) | Gabarits de scripts et fiches méthodes. | Script templates & cheat sheets. | Markdown | ![Meta: Templates](https://img.shields.io/badge/Meta-Templates-00ced1?style=flat-square) |
| `/notes/` _(à venir)_ | Ressources personnelles et mémos. | Personal notes & cheat sheets. | Markdown | ![Status: TODO](https://img.shields.io/badge/Status-TODO-lightgrey?style=flat-square) |

---

## Outils principaux / Core Tooling
- **FR :** GNU/Linux, tmux, GDB + GEF, pwntools, rizin/radare2, Ghidra, Binary Ninja (communautaire), qemu-user.
- **EN:** GNU/Linux, tmux, GDB + GEF, pwntools, rizin/radare2, Ghidra, Binary Ninja (community), qemu-user.

### Technologies clés / Key Technologies
- **GDB + GEF :** débogage dynamique, inspection mémoire et instrumentation de scénarios pwn ; dynamic debugging & memory inspection.
- **pwntools :** automatisation des exploits, génération de payloads, fuzzing ciblé ; exploit automation & payload orchestration.
- **NASM :** génération d'opcodes précis pour shellcodes et gadgets personnalisés ; handcrafted shellcode authoring.
- **Python 3 :** scripts d'automatisation, tooling auxiliaire et génération de données ; automation scripting backbone.
- **C :** base des binaires vulnérables, prototypes de vulnérabilités et harness de test ; vulnerable binary authoring.

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
