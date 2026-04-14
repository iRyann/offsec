**2. Mesurer `DELTA` pour `rbp_main` → `cat_rbp` :**

```
(gdb) finish
(gdb) p $rbp          # dans main → rbp_main
```

DELTA = rbp_main - cat_rbp
