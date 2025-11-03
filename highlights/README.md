# Highlights

## Stack Overflow "ret2libc"
- **FR :** Chaîne ROP vers `system` avec fuite dynamique de libc et calcul d'offsets.
- **EN:** ROP chain to `system` with dynamic libc leak and offset resolution.
- **Techniques / Techniques :** ret2libc, fuite GOT, ret2plt, bypass `NX`.

## Heap "Fastbin Frenzy"
- **FR :** Exploitation d'une double libération pour réécrire un pointeur de fonction et obtenir un shell.
- **EN:** Double-free abuse to overwrite a function pointer and pop a shell.
- **Techniques / Techniques :** fastbin dup, consolidation, forges de chunk.

## Reversing "Obfuscation Lab"
- **FR :** Désobfuscation d'un crackme en VM custom, reconstruction de bytecode et patch du binaire.
- **EN:** Reverse a custom VM crackme, rebuild bytecode semantics, and patch the binary.
- **Techniques / Techniques :** rizin, scripts Python, analyse de flux de contrôle.

## Automation "Oracle Attack"
- **FR :** Pipeline d'automatisation pwntools pour casser un schéma de signature à oracle.
- **EN:** Pwntools automation pipeline to break an oracle-based signing scheme.
- **Techniques / Techniques :** pwntools, crypto modulaire, fuzzing ciblé.
