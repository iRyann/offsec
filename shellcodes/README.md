# Shellcodes

## Approche / Approach
- **FR :** Construire, encoder et injecter des shellcodes adaptés aux contraintes (bad chars, canari, NX).
- **EN:** Craft, encode, and deliver shellcodes tailored to constraints (bad chars, canary, NX).

## Challenges & Difficulté
| Challenge | Description | Difficulté | Badges |
| --- | --- | --- | --- |
| [`shellcodes-ret2sc-tutorial`](./shellcodes-ret2sc-tutorial) | **FR :** Retour direct vers un shellcode stocké.<br>**EN:** Direct ret2shellcode primer. | ★★☆☆☆ | ![Level: Débutant](https://img.shields.io/badge/Niveau-D%C3%A9butant-lightgrey?style=flat-square) ![Tech: Shellcode](https://img.shields.io/badge/Tech-Shellcode-blue?style=flat-square) |
| [`shellcodes-ret2sc-no-null`](./shellcodes-ret2sc-no-null) | **FR :** Shellcode sans octet nul avec encodage custom.<br>**EN:** Null-free shellcode with custom encoder. | ★★★☆☆ | ![Level: Confirmé](https://img.shields.io/badge/Niveau-Confirm%C3%A9-orange?style=flat-square) ![Focus: Badchars](https://img.shields.io/badge/Focus-Badchars-brown?style=flat-square) |
| [`shellcodes-ret2sc-canary`](./shellcodes-ret2sc-canary) | **FR :** Fuite du canari avant injection polymorphique.<br>**EN:** Canary leak before polymorphic injection. | ★★★★☆ | ![Level: Avancé](https://img.shields.io/badge/Niveau-Avanc%C3%A9-red?style=flat-square) ![Focus: Canary Bypass](https://img.shields.io/badge/Focus-Canary%20Bypass-brown?style=flat-square) |
| [`shellcodes-burger`](./shellcodes-burger) | **FR :** Shellcode personnalisé multi-syscalls façon menu burger.<br>**EN:** Multi-syscall custom shellcode "burger" challenge. | ★★★☆☆ | ![Level: Confirmé](https://img.shields.io/badge/Niveau-Confirm%C3%A9-orange?style=flat-square) ![Focus: Syscall](https://img.shields.io/badge/Focus-Syscall-grey?style=flat-square) |

## Ressources externes / External Resources
- [Shell-Storm — Shellcode Database](http://shell-storm.org/shellcode/)
- [Modern Binary Exploitation — Shellcoding](https://github.com/RPISEC/MBE/tree/master/module3)
- [The Legend of Random — Writing Linux Shellcode](http://www.linuxnasm.be/shellcode/index.html)
