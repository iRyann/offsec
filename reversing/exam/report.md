---
title: "Examen"
analyst: "Ryan Bouchou"
date: 2025-12-12
target: "./exam"
hashes:
  md5: c880642d2ad7a1a2c99b972a408b40b4
---

# Reverse Engineering Report

## 1. Contexte & objectifs

### 1.1 Cible

- Binaire: ./exam
- Architecture: amd64
- Type: ELF exécutable
- Protections apparentes: aucune protection classique identifiée (PIE, RELRO, etc.), mais structure ELF volontairement incohérente

### 1.2 Objectif

Comprendre le mécanisme réel de chargement et d'exécution des instructions, ainsi que le rôle exact des arguments passés au programme, afin d'identifier le modèle de validation attendu.

### 1.3 Limites

- Temps d'analyse limité
- Environnement restreignant certaines techniques de debug (ptrace, signaux)
- Structure ELF volontairement non conforme compliquant l'analyse classique

---

## 2. Environnement

- OS: Arch Linux (x86_64)
- Outils:
  - gdb avec pwndbg
  - strace
  - imhex
  - outils ELF standards (readelf, objdump), partiellement exploitables

---

## 3. Reconnaissance

### 3.1 Propriétés générales

```bash
file ./exam
checksec --file=./exam
```

Résultat notable:

- Le fichier est reconnu comme ELF, mais avec une classe et des champs e_ident invalides.
- checksec ne fournit pas d'information exploitable, la structure ELF étant illisible.

Observation:
Le binaire reste néanmoins exécutable par le noyau, ce qui suggère que l'ELF est volontairement corrompu mais suffisant pour le loader Linux.

---

## 4. Analyse statique

### 4.1 Entry point

L'entry point reporté par readelf est incohérent (0x4eaf055) au regard de la taille réelle du fichier.
En pratique, l'exécution observée débute dans une zone mémoire anonyme, mappée dynamiquement.

### 4.2 Fonctions critiques

Aucune fonction symbolique identifiable.
L'analyse par désassemblage brut met en évidence un bloc d'initialisation suivi d'un mécanisme de dispatch indirect basé sur des sauts calculés.

Instructions structurantes observées:

- Séquence initiale:
  pop rsi
  pop rbp
  pop rsi

- Dispatcher:
  call <dispatcher>
  pop rax
  lodsb
  jmp rax

Ce schéma se répète indépendamment de la provenance exacte du code.

---

## 5. Analyse dynamique

### 5.1 Tests

Scénarios testés:

- Exécution sans argument:
  Résultat: SIGSEGV immédiat (lecture mémoire NULL)

- Exécution avec un argument simple:
  Résultat: variation de l'adresse de crash et des registres, mais crash systématique

- Exécution avec plusieurs arguments:
  Résultat: le contenu de argv[i] influence directement les registres (notamment RSI et la pile)

### 5.2 Validation d'hypothèses

Hypothèse:
Les arguments sont interprétés comme des données, pas comparés.

Test:
Observation de lodsb lisant directement argv[1], puis saut indirect via jmp rax.

Résultat:
Hypothèse confirmée. Chaque octet de l'argument modifie directement le flot de contrôle.

---

## 6. Protections & anti-RE

- Structure ELF volontairement corrompue
- Entrée effective déplacée dans une zone mémoire anonyme
- Usage intensif de sauts indirects
- Forte sensibilité à l'état initial des registres
- Absence de symboles et de sections exploitables

Ces éléments constituent une protection logicielle plus qu'un anti-debug explicite.

---

## 7. Modèle reconstruit

Le binaire implémente un interpréteur minimaliste:

- Chaque octet des arguments est lu séquentiellement via lodsb
- Cet octet est utilisé comme offset relatif dans la même page mémoire
- L'exécution saute vers le bloc correspondant
- Certains offsets rebouclent vers le dispatcher
- D'autres provoquent des syscalls ou des crashes

Le mot de passe attendu n'est donc pas une valeur statique, mais un programme minimal encodé sous forme d'arguments.

---

## 8. Points sensibles

