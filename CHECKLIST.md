# Checklist — Ajouter un nouveau challenge / Add a New Challenge

## 1. Préparation du dossier / Folder Bootstrap
- [ ] Créer un dossier dédié sous la catégorie adaptée (`stack-overflows/`, `heap/`, `format-strings/`, `shellcodes/`, `reversing/`, etc.).
- [ ] Respecter la structure minimale :
  - `README.md` (optionnel) ou `writeup.md` principal.
  - `src/` ou `challenge/` pour le code source original (C, NASM, Python…).
  - `exploit/` pour les scripts `pwntools`, `gdb` (GEF) ou outils auxiliaires.
  - `notes/` (facultatif) pour les captures, diagrammes ASCII, outputs GDB.
- [ ] Copier le template depuis [`templates/writeup.md`](./templates/writeup.md) vers `writeup.md` et le renseigner.

## 2. Documentation obligatoire / Mandatory Documentation
- [ ] Compléter toutes les sections du template (`Contexte`, `Objectif`, `Analyse`, `Résultat`, `Root cause`, `Mitigation`, `Leçons apprises`).
- [ ] Documenter les commandes clés (`gdb`, `gef`, `pwntools`, `nasm`, `python`) dans la section `Commandes & références`.
- [ ] Ajouter au moins un diagramme ou capture de flux (stack, heap, registre) lorsque pertinent.
- [ ] Inclure les extraits de code source C/NASM essentiels et préciser les compilations (`gcc`, `nasm`, `make`).

## 3. Artefacts techniques / Technical Artefacts
- [ ] Stocker le code vulnérable (`.c`, `.asm`, `.py`) dans un sous-dossier versionné.
- [ ] Fournir un script `pwntools` reproductible (`exploit.py`) avec instructions d’exécution.
- [ ] Ajouter les scripts ou macros GDB/GEF utilisés (`gef.rc`, `commands.txt`, etc.).
- [ ] Vérifier que les binaires générés sont ignorés via `.gitignore` ou placés dans `bin/` avec explication.

## 4. Validation finale / Final Review
- [ ] Relire pour cohérence FR/EN et orthographe.
- [ ] Vérifier que les technologies clés (GDB-GEF, pwntools, NASM, Python, C) sont mentionnées dans `writeup.md`.
- [ ] Lier le challenge depuis `README.md` ou une section pertinente (`highlights/`, `structure du dépôt`).

> _« Préparer, exploiter, documenter — dans cet ordre. »_
