# Miner du COINWOW

English: [mining.md](mining.md)

Les mineurs ajoutent de nouveaux blocs à la chaîne COINWOW. Chaque fois que vous
trouvez un bloc, vous gagnez 25 CW. Ce guide vous explique comment miner par
vous-même, avec votre propre nœud et votre propre adresse de paiement. Personne
d'autre ne détient jamais vos pièces.

> **Remarque :** il n'existe pas encore de pool communautaire public. Un pool
> garde et verse les pièces d'autres personnes : il faut donc des tests
> rigoureux et une vérification légale au préalable.

## COINWOW en bref

| | |
|---|---|
| Algorithme | SHA-256d (le même que Bitcoin) |
| Durée d'un bloc | 60 secondes |
| Récompense par bloc | 25 CW, divisée par deux tous les 200 000 blocs |
| Offre maximale | 10 000 000 CW |
| Ajustement de la difficulté | Tous les 2016 blocs |
| Pièces minées dépensables après | 100 blocs (environ 100 minutes) |
| Adresse de paiement | Commence par `cw1` |

## Ce qu'il vous faut

- **Un mineur ASIC SHA-256.** Par exemple, un Antminer S9.
- **Un ordinateur qui exécute votre propre nœud COINWOW**, entièrement
  synchronisé.
- **Un serveur Stratum.** Les mineurs ASIC parlent Stratum, pas le RPC du nœud.
  Vous en installez un à l'étape 4.
- **Une connexion réseau filaire** jusqu'à votre mineur.

## Puis-je miner avec mon PC ?

En théorie, oui. En pratique, non.

- Un processeur d'ordinateur atteint des dizaines de millions de hachages par
  seconde.
- Une carte graphique en atteint environ un milliard.
- Un ASIC comme l'Antminer S9 en atteint plusieurs mille milliards.

Un PC est donc des milliers de fois (ou plus) trop lent pour trouver des blocs
à la difficulté actuelle. De plus, COINWOW Core n'a pas de mineur intégré, et le
projet n'a testé aucun logiciel de minage pour PC.

La difficulté s'ajuste tous les 2016 blocs : cela peut donc changer si des
mineurs ASIC quittent le réseau.

## Combien de temps faut-il pour trouver un bloc ?

Si vous étiez le seul mineur :

```
secondes par bloc = difficulté × 4 294 967 296 ÷ hachages par seconde
```

Par exemple, à une difficulté de 68 812 et 1 TH/s (mille milliards de hachages
par seconde), cela donne environ 295 secondes.

La difficulté change avec le temps. Pour voir la valeur actuelle, exécutez :

```
coinwow-cli getmininginfo
```

> **Important :** le minage ne garantit jamais un revenu, et le CW n'a aucune
> valeur garantie.

## Configurer le minage

### Étape 1. Synchronisez votre nœud

Ouvrez `coinwow-qt`, ou démarrez `coinwowd`. Exécutez ensuite :

```
coinwow-cli getblockchaininfo
```

Attendez de voir `"initialblockdownload": false`.

> **Remarque :** la chaîne n'avance que lorsque quelqu'un mine. Si personne ne
> mine, le réseau reste au dernier bloc, et votre portefeuille peut afficher un
> pourcentage de synchronisation qui n'atteint jamais 100 %. C'est normal.

### Étape 2. Obtenez une adresse de paiement

Exécutez :

```
coinwow-cli getnewaddress "mining" bech32
```

L'adresse commence par `cw1`. Sauvegardez votre portefeuille **avant** de
commencer à miner. Consultez [managing-wallets.md](../managing-wallets.md).

> **Important :** ne partagez jamais votre fichier de portefeuille, vos données
> de récupération ni vos identifiants RPC, même avec une personne qui dit
> travailler pour COINWOW.

### Étape 3. Autorisez votre nœud à parler à un serveur Stratum

Ajoutez ces lignes à `coinwow.conf`
(voir [coinwow-conf.md](../coinwow-conf.md)) :

```
server=1
rpcbind=127.0.0.1
rpcallowip=127.0.0.1
```

Redémarrez ensuite votre nœud. Utilisez des identifiants hachés (`rpcauth`,
voir `share/rpcauth/README.md`) ou le fichier cookie. Évitez `rpcuser` et
`rpcpassword` en clair.

> **Important :** n'ouvrez jamais le port RPC (51446) sur Internet.

### Étape 4. Installez un serveur Stratum

Installez un serveur Stratum qui construit le travail à partir du RPC
`getblocktemplate` de votre nœud. Ensuite :

1. Dirigez-le vers `127.0.0.1:51446`.
2. Entrez votre adresse `cw1…` comme adresse de paiement.
3. Rendez le port Stratum accessible à votre mineur, idéalement seulement sur
   votre réseau domestique.

L'installation de test du projet COINWOW utilise
[NOMP](https://github.com/zone117x/node-open-mining-portal) avec un
Antminer S9. D'autres serveurs Stratum compatibles avec `getblocktemplate` de
Bitcoin Core devraient aussi fonctionner, mais le projet ne les a pas testés.

### Étape 5. Connectez votre mineur

Ouvrez la page web de votre mineur. Dans les réglages du pool, entrez :

```
stratum+tcp://<adresse de votre serveur Stratum>:<port>
```

Le nom du worker et le mot de passe dépendent de votre logiciel Stratum. Bien
des logiciels ignorent le mot de passe.

### Étape 6. Vérifiez que ça fonctionne

- La page de votre mineur affiche des shares **acceptées**.
- `coinwow-cli getmininginfo` affiche la difficulté et la hauteur de bloc.
- Quand vous trouvez un bloc, 25 CW apparaissent dans votre portefeuille comme
  **immatures**. Ils deviennent dépensables après 100 blocs de plus.

## Restez en sécurité

- **Électricité.** Un Antminer S9 consomme environ 1,3 kW. Utilisez un circuit
  qui peut supporter la charge, et évitez toute multiprise non prévue pour cette
  puissance.
- **Chaleur et bruit.** Un ASIC chauffe beaucoup et fait du bruit. Aérez la
  pièce, éloignez-le de tout ce qui peut brûler et ne le laissez jamais sans
  surveillance.
- **Moins de 18 ans ?** Demandez à un adulte de superviser le matériel.
- **Arnaques.** Personne de COINWOW ne vous demandera de taper des commandes, de
  partager votre portefeuille ou d'envoyer des pièces pour en recevoir.
- **Téléchargements.** COINWOW Core v1.0.0 n'est pas encore signé, donc Windows
  et macOS affichent des avertissements. Vérifiez votre téléchargement avec le
  fichier `SHA256SUMS` publié.

## Pour les développeurs

Si vous écrivez ou exploitez un logiciel de minage :

- Utilisez le `version` renvoyé par `getblocktemplate`.
- Le `scriptSig` de la transaction coinbase doit commencer par la hauteur du
  bloc sérialisée (BIP34). Le BIP34 commence à être appliqué à la hauteur
  227 931, que la chaîne devrait atteindre en 2027.
  Consultez [consensus-audit.md](consensus-audit.md).
