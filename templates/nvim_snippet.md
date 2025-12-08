-- ~/.config/nvim/lua/snippets/markdown.lua
local ls = require("luasnip")
local s = ls.snippet
local t = ls.text_node
local i = ls.insert_node
local fmt = require("luasnip.extras.fmt").fmt

return {
-- SNIPPET MINIMAL POUR PRISE DE NOTES RAPIDE (utilise ça PENDANT l'exam)
s("note", fmt([[

## [{}] {}

**FACT:** {}

**HYP:** {}

**TEST:** {}

**RES:** {}

---

]], {
i(1, vim.fn.strftime("%H:%M")),
i(2, "Phase"),
i(3, "observation"),
i(4, "hypothèse"),
i(5, "test prévu"),
i(6, "résultat"),
})),

-- SNIPPET FONCTION CRITIQUE (analyse statique)
s("func", fmt([[

### Fonction: {}

- **Adresse:** `{}`
- **Rôle:** {}
- **FACT:** {}
- **HYP:** {}

```c
{}
```

]], {
i(1, "function_name"),
i(2, "0x401234"),
i(3, "description"),
i(4, "observations"),
i(5, "hypothèse"),
i(6, "// pseudo-code Ghidra"),
})),

-- SNIPPET TEST DYNAMIQUE
s("test", fmt([[

#### Test: {}

**Commande:**

```bash
{}
```

**Observation:**
{}

**Conclusion:** {}
]], {
i(1, "description du test"),
i(2, "gdb ./bin"),
i(3, "résultat observé"),
i(4, "confirmé/infirmé/incertain"),
})),

-- SNIPPET STRUCTURE COMPLÈTE (À UTILISER APRÈS L'EXAM pour le rapport final)
s("report", fmt([[

---

title: "{}"
analyst: "Ryan Bouchou"
date: {}
target: "{}"
hashes:
sha256: {}

---

# Reverse Engineering Report

## 1. Contexte & objectifs

### 1.1 Cible

- Binaire: {}
- Arch: {}
- Protections: {}

### 1.2 Objectif

{}

### 1.3 Limites

{}

---

## 2. Environnement

- OS: Arch Linux
- VM: {}
- Tools: Ghidra, gdb+gef, r2, pwntools

---

## 3. Reconnaissance

### 3.1 Propriétés générales

```bash
file {}
checksec --file={}
strings {} | head -20
```

### 3.2 Imports/Exports

{}

### 3.3 Chaînes intéressantes

{}

---

## 4. Analyse statique

### 4.1 Entry point

{}

### 4.2 Fonctions critiques

{}

---

## 5. Analyse dynamique

### 5.1 Tests

{}

### 5.2 Validation hypothèses

{}

---

## 6. Protections & anti-RE

{}

---

## 7. Modèle reconstruit

{}

---

## 8. Points sensibles

{}

---

## 9. Journal chronologique

{}

---

## 10. Synthèse

### Compris

- {}

### Partiellement compris

- {}

### Non résolu

- {}

### Recommandations

- {}

---

## Annexes

### Scripts utilisés

```python
{}
```

]], {
i(1, "Challenge name"),
t(vim.fn.strftime("%Y-%m-%d")),
i(2, "./binary"),
i(3, "à calculer"),
i(4, "./binary"),
i(5, "amd64"),
i(6, "NX, PIE, RELRO"),
i(7, "Comprendre mécanisme X"),
i(8, "Temps limité, pas de réseau"),
i(9, "snapshot-1"),
i(10, "./binary"),
i(11, "./binary"),
i(12, "./binary"),
i(13, "crypto, socket, ptrace..."),
i(14, "URLs, messages d'erreur..."),
i(15, "main @ 0x401234"),
i(16, "check_password, decrypt_data..."),
i(17, "Scénarios + résultats"),
i(18, "HYP → TEST → RES"),
i(19, "Anti-debug, packing..."),
i(20, "Flow d'exécution, structures..."),
i(21, "Surfaces d'entrée, parsing..."),
i(22, "Timeline détaillée"),
i(23, "Point 1"),
i(24, "Point 2"),
i(25, "Point 3"),
i(26, "Suggestions"),
i(27, "# scripts d'analyse"),
})),
}
