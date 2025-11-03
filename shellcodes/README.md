# Shellcodes

## Approche
- Construire, encoder et injecter des shellcodes adaptés aux contraintes (bad chars, canari, NX).

## Challenges & Difficulté

| Challenge                                                    | Description                                              | Difficulté |
| ------------------------------------------------------------ | -------------------------------------------------------- | ---------- |
| [`shellcodes-ret2sc-tutorial`](./shellcodes-ret2sc-tutorial) | Retour direct vers un shellcode stocké.                  | ★★☆☆☆      |
| [`shellcodes-ret2sc-no-null`](./shellcodes-ret2sc-no-null)   | Shellcode sans octet nul avec encodage custom.           | ★★★☆☆      |
| [`shellcodes-ret2sc-canary`](./shellcodes-ret2sc-canary)     | Fuite du canari avant injection polymorphique.           | ★★★★☆      |
| [`shellcodes-burger`](./shellcodes-burger)                   | Shellcode personnalisé multi-syscalls façon menu burger. | ★★★☆☆      |

## Ressources externes

* [Shell-Storm — Shellcode Database](http://shell-storm.org/shellcode/)
* [Modern Binary Exploitation — Shellcoding](https://github.com/RPISEC/MBE/tree/master/module3)
* [The Legend of Random — Writing Linux Shellcode](http://www.linuxnasm.be/shellcode/index.html)
