## [18:49] Prise en main du binaire

**FACT:** ELF section header corrupted.

**HYP:**  Il faut rétablir l'intrégité de la structure ELF.
En l'occurence :
- sh_size
- sh_offset
-  Table d'index des chaînes d'en-tête de section

**TEST:** On édite :
```
Taille des en-têtes de section:    0 (octets)
Nombre d'en-têtes de section:      0
Table d'index des chaînes d'en-tête de section: 0
```

**RES:** Le programme est désormais débuggable dans gdb. 

---

## [18:49] Analyse statique

**FACT:** `in_stack_00000010 != 0` évite le `exit`  

**HYP:** argument ?

**TEST:** test prévu

**RES:** résultat

---
