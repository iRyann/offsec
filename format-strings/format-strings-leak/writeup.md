---
title: "Titre du challenge"
challenge: "root-me/picoCTF"
difficulty: "Easy"
platform: "amd64/remote"
date: "YYYY-MM-DD"
tags: [binary, overflow]
author: "Ryan Bouchou"
status: "in-progress"
---

# Titre lisible

**Résumé (1-2 lignes)**
Résumé court...

---

## Contexte

- Source : root-me / cours / picoCTF
- Environnement testé : Ubuntu 22.04, amd64, glibc 2.35
- Fichiers fournis : vuln, main.c, libc.so

---

## Objectif

Récupérer la flag / obtenir un shell

---

## Outils

- gdb + gef / pwndbg
- pwntools (python3)
- readelf / objdump / strings / file

---

## Analyse

### 1) Reconnaissance statique

- commandes & observations

### 2) Analyse dynamique

- breakpoints, comportement runtime
  La stack frame est de taille `0x68`, comme on peut via l'hexdump du main, juste après le prologue :

```asm
 → 0x55ec898bf1fe 4883ec68            <main+0005>      sub    rsp, 0x68
```

Demblée, on remarque l'usage du registre _File Segment_ :

```asm
 mov    rax, QWORD PTR fs:0x28
```

> There was still a need for threads to access thread local store, and each thread needed a a pointer ... somewhere in the immediately accessible thread state (e.g, in the registers) ... to thread local store. -- [source](https://stackoverflow.com/questions/10810203/what-is-the-fs-gs-register-intended-for)

Certainement pour le rand.

Si on avance dans le flot d'exécution, on constate que les appelsà rand() produisent un résultat danx `$eax`, subséquemment copié à `$rbp - 0x68`. Ainsi, la variable `secret` est en tête de la stackframe de `main`, et plus particulièrement

```asm
    0x55ec898bf230 e87bfeffff          <main+0037>      call   0x55ec898bf0b0 <rand@plt>
    0x55ec898bf235 01d8                <main+003c>      add    eax, ebx
    0x55ec898bf237 4898                <main+003e>      cdqe
 →  0x55ec898bf239 48894598            <main+0040>      mov    QWORD PTR [rbp-0x68], rax
```

Maintenant, on vient d'écrire `secret`, comme en témoigne le dump de la stack :

```asm
0x00007ffec08362e0│+0x0000: 0x0000000000000800   ← $rsp
0x00007ffec08362e8│+0x0008: 0xffffffffeb0525df
```

On arrive désormais à la saisie de la variable `username`, laquelle étant située à l'adresse ``comme en témoigne`$rdi`lors de l'appel à`getstr` :

```asm
   0x55ec898bf24e 488d45a0            <main+0055>      lea    rax, [rbp-0x60]
   0x55ec898bf252 4889c7              <main+0059>      mov    rdi, rax
 → 0x55ec898bf255 e85fffffff          <main+005c>      call   0x55ec898bf1b9 <getstr>
```

Ensuite, `lea    rax, [rbp-0x70]` charge `0x00007ffec08362e0` soit `$rsp`, qu'on translate ensuite dans `$rsi`, blabla, puis :

```asm
_isoc99_scanf@plt (
   $rdi = 0x000055ec898c0058 → 0x0000687300646c25 ("%ld"?),
   $rsi = 0x00007ffec08362e0 → 0x0000000000000800,
   $rdx = 0x0000000000000000
)
```

On rapelle l'état de la stack :

```asm
0x00007ffec08362e0│+0x0000: 0x0000000000000800   ← $rsp, $rsi
0x00007ffec08362e8│+0x0008: 0xffffffffeb0525df
```

On conclut avec la comparaison :

```asm
 0x55ec898bf2a5 488b4590            <main+00ac>      mov    rax, QWORD PTR [rbp-0x70]
 → 0x55ec898bf2a9 48394598            <main+00b0>      cmp    QWORD PTR [rbp-0x68], rax
```

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