- Lecture implicite du byte NULL terminal des chaînes C
- Dépendance forte à l'ordre et au nombre d'arguments
- Effets de pile induits par pop et ret
- Impossibilité d'éviter certains chemins invalides sans préparation explicite

---

## 9. Journal chronologique

# Journal de bord

## [10:14] Prédicats du binaire

**FACT:** Le binaire répond aux conditions suivantes :

1. ELF x64, compilé depuis divers langages.
2. Le mot de passe doit être passé via les args.
3. Un argument ne peut pas dépasser 16 bytes.
4. L'exécutable doit afficher **UNIQUEMENT "OK\n" sur STDOUT et retourner zéro** si le mot
   de passe est correct.
5. Bruteforce d'au plus une seconde au besoin.
6. Selon la taille du binaire, il peut y avoir plus d'un argument :
   - Un bianire <= 8 KiB : max 1 args.
   - Un binaire <= 4 KiB : max 2 args
   - Un binaire <= 2 KiB : max 4 args
   - Un binaire <= KiB : max 8 args
   - etc... sans limite basse.
7. L'ELF est décodé depuis la base 64 :

```zsh
󰣇 offsec/reversing/exam   main  ✘!? ❯ md5sum exam
c880642d2ad7a1a2c99b972a408b40b4  exam
```

---

## [10:14] Première approche

**FACT:** Le binaire est certes exécutable, mais pas débuggable.

**HYP:** Le format ELF, les headers, sont corrumpus.

**TEST:**

- On regarde les informations sur le fichier

```zsh
exam: ELF, unknown class 102
```

- On lance gdb dessus :

```gdb
pwndbg> file
No executable file now.
No symbol file now.
```

**RES:** Hypothèse confirmée

---

## [10:14] Rétablissement de la structure ELF

**FACT:**

```zsh
En-tête ELF:
  Magique:   7f 45 4c 46 66 0f 91 ac 89 c7 b2 03 0f 05 eb 19
  Classe:                            <inconnu: 66>
  Données:                          <inconnu: f>
  Version:                           145 <inconnu>
  OS/ABI:                            <inconnu: ac>
  Version ABI:                       137
  Type:                              EXEC (fichier exécutable)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0xa95d5bbb
  Adresse du point d'entrée:         0x4eaf055
  Début des en-têtes de programme :  2795 (octets dans le fichier)
  Début des en-têtes de section :    56 (octets dans le fichier)
  Fanions:                           0x0
  Taille de cet en-tête:             45251 (octets)
  Taille de l'en-tête du programme:  35132 (octets)
  Nombre d'en-tête du programme:     4071
  Taille des en-têtes de section:    34309 (octets)
  Nombre d'en-têtes de section:      50113
  Table d'index des chaînes d'en-tête de section: 28331

readelf: Avertissement : Le champ e_shentsize dans l'en-tête ELF est plus grand que la taille d'un en-tête de section ELF
readelf: ERREUR : La lecture de 1719326917 octets va au delà de la fin du fichier pour En-têtes de section
readelf: ERREUR : En-têtes de section ne sont pas disponibles !
readelf: ERREUR : Trop d'en-têtes de programme — 0xfe7 — le fichier n'est pas si grand

Il n'y a pas de section dynamique dans ce fichier.
readelf: ERREUR : Trop d'en-têtes de programme — 0xfe7 — le fichier n'est pas si grand
```

**HYP:** On prend les corrections à faire une par une,
pour essayer de rendre la lecture de la structure ELF faisable.

---

## [11:01:43] Correction de `e_ident`

**FACT:** Incohérences

**HYP:**

- 64 bits, little-endian
- ei_version 1
- padding nul
- e_version 1
- point d'entrée à tronquer, mais permettant pourtant l'exécution.
- pas de headers

**TEST:**

```gdb
❯ ./exam de    11:38
zsh: erreur de format pour exec(): ./exam
```

**RES:** Pas d'avancée, même si readelf ne rale plus, on ne peut pourtant pas débug le programme ainsi. L'hypothèse est incertaine.

---

## [12:10:21] Disas

**FACT:** Le disas imhex produit un asm peu éloquent, avec
**HYP:** Offset du début de disas à revoir.

