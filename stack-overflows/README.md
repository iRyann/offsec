# Stack Overflows

## Approche / Approach
- **FR :** Cette catégorie illustre la progression classique : identifier un débordement, cartographier la pile, obtenir une fuite, puis chaîner des gadgets (`ret2libc`, `ret2plt`, pivots `.bss`).
- **EN:** The labs walk through the canonical flow: detect overflow, map the stack, leak memory, and chain gadgets (`ret2libc`, `ret2plt`, `.bss` pivots).

## Challenges & Difficulté
| Challenge | Description | Difficulté | Badges |
| --- | --- | --- | --- |
| [`stack-overflows-id-check`](./stack-overflows-id-check) | **FR :** Débordement simple sur vérification d'identifiant.<br>**EN:** Straightforward overflow on an ID check routine. | ★★☆☆☆ | ![Level: Débutant](https://img.shields.io/badge/Niveau-D%C3%A9butant-lightgrey?style=flat-square) ![Tech: Stack Smashing](https://img.shields.io/badge/Tech-Stack%20Smash-blue?style=flat-square) |
| [`stack-overflows-ret2func`](./stack-overflows-ret2func) | **FR :** Rediriger l'exécution vers une fonction cachée.<br>**EN:** Redirect control flow into a hidden win function. | ★★☆☆☆ | ![Level: Intermédiaire](https://img.shields.io/badge/Niveau-Interm%C3%A9diaire-blue?style=flat-square) ![Tech: ret2func](https://img.shields.io/badge/Technique-ret2func-success?style=flat-square) |
| [`stack-overflows-ret2main`](./stack-overflows-ret2main) | **FR :** ret2libc avec fuite dynamique et double payload.<br>**EN:** Two-stage ret2libc leveraging a live leak. | ★★★★☆ | ![Level: Avancé](https://img.shields.io/badge/Niveau-Avanc%C3%A9-red?style=flat-square) ![Tech: ret2libc](https://img.shields.io/badge/Tech-ret2libc-critical?style=flat-square) |
| [`stack-overflows-ret2plt`](./stack-overflows-ret2plt) | **FR :** Contrôle fin des entrées GOT via ret2plt.<br>**EN:** Fine-grained GOT hijack using ret2plt. | ★★★☆☆ | ![Level: Confirmé](https://img.shields.io/badge/Niveau-Confirm%C3%A9-orange?style=flat-square) ![Tech: ret2plt](https://img.shields.io/badge/Tech-ret2plt-ff4500?style=flat-square) |
| [`stack-overflows-ret2plt-advanced`](./stack-overflows-ret2plt-advanced) | **FR :** Pivot ROP multi-phase avec écriture sur `.bss`.<br>**EN:** Multi-phase ROP pivot chaining `.bss` stacks. | ★★★★★ | ![Level: Expert](https://img.shields.io/badge/Niveau-Expert-purple?style=flat-square) ![Focus: Stack Pivot](https://img.shields.io/badge/Focus-Stack%20Pivot-orange?style=flat-square) |

## Ressources externes / External Resources
- [Phrack 59-9 — Smashing The Stack For Fun And Profit](http://phrack.org/issues/49/14.html)
- [ROP Emporium — Call Me / Ret2libc progression](https://ropemporium.com/)
- [OWASP Cheat Sheet Series — Buffer Overflow Prevention](https://cheatsheetseries.owasp.org/cheatsheets/Buffer_Overflow_Prevention_Cheat_Sheet.html)
