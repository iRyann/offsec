# Heap Exploitation

## Approche / Approach
- **FR :** Comprendre les allocateurs glibc (`tcache`, `fastbin`) avant de manipuler les métadonnées pour détourner le flux.
- **EN:** Understand glibc allocators (`tcache`, `fastbin`) and then tamper with metadata to bend control flow.

## Challenges & Difficulté
| Challenge | Description | Difficulté | Badges |
| --- | --- | --- | --- |
| [`heap-data-overflow`](./heap-data-overflow) | **FR :** Corrompre un chunk adjacent pour écraser un pointeur fonctionnel.<br>**EN:** Overflow into a neighbour chunk to smash a function pointer. | ★★★☆☆ | ![Level: Confirmé](https://img.shields.io/badge/Niveau-Confirm%C3%A9-orange?style=flat-square) ![Focus: Fastbin](https://img.shields.io/badge/Focus-Fastbin-purple?style=flat-square) ![Tech: Heap Overflow](https://img.shields.io/badge/Tech-Heap%20Overflow-9cf?style=flat-square) |

## Ressources externes / External Resources
- [how2heap — Glibc Heap Exploitation Playground](https://github.com/shellphish/how2heap)
- [SploitFun — Understanding the Heap by Example](https://sploitfun.wordpress.com/2015/02/10/understanding-the-heap-by-example-part-1/)
- [glibc malloc Internals (Malloc Maleficarum)](http://phrack.org/issues/66/10.html)