---

## [12:16:39] Approche externe

**FACT:** Le debug classique est pour le moment indisponible.
Regardons comment le binaire interragit avec le système.

**HYP:** On peut avoir d'avantage d'indication, en regardant les syscalls.
On voit d'ailleurs qu'il y en a dans le dump, aussi fiable soit-il, effectué précédemment.

**TEST:**

```zsh
󰣇 offsec/reversing/exam   main  ✘!? ❯ strace exam                                                                      12:15
execve("/home/ryan/dev/lab/offsec/reversing/exam/exam", ["exam"], 0x7ffdd0115750 /* 92 vars */) = 0
--- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_MAPERR, si_addr=NULL} ---
+++ killed by SIGSEGV (core dumped) +++
[1]    541802 segmentation fault (core dumped)  strace exam
```

**RES:** Il serait intéressant de se greffer à ce process ? Comment l'exploiter ?

---

#### Test: execve

**Commande:**

```bash
󰣇 reversing/exam/bin   main  ✘!? ❯ gcc -g -O0 -no-pie -fno-omit-frame-pointer execwrap.c -o execwrap

󰣇 reversing/exam/bin   main  ✘!? ❯ gdb -q ./execwrap
```

**Observation:**

```asm
────────────────────────[ BACKTRACE ]────────────────────────
 ► 0   0x7ffff7ce5e0b execve+11
   1         0x401195 main+79
   2   0x7ffff7c27635 None
   3   0x7ffff7c276e9 __libc_start_main+137
   4         0x401085 _start+37
─────────────────────────────────────────────────────────────
pwndbg> c
Continuing.
process 68231 is executing new program: /home/ryan/dev/lab/offsec/reversing/exam/bin/exam
[New inferior 2]
warning: "/home/ryan/dev/lab/offsec/reversing/exam/bin/exam": not in executable format: file format not recognized
warning: `/home/ryan/dev/lab/offsec/reversing/exam/bin/exam': can't read symbols: file format not recognized.

Thread 2.1 "exam" received signal SIGSEGV, Segmentation fault.
[Switching to process 68231]
0x00000aeb04eaf075 in ?? ()
=> 0x00000aeb04eaf075:	ac                 	lods   al,BYTE PTR ds:[rsi]
```

```gdb vmmap
 0xaeb04eb1000      0xaeb05545000 rwxp   694000      0 [anon_aeb04eb1]
```

**Conclusion:** L'environnement ne prête pas à la réussite.

- rsi = 0 et on charge [0] dans al ; d'ou le `Program received signal SIGSEGV, Segmentation fault.`
- Malgré tout on constate un zone mémoire importante `[anon_aeb04eb1]` **Hyp:** peut être dédiée au chargement du reste du code qui serait calculée dynamiquement ?

## [14:05] Tracage

**FACT:** Le binaire crash d'emblée,
il serait intéressant de pouvoir tracer davantage
son comportement.

**HYP:** On pourrait utiliser `ptrace()`

**TEST:** à faire

**RES:**

---

## [15:33:21] Phase

**FACT:** Le programme se comporte différemment selon
les octets donnés en arguments, qui influencent directement l'état des registres.

**HYP:** On pourrait faire une boucle sur les caractères ASCII pour voir si on atteint un flot d'exécution viable.

**TEST:** test prévu

```asm
pwndbg> x/30i $rip - 0x10
   0xaeb04eaf055:	pop    rsi ; RSI=argc
   0xaeb04eaf056:	pop    rbp ; RBP=argv0
   0xaeb04eaf057:	pop    rsi ; RSI=argv1
   0xaeb04eaf058:	call   0xaeb04eaf074 ; (*)
   0xaeb04eaf05d:	add    BYTE PTR [rax],al
   0xaeb04eaf05f:	add    BYTE PTR [rdi+0x5c],cl
   0xaeb04eaf062:	imul   eax,DWORD PTR [rax],0x0
   0xaeb04eaf068:	jmp    0xaeb04eaf069
   0xaeb04eaf06a:	lock pop rsi
   0xaeb04eaf06c:	popcnt eax,ebx
   0xaeb04eaf070:	dec    al
   0xaeb04eaf072:	jmp    0xaeb04eaf06f
   0xaeb04eaf074:	pop    rax ; (*)
   0xaeb04eaf075:	lods   al,BYTE PTR ds:[rsi]

