On veut sauter sur la fonction `login` située à l'adresse `0x0000000000401156` ; cette dernière étant déterminée avec la commande `info functions`.
On `disas` la fonction `main` afin de positionner un breakpoint à l'instruction d'appel à `gets`, afin de :

- déterminer la position du buffer en mémoire,
- observer la stack après la saisie du payload.

Manifestement, d'après le code machine, et a fortiori les arguments de gets, on constate que le buffer commence à `$rbp-0x40`. Sinon, l'adresse du buffer correspond aussi à la valeur de `$rdi` lors de l'appel à `gets`, en l'occurence `0x00007fffffffd4e0` ici.

On met un breakpoint sur `ret` afin d'observer l'adresse de retour. On constate que `rip` pointe sur `0x7ffff7c27675`. Ainsi il faut écraser la valeur située à `$rbp + 8`, en la substituant par `0x401156`.

1. On calcule l'offset : ($rbp + 8) - ($rbp - 40) = 0x48 = 72 octets
2. On injecte le payload : `aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaa 56 11 40 00 00 00 00 00` 
