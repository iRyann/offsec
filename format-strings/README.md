# Format Strings

## Approche / Approach
- **FR :** Objectif : transformer un simple `%x` en fuite contrôlée, puis en écriture ciblée via `%n`, tout en gérant l'alignement et la largeur. Les labs montent en puissance jusqu'aux multi-écritures coordonnées.
- **EN:** Goal: escalate from `%x` info leaks to `%n`-driven arbitrary writes while juggling alignment and field width. Labs culminate in coordinated multi-write payloads.

## Challenges & Difficulté
| Challenge | Description | Difficulté | Badges |
| --- | --- | --- | --- |
| [`format-strings-leak-smallbuf`](./format-strings-leak-smallbuf) | **FR :** Première fuite sur tampon limité.<br>**EN:** First leak with a constrained buffer. | ★★☆☆☆ | ![Level: Débutant](https://img.shields.io/badge/Niveau-D%C3%A9butant-lightgrey?style=flat-square) ![Focus: Leak](https://img.shields.io/badge/Focus-Leak-orange?style=flat-square) |
| [`format-strings-leak`](./format-strings-leak) | **FR :** Enumération des adresses stack/GOT par `%p`.<br>**EN:** Enumerate stack/GOT addresses via `%p`. | ★★☆☆☆ | ![Level: Intermédiaire](https://img.shields.io/badge/Niveau-Interm%C3%A9diaire-blue?style=flat-square) ![Tech: Format String](https://img.shields.io/badge/Tech-Format%20String-ff69b4?style=flat-square) |
| [`format-strings-arbitrary-write`](./format-strings-arbitrary-write) | **FR :** Première écriture `%n` en ciblant un pointeur de fonction.<br>**EN:** Initial `%n` write to hijack a function pointer. | ★★★☆☆ | ![Level: Confirmé](https://img.shields.io/badge/Niveau-Confirm%C3%A9-orange?style=flat-square) ![Focus: Arbitrary Write](https://img.shields.io/badge/Focus-Arbitrary%20Write-green?style=flat-square) |
| [`format-strings-extended-write`](./format-strings-extended-write) | **FR :** Payloads avec largeurs dynamiques et padding précis.<br>**EN:** Payloads with dynamic widths and tight padding. | ★★★★☆ | ![Level: Avancé](https://img.shields.io/badge/Niveau-Avanc%C3%A9-red?style=flat-square) ![Focus: Width Control](https://img.shields.io/badge/Focus-Width%20Control-teal?style=flat-square) |
| [`format-strings-multiwrite`](./format-strings-multiwrite) | **FR :** Ecritures partielles coordonnées pour forger une GOT complète.<br>**EN:** Coordinated partial writes to forge a full GOT entry. | ★★★★★ | ![Level: Expert](https://img.shields.io/badge/Niveau-Expert-purple?style=flat-square) ![Tech: Multi-write](https://img.shields.io/badge/Tech-Multiwrite-brown?style=flat-square) |

## Ressources externes / External Resources
- [Shellblade — Format String Exploitation Tutorial](https://shellblade.net/docs/format-string-exploitation/)
- [SploitFun — Format String Exploitation](https://sploitfun.wordpress.com/2015/06/26/format-string-exploitation/)
- [Nightmare — Format Strings](https://guyinatuxedo.github.io/02-jump_to_shellcode/06-format_strings/index.html)