```

- **FACT** : RSP pointe maintenant sur `argv2` (ou NULL)

- **TEST** :

```gdb
pwndbg> run alpha beta
...
00:0000│ rsp 0x7fffffffd8ed ◂— 0x4e49420061746562 /* 'beta' */
```

Donc tout opcode qui fait `ret` va tenter de sauter vers `argv2`/NULL

**RES:** hypothèse de la construction dynamique cohérente, sur la base des arguments donnés,
le flot d'exécution évolue au gré de ces derniers, respectivement vecteur de jump/déplacements internes.

```asm
pwndbg> x/8i $rip
=> 0x100055:	pop    rsi
   0x100056:	pop    rbp
   0x100057:	pop    rsi
   0x100058:	call   0x100074
```

---

## [15:58:02] Clarification du mécanisme de dispatch

**FACT:** Le couple `call 0x74 / pop rax` ne sert pas à calculer une adresse par addition,
mais à récupérer une adresse de base dont l'octet de poids faible sera modifié.

**OBS:**

```asm
0x100058: call   0x100074
0x100074: pop    rax ; rax = base + 0x5d
0x100075: lodsb ; al = *rsi ; rsi++
0x100076: jmp    rax
```

**FACT:** Après `lodsb`, AL est remplacé par l'octet lu, donc **RAX voit son low-byte écrasé**.

**RES:** Le saut ne se fait pas vers `base + 0x5d + al`,
mais vers `base + (al)` dans la même page mémoire.

Chaque octet de l'argument est un opcode, interprété comme un offset de saut.

---

## [16:07:41] Nature réelle du "mot de passe"

**FACT:** Le mot de passe n'est pas comparé.
Il est **interprété**.

**HYP:** Le binaire implémente une VM minimale / un interpréteur de bytecode,
où chaque byte de l'argument correspond à un bloc de code situé à l'offset égal à sa valeur.

**OBS:**

- `\x4c` -> saut vers `0x4c` -> `jmp 0x58` -> retour au dispatcher
- `\x6c` -> `popcnt eax, ebx`
- `\x29` -> stub `exit`
- `\x08` -> stub syscall write (partiel)

**RES:** Le problème n'est pas "trouver une valeur correcte",
mais **composer une séquence valide de transitions**.

---

## [16:18:55] Contrainte implicite : fin de chaîne C

**FACT:** Les arguments sont des chaînes C.

**FACT:** `lodsb` lit **inévitablement** un `0x00` terminal après le dernier byte fourni.

**OBS:**

```asm
RSI -> argv1 bytes
...
lodsb  ; lit le dernier byte
lodsb  ; lit 0x00
jmp rax ; saute à base + 0x00
```

**RES:** Il est impossible d'éviter l'exécution du byte `0x00`.

➡️ Toute solution doit **prévoir un chemin d'exécution valide à l'offset 0x00**,
ou accepter un crash contrôlé avant.

---

## [16:27:03] Effet des ret et de la pile

**FACT:** Après les trois `pop` initiaux, la pile contient :

```
argv2
NULL
...
```

**OBS:**

```gdb
00:0000│ rsp ◂— argv2
```

**FACT:** Tout opcode contenant un `ret` tentera de sauter vers `argv2` ou `NULL`.

**RES:**

- Sans contrôle explicite, `ret` mène à un crash.
- Les transitions doivent **éviter** les ret, ou **utiliser argvN comme vecteur**.

---

## [16:38:44] Usage de plusieurs arguments

**FACT:** L'énoncé autorise plusieurs arguments selon la taille du binaire.

**OBS:** Certains opcodes (`pop rsi`) consomment la pile.

**HYP:** Les arguments peuvent être consommés **séquentiellement** comme sources de données
(et non uniquement comme code).

**TEST:**

```bash
./exam "$(printf '\x4c')" "$(printf 'OK\n')"
```

**RES:** Le comportement change (adresse de crash, registre RSI),
confirmant que **argv[i] influence directement le flot**.

---

## [16:52:10] Identification des blocs stables

**FACT:** Certains offsets sont structurels et fiables :

- `0x55` : initialisation (pop argc / argv0 / argv1)
- `0x58` : dispatcher
- `0x4c` : rebouclage (`jmp 0x58`)
- `0x29` : sortie (`sys_exit`)
- `0x50` : données `"OK\n"`

**RES:** Le graphe de contrôle est **cyclique**, mais fragile.
La majorité des offsets mènent à des instructions invalides ou à des effets de bord.

---

## [17:03:18] Limites actuelles

**FACT:** Le débogage du binaire original est fortement contraint :

- `ptrace` restreint (Yama)
- NX stack
- SIGILL fréquents
- Boucles infinies tuées par l'environnement

**RES:**
L'exploitation "directe" est difficilement itérable dans ce contexte.

---

## [17:11:54] Décision méthodologique

**FACT:** La compréhension structurelle est désormais suffisante
pour justifier un **outil d'observation dédié**.

**HYP:** Un loader contrôlé (mmap RWX, stack synthétique, signaux interceptés)
permettrait de :

- tracer précisément chaque itération du dispatcher,
- observer les effets mémoire,
- tester des séquences courtes sans subir les limitations système.

**RES:** Passage à une phase outillée (oracle / loader) justifié,
sans prétendre à une solution immédiate.

---

## 10. Synthèse

### Compris

- Le binaire n'effectue pas de comparaison de mot de passe classique
- Les arguments sont interprétés comme un flux d'opcodes
- Le dispatcher central repose sur call/pop/lodsb/jmp
- Le flot de contrôle dépend entièrement des octets fournis

### Partiellement compris

- Le rôle exact de certains offsets menant à des syscalls
- Les conditions nécessaires pour atteindre une exécution complète sans crash
- L'usage possible de plusieurs arguments comme segments distincts

### Non résolu

- Chemin exact permettant l'affichage de "OK\n" suivi d'un exit(0)
- Initialisation contrôlée des registres sans loader dédié
- Exploitation complète dans l'environnement contraint fourni

### Recommandations

- Utiliser un loader ou un émulateur pour instrumenter finement l'exécution
- Tracer systématiquement les transitions entre offsets
- Explorer les combinaisons multi-arguments pour construire un flot valide

---

## Annexes

### Scripts utilisés

```c
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>

