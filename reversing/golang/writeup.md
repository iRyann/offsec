# **[Write-up] ReverseGo — XOR layer + Go slice reconstruction**

## 1. Introduction

Le binaire étudié est un ELF Go 1.x statique.
Comme d'habitude, le reversing est obscurci par :

- le runtime Go,
- les slices (triplets data/len/cap),
- une quantité massive de glue code.

L'objectif est d'identifier la logique de validation et de reconstruire l'entrée correcte.

---

## 2. Localisation de la comparaison

Dans `main.main`, après lecture de l'entrée, on trouve un appel à :

```
bytes.Compare(a, b)
```

`bytes.Compare` étant un `memcmp` optimisé, la vérification repose donc sur la construction de ses deux arguments :

- `a` : slice constant `main.statictmp_2`,
- `b` : résultat d'une transformation appliquée à l'entrée utilisateur.

---

## 3. Analyse de la transformation

Avant l'appel à `bytes.Compare`, une boucle prépare le slice `b` :

```asm
MOVZX R10D, byte ptr [RBX]; c = src[i]
IDIV RDI; i % key_len
MOVZX EDX, [R8 + RDX]; key = key[i % len]
XOR R10D, EDX; c ^= key
MOV [R12 + R9], R10B; dst[i] = c
```

Les registres impliqués :

- `RBX` = pointeur sur le buffer source,
- `R8` = pointeur sur la clé (issue d'un `stringtoslicebyte`),
- `RDI` = longueur de cette clé (= 6).

En clair :

```go
for i := range src {
    dst[i] = src[i] ^ key[i % len(key)]
}
```

La clé provient de l'adresse `DAT_004c446d`, qui contient :

```
72 6f 6f 74 6d 65 -> "rootme"
```

---

## 4. Extraction de la constante attendue

`main.statictmp_2` contient :

```
3b 02 23 1b 1b 0c 1c 08 28 1b 21 04 1c 0b
```

avec une longueur de 14.

C'est ce buffer XOR-transformé que le programme attend après la boucle.

---

## 5. Récupération de l'entrée correcte

La boucle effectue :

```
dst = src XOR key -> src = dst XOR key
```

Donc :

```python
expected = [0x3b,0x02,0x23,0x1b,0x1b,0x0c,0x1c,0x08,0x28,0x1b,0x21,0x04,0x1c,0x0b]
key      = [0x72,0x6f,0x6f,0x74,0x6d,0x65] # "rootme"
```

Application du XOR cyclique :

```
ImLovingGoLand
```

---

## 6. Conclusion

Le programme applique un XOR sur l'entrée avec la clé `"rootme"` puis compare le résultat à une constante de 14 octets.
L'inversion de la transformation suffit à retrouver l'entrée valide.
