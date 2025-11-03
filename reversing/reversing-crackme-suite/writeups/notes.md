# 00

mdp : mistigri

## Méthode employée

- b sur strcmp
- b main puis grep "Failed!"

# 01

```gdb
info functions
```

mdp : ecret
@ écrite en dur -> def avant d'entrer dans la fonction.
Pas besoin de bp, on regarde donc d'emblée à @.

# 02

mdp : 43ys

b strcmp puis rdi

# 03

mdp : Very/Easy

Unique cmp eax/edx : très probablement comparaison entre input et le mdp.
Sur plusieurs entrée, offset de 1
Décodage avec un décalage de 1

# 04

mdp hiddenPhrase
info functions
b sur strcmp puis valeur de al par itération pour identifier la chaine

# 05

mdp : ROBINET

On observe le calcul opéré sur `sil` ; à savoir sub dil puis shl. Ainsi, en dénotant la suite des valeurs du registre dil qui manifestement ne sont pas fonction du texte saisi en entrée, on en déduit la valeur attendu pour assurer la réussite de la comparaison à `0x80`

# 06

mdp : 0x79e, ie 1950

## Méthodologie

b sur atoi, et b sur ret afin de pouvoir step in et d'éviter la fin d'exécution du programme
on repèse ensuire l'opératio add entre nos deux argument puis la comparaison à 0x79e. On en déduit les valeurs adéquates des arguments.

# 07

b2b -> mdp: 2849

# 08

mdp : g00d3n0ugh

## Méthodologie

1. objdump nous donne l'adresse de .text, y'a tout
2. gdb et b sur .text
3. bh convitent string, on fait x/ [rdi +rcx - 0x4]
4. on itère en complétant par la valeur du char attendu, on observe d'ailleurs un pattern tous les deux chars

# 09

mdp : 9876543210
c'était très simple, comme indiqué