static void die(const char *msg) {
  perror(msg);
  exit(1);
}

static int hexval(char c) {
  if ('0' <= c && c <= '9') return c - '0';
  if ('a' <= c && c <= 'f') return c - 'a' + 10;
  if ('A' <= c && c <= 'F') return c - 'A' + 10;
  return -1;
}

static size_t parse_hex(const char *s, uint8_t *out, size_t out_max) {
  size_t L = strlen(s);
  if (L % 2 != 0) {
    fprintf(stderr, "hex len must be even\n");
    exit(2);
  }
  size_t n = L / 2;
  if (n > out_max) {
    fprintf(stderr, "max %zu bytes\n", out_max);
    exit(2);
  }
  for (size_t i = 0; i < n; i++) {
    int hi = hexval(s[2 * i]);
    int lo = hexval(s[2 * i + 1]);
    if (hi < 0 || lo < 0) {
      fprintf(stderr, "invalid hex at pos %zu\n", 2 * i);
      exit(2);
    }
    out[i] = (uint8_t)((hi << 4) | lo);
  }
  return n;
}

static void hexdump(const uint8_t *p, size_t n) {
  for (size_t i = 0; i < n; i++) fprintf(stderr, "%02x", p[i]);
  fprintf(stderr, "\n");
}

static void segv_handler(int sig, siginfo_t *si, void *uctx_) {
  (void)sig;
  ucontext_t *uc = (ucontext_t *)uctx_;
#if defined(__x86_64__)
  greg_t *g = uc->uc_mcontext.gregs;
  fprintf(stderr, "\n[!] SIGSEGV @ %p\n", si->si_addr);
  fprintf(stderr, "RIP=%#llx RSP=%#llx RAX=%#llx RBX=%#llx RCX=%#llx RDX=%#llx\n",
          (unsigned long long)g[REG_RIP],
          (unsigned long long)g[REG_RSP],
          (unsigned long long)g[REG_RAX],
          (unsigned long long)g[REG_RBX],
          (unsigned long long)g[REG_RCX],
          (unsigned long long)g[REG_RDX]);
  fprintf(stderr, "RSI=%#llx RDI=%#llx RBP=%#llx R8=%#llx R9=%#llx R10=%#llx\n",
          (unsigned long long)g[REG_RSI],
          (unsigned long long)g[REG_RDI],
          (unsigned long long)g[REG_RBP],
          (unsigned long long)g[REG_R8],
          (unsigned long long)g[REG_R9],
          (unsigned long long)g[REG_R10]);
  fprintf(stderr, "R11=%#llx R12=%#llx R13=%#llx R14=%#llx R15=%#llx EFL=%#llx\n",
          (unsigned long long)g[REG_R11],
          (unsigned long long)g[REG_R12],
          (unsigned long long)g[REG_R13],
          (unsigned long long)g[REG_R14],
          (unsigned long long)g[REG_R15],
          (unsigned long long)g[REG_EFL]);
#endif
  _exit(139);
}

