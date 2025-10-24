b après le call, pour observer l'@ du buffer, trouver l'@ de retour et l'override par @ login.

grep "COUCOU" pour trouver l'@ buffer

info func pour @ login

rsp pointe sur le sommet de la stack, on vient d'ajouter un nouvel élément, d'ou

Une ligne 16, on voit rapidement l'offset de rsp à l'endroit qui nous interesse.

on rentre l'add : il faut la noter en little endian : 401156 devient 56 11 40.
En interpollant, 56 : V, 11 : <C-q>, 40 : @

movdqa : effecte à un registre de 128 bits, mais attend que la pile soit bien alignée.

Or, ici, le push desalligne. Ansi, on se dit qu'on va simplement jump après le push

-> 57 : W
