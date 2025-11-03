# Format Strings

## Approche
- Objectif : transformer un simple `%x` en fuite contrôlée, puis en écriture ciblée via `%n`, tout en gérant l'alignement et la largeur. Les labs montent en puissance jusqu'aux multi-écritures coordonnées.

## Challenges & Difficulté
| Challenge | Description | Difficulté |
| --- | --- | --- |
| [`format-strings-leak-smallbuf`](./format-strings-leak-smallbuf) | Première fuite sur tampon limité. | ★★☆☆☆ |
| [`format-strings-leak`](./format-strings-leak) | Enumération des adresses stack/GOT par `%p`. | ★★☆☆☆ | 
| [`format-strings-arbitrary-write`](./format-strings-arbitrary-write) | Première écriture `%n` en ciblant un pointeur de fonction. | ★★★☆☆ |
| [`format-strings-extended-write`](./format-strings-extended-write) | Payloads avec largeurs dynamiques et padding précis. | ★★★★☆ | 
| [`format-strings-multiwrite`](./format-strings-multiwrite) | Ecritures partielles coordonnées pour forger une GOT complète. | ★★★★★ |

## Ressources externes
- [Shellblade — Format String Exploitation Tutorial](https://shellblade.net/docs/format-string-exploitation/)
- [SploitFun — Format String Exploitation](https://sploitfun.wordpress.com/2015/06/26/format-string-exploitation/)
- [Nightmare — Format Strings](https://guyinatuxedo.github.io/02-jump_to_shellcode/06-format_strings/index.html)
