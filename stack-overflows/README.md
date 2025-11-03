# Stack Overflows

## Approche
- Cette catégorie illustre la progression classique : identifier un débordement, cartographier la pile, obtenir une fuite, puis chaîner des gadgets (`ret2libc`, `ret2plt`, pivots `.bss`).

## Challenges & Difficulté

| Challenge                                                                | Description                                        | Difficulté |
| ------------------------------------------------------------------------ | -------------------------------------------------- | ---------- |
| [`stack-overflows-id-check`](./stack-overflows-id-check)                 | Débordement simple sur vérification d'identifiant. | ★★☆☆☆      |
| [`stack-overflows-ret2func`](./stack-overflows-ret2func)                 | Rediriger l'exécution vers une fonction cachée.    | ★★☆☆☆      |
| [`stack-overflows-ret2main`](./stack-overflows-ret2main)                 | ret2libc avec fuite dynamique et double payload.   | ★★★★☆      |
| [`stack-overflows-ret2plt`](./stack-overflows-ret2plt)                   | Contrôle fin des entrées GOT via ret2plt.          | ★★★☆☆      |
| [`stack-overflows-ret2plt-advanced`](./stack-overflows-ret2plt-advanced) | Pivot ROP multi-phase avec écriture sur `.bss`.    | ★★★★★      |

## Ressources externes

* [Phrack 59-9 — Smashing The Stack For Fun And Profit](http://phrack.org/issues/49/14.html)
* [ROP Emporium — Call Me / Ret2libc progression](https://ropemporium.com/)
* [OWASP Cheat Sheet Series — Buffer Overflow Prevention](https://cheatsheetseries.owasp.org/cheatsheets/Buffer_Overflow_Prevention_Cheat_Sheet.html)
