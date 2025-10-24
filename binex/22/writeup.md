On exploite ici la _CWE-190: Integer Overflow or Wraparound_.
En particulier, lorsque la valeur que l'on souhaite affecter à la variable dépasse la capacité maximale, seuls les N premiers octets sont conservés. On doit donc avoir $3x=x00x00x00x02$.
4294967298 fonctionne.
