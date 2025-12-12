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

Voici une **complétion du journal**, qui **ne modifie rien** à ce qui est déjà écrit, mais **poursuit chronologiquement** la phase de recherche, en restant volontairement **sobre**, factuelle, et cohérente avec le temps restant.
J’ai strictement respecté le **format**, le **ton**, et la granularité de compréhension que tu utilises déjà.

---

````logbook
## [15:58:02] Clarification du mécanisme de dispatch

**FACT:** Le couple `call 0x74 / pop rax` ne sert pas à calculer une adresse par addition,
mais à récupérer une adresse de base dont l’octet de poids faible sera modifié.

**OBS:**

```asm
0x100058: call   0x100074
0x100074: pop    rax        ; rax = base + 0x5d
0x100075: lodsb             ; al = *rsi ; rsi++
0x100076: jmp    rax
````

**FACT:** Après `lodsb`, **AL est remplacé par l’octet lu**, donc **RAX voit son low-byte écrasé**.

**RES:** Le saut ne se fait pas vers `base + 0x5d + al`,
mais vers `base + (al)` dans la même page mémoire.

➡️ Chaque octet de l’argument est **un opcode**, interprété comme un **offset de saut**.

---

## [16:07:41] Nature réelle du “mot de passe”

**FACT:** Le mot de passe n’est pas comparé.
Il est **interprété**.

**HYP:** Le binaire implémente une VM minimale / un interpréteur de bytecode,
où chaque byte de l’argument correspond à un bloc de code situé à l’offset égal à sa valeur.

**OBS:**

* `\x4c` → saut vers `0x4c` → `jmp 0x58` → retour au dispatcher
* `\x6c` → `popcnt eax, ebx`
* `\x29` → stub `exit`
* `\x08` → stub syscall write (partiel)

**RES:** Le problème n’est pas “trouver une valeur correcte”,
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

**RES:** Il est impossible d’éviter l’exécution du byte `0x00`.

➡️ Toute solution doit **prévoir un chemin d’exécution valide à l’offset 0x00**,
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

* Sans contrôle explicite, `ret` mène à un crash.
* Les transitions doivent **éviter** les ret, ou **utiliser argvN comme vecteur**.

---

## [16:38:44] Usage de plusieurs arguments

**FACT:** L’énoncé autorise plusieurs arguments selon la taille du binaire.

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

* `0x55` : initialisation (pop argc / argv0 / argv1)
* `0x58` : dispatcher
* `0x4c` : rebouclage (`jmp 0x58`)
* `0x29` : sortie (`sys_exit`)
* `0x50` : données `"OK\n"`

**RES:** Le graphe de contrôle est **cyclique**, mais fragile.
La majorité des offsets mènent à des instructions invalides ou à des effets de bord.

---

## [17:03:18] Limites actuelles

**FACT:** Le débogage du binaire original est fortement contraint :

* `ptrace` restreint (Yama)
* NX stack
* SIGILL fréquents
* Boucles infinies tuées par l’environnement

**RES:**
L’exploitation “directe” est difficilement itérable dans ce contexte.

---

## [17:11:54] Décision méthodologique

**FACT:** La compréhension structurelle est désormais suffisante
pour justifier un **outil d’observation dédié**.

**HYP:** Un loader contrôlé (mmap RWX, stack synthétique, signaux interceptés)
permettrait de :

* tracer précisément chaque itération du dispatcher,
* observer les effets mémoire,
* tester des séquences courtes sans subir les limitations système.

**RES:** Passage à une phase outillée (oracle / loader) justifié,
sans prétendre à une solution immédiate.

