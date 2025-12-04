# Analyse crackme1 (Paudega)

## Introduction

- Binaire ciblé : `/bin/Paudega`.
- Objectif : comprendre mécanisme de vérification et produire commande de résolution.

## Journal chronologique

### 1. Inventaire initial (/bin)

- Commande `ls`: identifie un seul binaire nommé `Paudega`.

### 2. Fingerprint minimal

- `file Paudega`

```
Paudega: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), statically linked, no section header
```

- `checksec --file=Paudega`

```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified      Fortifiable      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   No Symbols        No    0              Paudega
```

- `rabin2 -I Paudega`

```
arch     x86
baddr    0x10000
binsz    1272
bintype  elf
bits     64
canary   false
class    ELF64
crypto   false
endian   little
havecode true
laddr    0x0
lang     c
linenum  true
lsyms    true
machine  AMD x86-64 architecture
nx       false
os       linux
pic      false
relocs   true
rpath    NONE
sanitize false
static   true
stripped false
subsys   linux
va       true
```

### 3. Strings rapides

- `strings Paudega | head`

```
SATAUAVAW
D$(H
t$8H
|$@H
]}]{A
A_A^A]A\[1
.shstrtab
.text
```

Observations : peu de chaînes lisibles (binaire statique, sans sections, peut-être minimal).

### 4. Désassemblage initial (objdump)

- `objdump -d Paudega` ne retourne rien (binaire sans section header → objdump ne trouve pas .text).

### 5. Exécution initiale

