#### Test: Suivi des syscalls

**Commande:**

```bash
strace -o ch32.strace ch32.bin
```

**Observation:**
Les caractères sont lus un à un, puis écriture de l'échec.
Retour 0 tout le temps

**Conclusion:** Aucun aperçu de la logique de comparaison,
c'est de la logique applicative, et non kernel.

> [!TIP]
> On peut combiner strace avec `-e trace=read,write,open,connect` pour filtrer seulement ce qui est intéressant (I/O, réseau…), et éviter le bruit des futex, mmap, etc.

#### Test: Exploration des chaînes

**Commande:**

```bash
strings -d ch32.bin
```

**Observation:**
Rien d'éloquent, encryption ou génération dynamique du mot de passe.
Sinon, validation fonctionnelle suivant une logique différente

**Conclusion:** confirmé

## [09:23] Approche bottom-up

**FACT:** On peut tenter de remonter le flot d'exécution depuis
la chaîne `wrong flag`.

**HYP:** On accède ainsi à la fonction responsable de l'embranchement conditionnel

**TEST:**

1. On cherche l'adresse de la chaîne

```zsh
wndbg> search "wrong flag"
Searching for byte: b'wrong flag'
ch32.bin        0x4c4d7a 0x6c6620676e6f7277 ('wrong fl')
```

On a effectivement cette dernière dans `.rodata`

```asm
pwndbg> vmmap
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
             Start                End Perm     Size Offset File (set vmmap-prefer-relpaths on)
          0x400000           0x494000 r-xp    94000      0 ch32.bin
          0x494000           0x52c000 r--p    98000  94000 ch32.bin
          0x52c000           0x53f000 rw-p    13000 12c000 ch32.bin
          0x53f000           0x560000 rw-p    21000      0 [anon_0053f]
    0x7ffff7ff7000     0x7ffff7ffb000 r--p     4000      0 [vvar]
    0x7ffff7ffb000     0x7ffff7ffd000 r--p     2000      0 [vvar_vclock]
    0x7ffff7ffd000     0x7ffff7fff000 r-xp     2000      0 [vdso]
    0x7ffffffdd000     0x7ffffffff000 rw-p    22000      0 [stack]
0xffffffffff600000 0xffffffffff601000 --xp     1000      0 [vsyscall]
```

2. On cherche les parties du code faisant intervenir cette @

#### Test: Lister les fonctions applicatives

**Commande:**

```zsh
strings -n 4 ch32.bin | grep -i main
```

**Observation:** On a, entre autre, la connexité au point d'entrée suivante :

```zsh
runtime.mainStarted
runtime.mainPC
runtime.main
runtime.main.func1
runtime.main.func2
```

**Conclusion:** confirmé/infirmé/incertain
**RES:** résultat

---

> [!TIP] Ontologie de lecture

> | Ghidra dit              | Traduction mentale                      |
> | ----------------------- | --------------------------------------- |
> | `local_xx`              | slot de stack sans sémantique           |
> | `Stack[0xN]`            | offset du stack frame                   |
> | `MOV [RSP+X]`           | écrire un champ de slice / buffer local |
> | `Slice = {ptr,len,cap}` | structure Go 3× 8 octets sur la pile    |
> | `lea [RSP+x]`           | adresse d'un buffer local               |
