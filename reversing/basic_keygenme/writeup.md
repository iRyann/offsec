# Write-up — x64 NASM Keygen-Me (Root-Me)

## 1. Analyse statique

Le binaire est un ELF x86-64 écrit en NASM.
Le flot principal est le suivant :

1. Affichage d’un banner
2. Lecture du **login** depuis `stdin` (`read(0, login, 0x20)`)
3. Ouverture du fichier `.m.key`
   - s’il est absent → échec immédiat
4. Lecture de `.m.key` dans un buffer
5. Appel à une fonction de vérification `FUN_00400146(login, key)`
6. En cas de succès → message _“Yeah, good job bro”_

---

## 2. Fonction de vérification

La fonction critique est :

```c
for (i = 0; i != strlen(login) - 1; i++) {
    if ((login[i] - i + 0x14) != key[i])
        return 0x1337;
}
return 0;
```

La condition de succès est donc :

```
key[i] = login[i] - i + 0x14   (mod 256)
```

La boucle compare uniquement les indices `0 .. len(login)-2`.

---

## 3. Hypothèse du challenge

Le login est imposé par l’énoncé :

```
login = "root-me.org"
```

Le fichier `.m.key` n’est **pas fourni**, mais il est possible de le **déduire** à partir du login, puisque la relation est linéaire et inversible.

Ainsi, `.m.key` est en réalité le **serial attendu**.

---

## 4. Construction de `.m.key`

En prenant en compte le comportement exact du programme (lecture brute avec `read`, comparaison sur `len(login)-1` octets), on inclut le `\n` envoyé par l’utilisateur mais on ne génère la clé que pour les octets comparés.

Script Python :

```python
import hashlib

login = b"root-me.org\n"
n = len(login) - 1  # la boucle va jusqu’à len-1 exclu

key = bytes(((login[i] - i + 0x14) & 0xff) for i in range(n))

with open(".m.key", "wb") as f:
    f.write(key)

flag = hashlib.sha256(key).hexdigest()
print(flag)
```

---

## 5. Flag

Le **flag** est défini par l’énoncé comme :

```
SHA256(serial)
```

Ici, le _serial_ correspond au contenu de `.m.key`.

Le script précédent calcule directement le flag correct.

---

## 6. Conclusion

- Le challenge repose sur une **relation arithmétique simple** entre le login et le serial.
- L’absence de `.m.key` n’est pas un obstacle : le fichier est **entièrement déductible**.
- Le piège principal est l’écriture **binaire** (octets non ASCII) et la gestion du `\n` lu par `read`.

Challenge résolu par inversion directe de la condition de vérification.
