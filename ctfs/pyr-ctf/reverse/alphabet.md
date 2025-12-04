---
title: "Alphabet"
challenge: "pyr-ctf/reverse"
difficulty: "Easy"
platform: "amd64"
date: "2025-11-27"
tags: [binary, reverse]
author: "Ryan Bouchou"
status: "in-progress"
---

# Alphabet

**Résumé**
Résumé court...

---

## Contexte

- Source : pyr-ctf
- Environnement testé : Arch Linux, amd64, glibc 2.35
- Fichiers fournis : vuln

---

## Objectif

Récupérer le flag

---

## Outils

- gdb + gef
- pwntools (python3)
- checksec / readelf / objdump / strings / file

---

## Analyse

### 1) Reconnaissance statique

On commence par observer les mécanismes de sécurité implémenté dans le binaire :

```zsh
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH	Symbols		FORTIFY	Fortified	Fortifiable	FILE
Partial RELRO   No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   41 Symbols	 No	0		2	alphabet
```

Partant de là, on sait que :

- la stack n'est pas exécutable,
- on peut réécrire `.got.plt.`

On peut regarder les chaînes encodées en dur avec la commande `strings` :

```zsh
[...]
Enter the password:
Correct password!
abcdefghijklmnopqrstuvwxyz0123456789
;*3$"
[...]
```

Jusque là rien d'éloquent, sinon un alphabet ; on sent venir le codage ?

Voyons ce que donne le programme dans GDB.
On note la présence des fonctions suivantes :

```zsh

```

On place un breakpoint sur main et on effectue une passe pour comprendre le flot d'exécution.
On place un second bp sur `main+25`, sur `fgets` afin de voir à quel emplacement mémoire on stocke
la saisie utilisateur, et quelle est la taille du buffer.

```asm
0x401094 be09000000          <main+0014>      mov    esi, 0x9
0x401099 488d7c2407          <main+0019>      lea    rdi, [rsp+0x7]
0x40109e 488b159b2f0000      <main+001e>      mov    rdx, QWORD PTR [rip+0x2f9b]        # 0x404040 <stdin@GLIBC_2.2.5>
0x4010a5 e8c6ffffff          <main+0025>      call   0x401070 <fgets@plt>
```

Ainsi, on constate que :

- la saisie utilisateur ne doit pas excéder 9 octets,
- le buffer se situe à `rsp+7`

Ensuite, on le programme détermine la taille de notre saisie :

```asm
    0x4010aa 488d7c2407          <main+002a>      lea    rdi, [rsp+0x7]
    0x4010af be19204000          <main+002f>      mov    esi, 0x402019
 →  0x4010b4 e8a7ffffff          <main+0034>      call   0x401060 <strcspn@plt>
```

> [!INFO] Fonction `strcspn`
> Renvoie la longueur de la plus grande sous-chaîne (en partant du début de la chaîne initiale) ne contenant aucun des caractères spécifiés dans la liste des caractères en rejet.

```asm
    lea    rdi, [rsp+0x7]
    0x4010be c644040700          <main+003e>      mov    BYTE PTR [rsp+rax*1+0x7], 0x0
 →  0x4010c3 e878ffffff          <main+0043>      call   0x401040 <strlen@plt>

```

On observe que le résultat de `strcspn` dicte l'emplacement du stockage de `0x0`  
Dans le cas ou le résultat du `strlen` suivant, si le résultat n'est pas égal à `8`, on jump à `main + 135` :

```asm
gef➤  x/6i *main + 135
   0x401107 <main+135>: mov    eax,0x1
   0x40110c <main+140>: add    rsp,0x30
   0x401110 <main+144>: pop    rbx
   0x401111 <main+145>: pop    rbp
   0x401112 <main+146>: pop    r12
   0x401114 <main+148>: ret
```

Le premier caractère du notre input est stocké dans `rbp` :

```asm
0x4010e3 420fb66c2407        <main+0063>      movzx  ebp, BYTE PTR [rsp+r12*1+0x7]
```

### 2) Analyse dynamique

- breakpoints, comportement runtime

### 3) Exploit

Stratégie : ret2libc / overflow / format-string

Payload (extrait) :

```py
from pwn import *
context.update(arch='amd64', timeout=2)
p = process('./build/a.out')
p.sendline(b'...')
p.interactive()
```

---

## Résultat

- Flag : CTF{...}

## Root cause

Explication courte du bug

## Mitigation

- corrections proposées

## Leçons apprises / next steps

- pistes d'amélioration

## Commandes & références

- readelf -a binary

## Artefacts

- exploit.py, build/