static void install_segv_oracle(void) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_sigaction = segv_handler;
  sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIGSEGV, &sa, NULL) < 0) die("sigaction");
}

__attribute__((noreturn, naked))
static void jump_with_rsp(void *new_rsp, void *entry) {
  __asm__ volatile(
    "mov %rdi, %rsp \n"
    "jmp *%rsi      \n"
  );
}

int main(int argc, char **argv) {
  install_segv_oracle();

  if (argc < 2) {
    fprintf(stderr, "usage: %s <hexbytes>\n", argv[0]);
    fprintf(stderr, "example: %s 4c0a4c29\n", argv[0]);
    return 2;
  }

  // ---- input bytes (<= 16) ----
  uint8_t *in = (uint8_t *)calloc(17, 1); // +1 for trailing 0x00
  if (!in) die("calloc");
  size_t in_len = parse_hex(argv[1], in, 16);
  // ensure 0-terminated (already due to calloc)

  fprintf(stderr, "IN = ");
  hexdump(in, in_len);

  // ---- load blob ./exam (128 bytes) ----
  int fd = open("./exam", O_RDONLY);
  if (fd < 0) die("open ./exam");

  uint8_t code[128];
  ssize_t r = read(fd, code, sizeof(code));
  if (r != (ssize_t)sizeof(code)) {
    fprintf(stderr, "read ./exam: got %zd bytes, expected 128\n", r);
    return 1;
  }
  close(fd);

  // ---- map at fixed address ----
  const uintptr_t BASE = 0x100000;
  void *buf = mmap((void *)BASE, 0x1000,
                   PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
  if (buf == MAP_FAILED) die("mmap(MAP_FIXED 0x100000)");

  memcpy(buf, code, sizeof(code));

  // ---- fake stack as expected by stub: pop rsi ; pop rbp ; pop rsi ----
  static char fake_argv0[] = "oracle";
  // Layout: [0]=argc (junk), [1]=argv0 ptr, [2]=argv1 ptr, [3]=argv2 (NULL)
  // stub will do: pop rsi (argc), pop rbp (argv0), pop rsi (argv1=password)
  uint64_t *fake_stack = (uint64_t *)aligned_alloc(16, 0x100);
  if (!fake_stack) die("aligned_alloc");
  memset(fake_stack, 0, 0x100);
  fake_stack[0] = 2;                         // argc
  fake_stack[1] = (uint64_t)(uintptr_t)fake_argv0; // argv[0]
  fake_stack[2] = (uint64_t)(uintptr_t)in;         // argv[1] = password bytes
  fake_stack[3] = 0;                         // argv[2] = NULL

  // ---- jump into blob at base+0x55 ----
  void *entry = (void *)(BASE + 0x55);
  jump_with_rsp(fake_stack, entry);
  return 0;
}

```

---