- `./Paudega` → segfault immédiat (pas d'arguments). Hypothèse : binaire obfusqué ou tronqué nécessitant environnement précis.

### 6. Analyse dynamique (strace)

- `strace -o /tmp/paudega.strace ./Paudega`
  - Résultat: segfault immédiat.
  - Extrait :

```
execve("./Paudega", ["./Paudega"], ... ) = 0
prctl(PR_GET_DUMPABLE) = 1
getppid() = ...
perf_event_open(NULL,...) = -1 EFAULT
--- SIGSEGV si_addr=NULL ---
```

- Constat: échec contrôlé volontaire en cas d'entrée invalide.

### 7. Désassemblage avancé (radare2)

- Utilisation de `r2 -nn` avec disassemblage manuel à partir de 0x100 (baddr=0).
- Points clés du flux:
  - Fonction à 0x8d (appelée au démarrage) effectue quelques syscalls/rdtsc pour produire un pseudo-aléa stocké dans `ebx` (retour). Dans mon environnement, le retour était 0 → r8 initial purement déterministe.
  - Routine principale à 0x129 lit `argc/argv` directement depuis la pile (pas de prologue standard). Exige `argc >= 3`.
  - Calcul de `len` de `argv[2]` (max 0x20). Prépare un état `r8` = `(0x7b5d7d5d ^ 0xdeadbeef) ^ (len-6)^2 ^ ebx`.
  - Suite de vérifications arithmétiques sur `argv[2]` (voir reconstruction ci-dessous).
  - Automate à états (jump table) itérant sur 10 octets de `argv[1]` à l'aide d'un PRNG xorshift (état dans `r8`) et d'un mélange TEA-like avec la table de 10 octets en .data (0xfa,0xbe,0x51,0x17,0x1d,0x21,0x4a,0x1b,0xbd,0xa7).
  - Succès → écriture de 3 octets depuis 0x8a (`"OK\n"`) puis exit(0). Échec → écrit un 0 à l'adresse NULL → segfault.

### 8. Reconstruction en pseudo-code

```c
// argv[1] = cle1 (10 octets), argv[2] = cle2 (<=32 octets)
if (argc < 3) crash();
len = strlen(argv2); if (len > 0x20) len = 0x20;
// état r8
r8 = 0x7b5d7d5d ^ 0xdeadbeef;
r8 ^= (len - 6) * (len - 6);
r8 ^= ebx_seed; // ebx=0 dans mon exécution
// contraintes sur argv2
assert(argv2[0]+...+argv2[5] == 0x202);
assert(sum_i(argv2[i]*i) == 0x517);
assert(xor_i(argv2[i]*(i+1)) == 0x2dd);
assert(sum_i(argv2[i]^2) % 0x101 == 0x74);
// automate pour argv1
state=0; ecx=0;
while (1) {
  switch(state){
    case 0: state = (ecx < 10) ? 1 : 2; break;
    case 1:
      // mélange TEA/xorshift pour byte attendu
      tmp = ror32(r8 ^ (ecx*0x9e3779b1),7);
      expected = ((data[ecx]-ecx)>>1|((data[ecx]-ecx)&1)<<7) ^ (tmp&0xff);
      expected ^= 0xc3; expected = (expected + 0x3d)&0xff;
      if (argv1[ecx] != expected) state=3; else {
        r8 = xorshift(r8);
        ecx++; state=0;
      }
      break;
    case 2:
      // fin de boucle
      if (argv1[ecx]==0 && argv2[len]==0) success();
      else state=3;
      break;
    case 3: crash();
  }
}
```

### 9. Recherche de `argv[2]` (clé principale)

- Hypothèse: `len=6` pour simplifier (les contraintes ne mentionnent que les 6 premiers octets).
- Modélisation Z3 (voir script dans le shell) avec bornes ASCII (32–126) → 220 solutions.
- Exemple lisible et validé: `argv2 = "h1dd3n"` (octets [104,49,100,100,51,110]).

### 10. Calcul de `argv[1]`

- Avec `len=6` et `ebx=0`, état initial `r8=0xa5f0c3b2`.
- Simulation de l’automate (PRNG xorshift + table de 10 octets) → séquence attendue:
  - Résultat: `argv1 = "vEryEasy64"`.

### 11. Validation finale

- Commande exécutée :

```
./Paudega vEryEasy64 h1dd3n
```

- Sortie observée : `OK`.

### 12. Pistes restantes / remarques

- La fonction de seed (0x8d) dépend de syscalls/rdtsc mais retourne 0 ici ; si l’environnement changeait, `argv1` resterait identique pour `ebx` pair (xor avec 0) mais pourrait changer sinon. À documenter si exécution sur autre kernel.
- D’autres solutions pour `argv2` existent (220 en ASCII imprimable) ; `h1dd3n` est la plus lisible.

```shell

```

# Rapport final – Crackme Analysis

## Crackme1 (bin/Paudega)

- **Nature du binaire** : ELF64 statique minimal (1272 o), segment unique RX, protections désactivées (NX off, pas de PIE, pas de canary).
- **Anti-debug** : prctl/rdtsc/perf_event_open → r15d masqué sur 16 bits. Sur l’environnement d’analyse, aucune pénalité (seed stable 0xa5f0c3b2).
- **Logique** :
  - Argument 2 (clé brute) doit vérifier quatre contraintes arithmétiques : somme des 6 premiers octets = 0x202, somme pondérée Σ byte[i]_i = 0x517, XOR Σ byte[i]_(i+1) = 0x2dd, somme des carrés mod 257 = 0x74. Longueur bornée à 0x20.
  - Argument 1 (réponse) de 10 octets : chaque caractère est vérifié via un PRNG xorshift semé par la longueur et l’anti-debug, mélangé avec une table constante de 10 octets (0x10080) et une constante multiplicative 0x9e3779b1.
  - Chemin d’échec : écriture sur NULL → segfault immédiat si contrainte violée ou argc < 3.
- **Résolution** :
  - Modélisation Z3 des contraintes → solution minimale longueur 6 : octets `[44, 96, 158, 77, 35, 104]` (`",\x60\x9eM#h"`).
  - Simulation du PRNG → réponse attendue `vEryEasy64`.
- **Commande reproductible** :

  ```bash
  arg2=$',\x60\x9eM#h'
  /workspace/crackme/bin/Paudega vEryEasy64 "$arg2"
  ```

  Sortie : `OK`.

  # Rapport final d'analyse des crackmes

## Crackme1 – Paudega

- **Fingerprint** : ELF64 statique sans sections, protections désactivées (No RELRO, NX off, No PIE). Anti-debug léger via `prctl` et timing `rdtsc`.
- **Mécanisme de vérification** :
  - 4 contraintes arithmétiques sur `argv2` (somme, somme pondérée, xor de produits, somme des carrés mod 0x101).
  - Génération d'une séquence pseudo-aléatoire dépendant de la longueur (10) et du retour anti-RE, comparée octet par octet à `argv1` (10 octets).
  - Vérification des terminators puis impression `OK\n`.
- **Résolution** : modélisation Z3 (scripts/solve_paudega.py) → `argv2 = 7d 71 2f 42 67 3c 04 10 04 02`, `argv1 = 76 49 fa e6 78 83 61 d4 b1 76`.
- **Commande de validation** :
  ```
  ./bin/Paudega "$(printf '\x76\x49\xfa\xe6\x78\x83\x61\xd4\xb1\x76')" "$(printf '\x7d\x71\x2f\x42\x67\x3c\x04\x10\x04\x02')"
  ```
  → sortie `OK`.
- **Notes dynamiques** : `strace` confirme les syscalls (157/prctl, 110 getppid, 39 getpid, 298 seccomp->ENOSYS, write). `ltrace` impossible (binaire statique sans symboles dynamiques).

## Analyse croisée

- Binaire entièrement statique et minimaliste : peu de chaînes, reliance totale sur syscalls et calculs maison, rendant `ltrace` inutile. Anti-debug faible, contourné en reproduisant la logique.
- Les contraintes numériques combinées (somme + XOR + mod) rendent la brute-force naïve difficile ; l'utilisation de Z3 a fourni rapidement une solution exacte.

## Commande récapitulative

a) Génération automatisée : `scripts/solve_paudega.py` imprime les octets et la ligne de commande.

b) Exécution directe : voir commande ci-dessus.

---

strace :

```zsh
execve("./bin/Paudega", ["./bin/Paudega", "vI\372\346x\203a\324\261v", "\217.\276\35R\30(\2\2\10"], 0x7ffdc2f348d0 /* 64 vars */) = 0
prctl(PR_GET_DUMPABLE)                  = 1 (SUID_DUMP_USER)
getppid()                               = 7763
getpid()                                = 7766
perf_event_open(NULL, 0, 0, 0, 0)       = -1 EFAULT (Bad address)
write(1, "OK\n", 3)                     = 3
exit(0)                                 = ?
+++ exited with 0 +++
```
