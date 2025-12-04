# Racine – Steganography/RE Writeup

## Contexte

Le challenge « Racine » fournit une archive `racine.tar.xz` contenant un immense arbre de dossiers, chacun comportant un binaire nommé `executable`. Le flag est annoncé au format `pyr_ctf{FLAG}`.

## Démarche

1. **Extraction et reconnaissance rapide** : après avoir extrait l’archive, j’ai lancé un `file`/`objdump` sur un binaire quelconque. Tous sont de minuscules ELF 64 bits réalisant la même routine : ils copient un bloc d’octets constants sur la pile puis appellent une fonction de déchiffrement XOR.
2. **Reverse engineering** : le désassemblage met en évidence un appel `xor_decrypt(buffer, 0x28, 0x55)` ; la boucle XORe chaque octet du bloc par la clé `0x55` avant d’afficher le résultat avec `puts`. En reconstruisant les octets immédiats (ou en exécutant le binaire sous Python pour extraire le bloc), on obtient la chaîne chiffrée.
3. **Déchiffrement automatisé** : un petit script Python lit ces constantes (ou exécute chaque binaire) et applique le XOR `0x55` pour récupérer les messages. La plupart des exécutables renvoient des chaînes brouillées, mais un sous-ensemble affiche `ctf_pyr{____@@uuttvvvsrzcmmmmn000333$'!}` une fois décodé.
4. **Normalisation du flag** : le préfixe inversé `ctf_pyr` indique simplement qu’il faut remettre l’ordre standard `pyr_ctf`. En lisant le reste en leet-speak (« mais comment vous m’avez trouvé »), on recompose le flag complet `pyr_ctf{m@!s_c0mm3nt_v0u$_m'@v3z_tr0uv3}`.

## Flag

`pyr_ctf{m@!s_c0mm3nt_v0u$_m'@v3z_tr0uv3}`
