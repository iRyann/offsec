# Write-up -- 3-heap-02-overflow

#### Analyse préliminaire

**Commande:**

```bash
checksec --file=3-heap-02-overflow
```

**Observation:**

- Full RELRO -> GOT en RO
- NX -> pas d'exécution dans la stack.
- Pas de pie : pas dans randomisation

**Conclusion:** pas d'exec arbitraire simple. Besoin d'un leak, attaque non invasive ou retour à fn déjà résolue.

### Fonction: getstr

- **Adresse:** `0x4011bf`
- **Rôle:** Capture de l'input utilisateur
- **FACT:** Interrompt la capture à la rencontre du retour chariot.
- **HYP:** Pas génante

```c
void getstr(char *b) {
    while((*b = getchar()) && *b != '\n') b++; *b = '\0';
}
```

## [11:19] Analyse du dump

**FACT:** le buffer d'input est stocké en amont de l'adresse de la fonction appelée.

**HYP:** écraser l'adresse suffit à faire exécuter
la fonction `login`

**TEST:** test prévu

**RES:** résultat

---

## [11:19] Phase

**FACT:**

- l'input est stocké à `0x0000000000405310`
- le pointeur est stocké à `0x0000000000405360`

**HYP:** l'absence de pie rend le leak inutile, les adresses seront les mêmes qu'on local

**TEST:**

```asm
ef➤  print 0x00000000004011a5-0x1f
$5 = 0x401186
gef➤  set $rdx=0x401186
```

**RES:**

```
Good job !
[Detaching after vfork from child process 458993]
sh-5.3$
```

---
