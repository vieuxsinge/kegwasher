# Laveuse de fûts DIY

Ce dépôt contient la partie technique pour la fabrication d'une laveuse de fûts. Vous pouvez lire [un article introductif](https://www.vieuxsinge.com/fabrication-laveuse-de-futs-open-source.html) pour avoir un peu plus de contexte.

* [Démo](#démo)
* [Plomberie](#plomberie)
* [Électronique](#électronique)
* [Programmation](#programmation)
* [Structure](#structure)
* [Composants utilisés et coût](#composants-utilisés-et-coût)
* [Améliorations](#améliorations)
* [Références](#références)
* [Questions fréquentes](#questions-fréquentes)

## Démo

Parce qu'une vidéo vaut parfois mieux qu'un long discours :

* [Laveuse en cours de fonctionnement](https://peertube.servebeer.com/videos/watch/00c410c9-e059-47cc-8bc2-f5a8decc9740)
* [Fonctionnement général](https://vimeo.com/318256040)
* [Description de la partie électronique](https://vimeo.com/318257333)


## Plomberie

Les cuves produits sont réalisées avec des marmites Bergland 50L bien connues des brasseurs amateurs. Des kits d'étanchéité <a href="http://microbrassage.com">Canard rouge</a> et des raccords cannelés permettent de réaliser très simplement des entrées et sorties pour les produits.

![Cuves produit](/images/cuves.jpg)

Le bas de chaque cuve et l'arrivée d'eau sont reliées à un distributeur de liquides réalisé à base de tés en inox : 3 entrées et 1 sortie. A chaque entrée se trouve une électrovanne qui permet de choisir si le liquide entre ou non dans le distributeur. Le distributeur de liquides est ensuite relié à l'entrée de la pompe.

![Distributeur de liquides](/images/distributeur_liquides.jpg)

Comme pour les liquides, la laveuse est équipée d'un distributeur de gaz à 2 entrées. Les électrovannes permettent d'activer ou non l'arrivée d'air ou de CO2. Le distributeur de gaz est relié à la sortie de la pompe. Les entrées gaz peuvent être équipées de détendeurs intermédiaires qui évitent de devoir régler le compresseur à chaque fois.

![Distributeur de gaz](/images/distributeur_gaz.jpg)

![Détendeur intermédiaire](/images/detendeur.jpg)

Pour éviter que le gaz ne retourne dans la pompe en sens inverse, la pompe est équipée d'un clapet antiretour à sa sortie. De la même manière, pour éviter que les liquides ne remontent dans les lignes de gaz, un clapet antiretour est installé sur l'arrivée de gaz juste avant la sortie de la pompe.
On pensait à la base s'économiser ces 2 clapets antiretour, mais croyez nous : ils sont indispensables !

![Clapets](/images/clapets.jpg)

À la sortie de la pompe, on a donc un tuyau qui contient le fluide (liquide ou gaz) à envoyer dans les fûts. Cette arrivée est simplement divisé en 2 tuyaux grâce à un té pour alimenter les 2 têtes de lavage.

![Répartiteur des têtes de lavage](/images/repartiteurs.jpg)

Les têtes de lavage ressemblent à des têtes de tirage sans antiretour et utilisées à l'envers. À la différence des têtes de tirage, elles ont une sortie plus grande pour laisser passer le liquide de nettoyage tout simplement.

![Têtes de lavage](/images/tete.jpg)

![Têtes de lavage connectée](/images/tete_branchee.jpg)

Les sorties des têtes de lavage sont reliées à un collecteur de produits. Il s'agit d'un assemblage de tés qui fonctionne sur le même principe que le distributeur, mais à l'envers : 1 entrée et 3 sorties. Sur chaque sortie se trouve une électrovanne qui permet de choisir la sortie à utiliser : retour dans les cuves produits ou évacuation au caniveau.

![Collecteur](/images/collecteur.jpg)

### Plus d'infos

#### Électrovannes

![Électrovanne](/images/electrovanne.jpg)

Les électrovannes utilisées sont en inox pour supporter le passage de produits agressifs. Nous avons choisi d'utiliser des électrovannes à solénoïde qui ont le double avantage de commuter rapidement et d'être moins chères que d'autres modèles. Nous avons aussi choisi des modèles pilotables en 12V pour éviter la présence de 230V sur la partie hydraulique.

#### Pompe

On utilise une pompe Novax 20B car on en avait une sous la main qui ne servait à rien. Pas sûr que ce soit le meilleur choix, mais elle s'avère tenir le coup et envoyer une pression suffisante.
La pompe n'a pas besoin d'être amorcée "manuellement" car le premier cycle est un cycle de rinçage ce qui amorce la pompe grâce à la pression du réseau d'eau.

#### Tuyaux

La majorité des tuyaux utilisés sont des tuyaux Thermoclean 100 de diamètre 19mm qui supportent les produits corrosifs.
Les tuyaux reliés aux têtes de lavage sont cependant en simple cristal car le thermoclean est trop rigide et empêche la manipulation des fûts.
La partie gaz est en tube cristal de diamètre 15mm.

#### Raccords

Tous les raccords sont en inox et étanchéifiés avec du teflon pour mieux supporter les produits corrosifs.

Pour percer les cuves et y fixer une vanne, [c'est ici](https://www.microbrassage.com/wp-content/uploads/2015/03/notice_kit_etancheite-.pdf).

## Électronique

La laveuse aurait pu fonctionner avec des vannes manuelles comme certains modèles commerciaux très basiques, mais pour un investissement assez faible, on peut automatiser tout ça et ainsi s'économiser beaucoup de temps de travail pas très valorisant.

Le cerveau de l'affaire c'est l'arduino : une carte électronique qui est programmable. C'est directement sur cette carte que sont reliés les boutons, l'écran LCD et le buzzer.

![Arduino](/images/arduino.jpg)

Par contre impossible de piloter nos électrovannes directement depuis l'arduino car elles consomment trop de courant. Il faut, pour ça, utiliser des relais, autrement dit des interrupteurs pilotables. Idem pour la pompe qui nécessite un relai pour être pilotée depuis l'arduino.
Comme la laveuse est équipée de 8 électrovannes et d'une pompe, il nous faut 9 relais au total. On utilise une carte de 16 relais (les modèles disponibles passent direct de 8 à 16). Cette carte fonctionne avec une alimentation en 12V et chaque relai consomme 20mA.

![Carte de relais](/images/relais.jpg)

Pour fournir une alimentation 12V à tous nos composants, on utilise un transformateur 230VAC/12VDC. Le 5V nécessaire à l'alimentation de l'arduino est fourni lui par la carte de relais. Le 12V sera également utilisé pour alimenter les électrovannes.

![Transformateur 12V](/images/transfo.jpg)

La première version de notre laveuse n'avait pas de protection contre les tensions de rappel générées par les électrovannes, ce qui a eu pour conséquence de cramer quelques relais en très peu de temps ([plus d'explications ici](http://www.yoctopuce.com/FR/article/relais-et-charges-inductives)).
Il faut donc bien penser à protéger chaque relai avec une diode de roue libre placée au plus près de celui-ci.

![Diode roue libre](/images/diode.jpg)

Le bouton de démarrage, le bouton de menu et le buzzer sont reliés directement à l'arduino en utilisant les I/O de celui-ci.
L'écran LCD est un écran I²C pour limiter le nombre de pins utilisées sur l'arduino.
L'utilisation d'un écran LCD permet la création d'un menu avec des combinaisons infinies, très pratique pour enrichir la laveuse avec de nouvelles fonctionnalités dans le futur.

Le bac de soude est équipé d'un thermoplongeur de 3500W qui permet de la maintenir à 80°C. Ce thermoplongeur est contrôlé par un thermostat STC1000. Comme le STC1000 ne peut pas piloter une charge d'une si grande puissance, on utilise un relai statique pour faire l'interface entre le thermostat et le thermoplongeur.

![Relai statique](/images/relai_statique.jpg)

Enfin l'alimentation générale est équipée d'un bouton coup de poing en cas d'arrêt d'urgence.

![Arrêt d'urgence](/images/urgence.jpg)


### Plus d'infos

#### Arduino

L'arduino utilisé sur notre laveuse est un modèle Uno, mais d'autres modèles peuvent être utilisés.

Voici les pins utilisées sur cet arduino :

| Pin de l'arduino | Reliée à |
|--|--|
| A0 | Buzzer |
| A1 | Bouton haut |
| A2 | Bouton bas |
| A3 | Bouton action |
| A4 | Écran LCD SDA |
| A5 | Écran LCD SCL |
| 2 | Relai électrovanne air |
| 3 | Relai électrovanne CO2 |
| 4 | Relai électrovanne eau |
| 5 | Relai électrovanne sortie cuve détergent |
| 6 | Relai électrovanne sortie cuve désinfectant |
| 7 | Relai électrovanne retour cuve détergent |
| 8 | Relai électrovanne retour cuve désinfectant |
| 9 | Relai électrovanne égoût |
| 10 | Relai pompe |
| 11 | LED |

#### Câblage

![Câblage](/images/cablage1.jpg)
![Câblage façade](/images/cablage2.jpg)

Les cartes électroniques ont été choisies de manière à n'avoir aucune soudure à faire ni aucune carte "maison". Les connecteurs des cartes sont directement reliés entre eux grâce à des câbles dupont.

Les câbles utilisés pour les alimentations électriques doivent avoir une section correspondant à l'intensité qui y circule :

Câbles dupont pour :

- signaux électroniques
- alimentation 5V arduino

Câbles électriques 0,5mm² pour :

- électrovannes
- alimentation 12V de la carte de relais

Câbles électriques 1,5mm² pour :
- alimentation 230V du transformateur 12V
- alimentation de la pompe

Câbles électriques 2,5mm² pour :
- alimentation générale
- interrupteur général
- alimentation thermoplongeur

#### Relier les électrovannes aux relais

Les relais sont reliés aux électrovannes et à l'alimentation grâce à des dominos. La diode de roue libre qui protège les relais est également placée à cet endroit.

![Dominos](/images/dominos.jpg)

## Programmation

Avant de pouvoir programmer l'arduino, il faut installer les logiciels arduino sur votre ordinateur.
Vous avez le choix entre :

1. [utiliser l'IDE arduino](https://www.arduino.cc/en/Main/Software) : un logiciel graphique simple pour les débutants
2. [utiliser Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) : un outil en ligne de commande pour les utilisateurs avancés

Il faut ensuite [installer les dépendances](https://www.arduino.cc/en/Guide/Libraries) à notre programme, à savoir les bibliothèques  `Bounce2`, `LiquidCrystal_I2C` et `RotaryEncoder`. Ces bibliothèques sont disponibles depuis l'interface de l'IDE arduino.

Une fois l'environnement arduino installé, vous pouvez brancher l'arduino en USB à votre ordinateur.

Le programme à installer sur l'arduino tient en 1 seul fichier `kegwasher.ino`.

Pour les utilisateurs de l'IDE arduino, il faudra simplement ouvrir ce fichier et aller dans le menu Croquis > Téléverser.
Pour les utilisateurs d'Arduino-Makefile, il faudra configurer le port de l'arduino dans le fichier Makefile et vous pourrez ensuite exécuter les commandes suivantes :

    make
    make upload

Le programme `kegwasher.ino` contient une constante appelée `MODES` qui définit les différents modes de lavage que l'on peut choisir via le menu de la laveuse.
Chaque mode consiste en une succession d'étapes définies un peu plus haut dans le programme. Ces enchaînement d'étapes peuvent également être modifiés et personnalisés, notamment la durée de chaque étape.

L'utilisation du programme est ensuite très simple. Les boutons haut et bas permettent de choisir le mode de lavage qui s'affiche sur l'écran LCD.

![Menu](/images/menu.jpg)

Le bouton d'action permet de lancer le mode sélectionné.
Pendant le déroulement d'un mode, un compteur de temps s'affiche sur l'écran et la LED clignote.

![Nettoyage en cours](/images/mode.jpg)

Lorsqu'on appuie sur le bouton d'action pendant un mode, le mode s'arrête et on retourne au menu principal. Sinon le mode se termine avec un bip et on retourne au menu principal.

Lorsque la laveuse est débranchée puis rebranchée, le dernier mode utilisé est choisi par défaut dans le menu.

## Structure

La structure de la laveuse est réalisée sur un cadre en acier fait de cornières soudées maison et fixée sur des roulettes.

![Cadre](/images/cadre1.jpg)
![Cadre](/images/cadre2.jpg)

Les cuves et la pompe sont posées sur la partie basse. La partie haute dispose de barres de support pour poser les fûts la tête à l'envers.

![Supports pour les fûts](/images/supports_futs.jpg)

Des petites gouttières ont été soudées au cadre pour positionner les électrovannes.

![Supports pour les vannes](/images/supports_vannes.jpg)

Côté électrique, les nombreux câbles des électrovannes ont été cachés dans des gaines.

![Gaines](/images/gaines.jpg)

La partie électronique est dissimulée dans un boîtier en plastique et fixée sur le côté du cadre.

![Boîtier électronique](/images/boitier_elec1.jpg)
![Boîtier électronique](/images/boitier_elec2.jpg)
![Boîtier électronique](/images/boitier_elec3.jpg)

### Et l'évier ?

L'évier fixé sur le dessus de la laveuse était prévu pour intégrer une boule de lavage et permettre le lavage des fûts ouverts ou des fûts de types sodakegs ou tout autre contenant, mais ça n'a pas été encore mis en place faute de temps.

## Composants utilisés et coût

Cette liste correspond à peu près à ce que nous avons acheté pour construire notre laveuse.
Les prix ont certainement évolué depuis, donc à prendre avec des pincettes.

<table>
  <tr>
    <th>Composant</th>
    <th>Fournisseur</th>
    <th>Quantité</th>
    <th>PU (HT)</th>
    <th>Total (HT)</th>
  </tr>
  <tr>
    <td colspan="5"></td>
  </tr>
  <tr>
    <th colspan="5" align="left">Structure</th>
  </tr>
  <tr>
    <td>Cornière et carré acier</td>
    <td>Ferrailleur</td>
    <td align="right">1</td>
    <td align="right">80,00 €</td>
    <td align="right">80,00 €</td>
  </tr>
  <tr>
    <td>Peinture acier</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">30,00 €</td>
    <td align="right">30,00 €</td>
  </tr>
  <tr>
    <td>Roulette</td>
    <td>Récup</td>
    <td align="right">4</td>
    <td align="right">10,00 €</td>
    <td align="right">40,00 €</td>
  </tr>
  <tr>
    <td>Boîtier électrique</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">37,00 €</td>
    <td align="right">37,00 €</td>
  </tr>
  <tr>
    <th align="left" colspan="4">Sous-total</th>
    <th align="right">187,00 €</th>
  </tr>
  <tr>
    <td colspan="5"></td>
  </tr>
  <tr>
    <th colspan="5" align="left">Plomberie</th>
  </tr>
  <tr>
    <td>Têtes de lavage</td>
    <td>Maisonneuve</td>
    <td align="right">2</td>
    <td align="right">52,00 €</td>
    <td align="right">104,00 €</td>
  </tr>
  <tr>
    <td>Pompe Novax 20B</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">79,17 €</td>
    <td align="right">79,17 €</td>
  </tr>
  <tr>
    <td>Electrovannes solénoide inox 3/4’’ 12V</td>
    <td><a href="https://www.amazon.fr/U-S-Solid-Électrovanne-Normalement-entraînement/dp/B00DQ24GS6/">Amazon</a></td>
    <td align="right">6</td>
    <td align="right">26,46 €</td>
    <td align="right">158,76 €</td>
  </tr>
  <tr>
    <td>Electrovannes solénoide inox 1/2’’ 12V</td>
    <td><a href="https://www.amazon.fr/U-S-Solid-%C3%89lectrovanne-Normalement-entra%C3%AEnement/dp/B00APDA6Z6/">Amazon</a></td>
    <td align="right">2</td>
    <td align="right">26,66 €</td>
    <td align="right">53,32 €</td>
  </tr>
  <tr>
    <td>Tuyau thermoclean 100 19mm x 20m</td>
    <td><a href="http://www.raccordspro.com/fr/553-tuyau-thermoclean-100.html">Raccords pro</a></td>
    <td align="right">1</td>
    <td align="right">174,20 €</td>
    <td align="right">174,20 €</td>
  </tr>
  <tr>
    <td>Tuyau crystal 12mm x 5m</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">9,08 €</td>
    <td align="right">9,08 €</td>
  </tr>
  <tr>
    <td>Clapet antiretour 3/4’’ inox</td>
    <td><a href="https://www.pompes-h2o.com/fr/clapet-anti-retour-inox/3501-clapet-a-disque-inox-316-3-pieces.html">Pompes H2O</a></td>
    <td align="right">1</td>
    <td align="right">46,67 €</td>
    <td align="right">46,67 €</td>
  </tr>
  <tr>
    <td>Clapet antiretour 1/2’’ inox</td>
    <td><a href="https://www.pompes-h2o.com/fr/clapet-anti-retour-inox/3501-clapet-a-disque-inox-316-3-pieces.html">Pompes H2O</a></td>
    <td align="right">1</td>
    <td align="right">36,83 €</td>
    <td align="right">36,83 €</td>
  </tr>
  <tr>
    <td>Raccord gardena 3/4’’ M laiton</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">3,75 €</td>
    <td align="right">3,75 €</td>
  </tr>
  <tr>
    <td>Raccord cannelé 3/4’’ M inox</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">16</td>
    <td align="right">3,75 €</td>
    <td align="right">60,00 €</td>
  </tr>
  <tr>
    <td>Raccord cannelé 3/4’’ F inox</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">5</td>
    <td align="right">4,08 €</td>
    <td align="right">20,40 €</td>
  </tr>
  <tr>
    <td>Raccord cannelé 1/2’’ M inox</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">2</td>
    <td align="right">3,00 €</td>
    <td align="right">6,00 €</td>
  </tr>
  <tr>
    <td>Coude inox 1/2’’ F/F</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">2,20 €</td>
    <td align="right">2,20 €</td>
  </tr>
  <tr>
    <td>Coude inox 3/4’’ F/F</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">2</td>
    <td align="right">3,54 €</td>
    <td align="right">7,08 €</td>
  </tr>
  <tr>
    <td>Té inox 3/4’’ F</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">7</td>
    <td align="right">3,92 €</td>
    <td align="right">27,44 €</td>
  </tr>
  <tr>
    <td>Té inox 1/2’’ F</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">3,08 €</td>
    <td align="right">3,08 €</td>
  </tr>
  <tr>
    <td>Réduction 3/4’’ M vers 1/2’’ M</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">2,00 €</td>
    <td align="right">2,00 €</td>
  </tr>
  <tr>
    <td>Mamelon inox 3/4’’ M/M</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">10</td>
    <td align="right">2,21 €</td>
    <td align="right">22,10 €</td>
  </tr>
  <tr>
    <td>Mamelon inox 1/2’’ M/M</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">3</td>
    <td align="right">1,83 €</td>
    <td align="right">5,49 €</td>
  </tr>
  <tr>
    <td>Colliers inox pour thermoclean</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">25</td>
    <td align="right">0,79 €</td>
    <td align="right">19,75 €</td>
  </tr>
  <tr>
    <td>Colliers inox pour crystal</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">2</td>
    <td align="right">0,75 €</td>
    <td align="right">1,50 €</td>
  </tr>
  <tr>
    <td>Cuve inox</td>
    <td>Bergland</td>
    <td align="right">2</td>
    <td align="right">35,21 €</td>
    <td align="right">70,42 €</td>
  </tr>
  <tr>
    <td>Écrou 1’’ pour thermoplongeur</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">2,83 €</td>
    <td align="right">2,83 €</td>
  </tr>
  <tr>
    <td>Kit d’étanchéité 1’’</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">7,67 €</td>
    <td align="right">7,67 €</td>
  </tr>
  <tr>
    <td>Kit d’étanchéité 3/4’’</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">4</td>
    <td align="right">3,67 €</td>
    <td align="right">14,68 €</td>
  </tr>
  <tr>
    <td>Écrou 3/4’’</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">4</td>
    <td align="right">2,16 €</td>
    <td align="right">8,64 €</td>
  </tr>
  <tr>
    <td>Coude inox 3/4’’ M/F</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">2</td>
    <td align="right">3,67 €</td>
    <td align="right">7,34 €</td>
  </tr>
  <tr>
    <td>Emporte pièce 1’’</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">13,75 €</td>
    <td align="right">13,75 €</td>
  </tr>
  <tr>
    <td>Emporte pièce 3/4’’</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">11,88 €</td>
    <td align="right">11,88 €</td>
  </tr>
  <tr>
    <td>Bobine 3/4’’</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">4</td>
    <td align="right">3,21 €</td>
    <td align="right">12,84 €</td>
  </tr>
  <tr>
    <td>Kit d’étanchéité 1/2’’ pour sonde de température</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">3,00 €</td>
    <td align="right">3,00 €</td>
  </tr>
  <tr>
    <td>Écrou 1/2’’</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">1,92 €</td>
    <td align="right">1,92 €</td>
  </tr>
  <tr>
    <td>Détendeur intermédiaire air comprimé</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">33,25 €</td>
    <td align="right">33,25 €</td>
  </tr>
  <tr>
    <td>Raccord rapide air comprimé M</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">3,33 €</td>
    <td align="right">3,33 €</td>
  </tr>
  <tr>
    <td>Raccord rapide air comprimé F</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">9,92 €</td>
    <td align="right">9,92 €</td>
  </tr>
  <tr>
    <th align="left" colspan="4">Sous-total</th>
    <th align="right">1 044,29 €</th>
  </tr>
  <tr>
    <td colspan="5"></td>
  </tr>
  <tr>
    <th colspan="5" align="left">Électronique</th>
  </tr>
  <tr>
    <td>Arduino Uno</td>
    <td><a href="https://boutique.semageek.com/fr/">Semageek</a></td>
    <td align="right">1</td>
    <td align="right">16,25 €</td>
    <td align="right">16,25 €</td>
  </tr>
  <tr>
    <td>Carte 16 relais</td>
    <td><a href="https://boutique.semageek.com/fr/">Semageek</a></td>
    <td align="right">1</td>
    <td align="right">25,00 €</td>
    <td align="right">25,00 €</td>
  </tr>
  <tr>
    <td>Diodes 1N4148</td>
    <td><a href="https://boutique.semageek.com/fr/">Semageek</a></td>
    <td align="right">1</td>
    <td align="right">0,83 €</td>
    <td align="right">0,83 €</td>
  </tr>
  <tr>
    <td>Câbles dupont</td>
    <td><a href="https://boutique.semageek.com/fr/">Semageek</a></td>
    <td align="right">3</td>
    <td align="right">3,95 €</td>
    <td align="right">11,85 €</td>
  </tr>
  <tr>
    <td>Transformateur 220V 12V 120W</td>
    <td><a href="https://www.amazon.fr/IKSACE-Transformateur-Alimentation-Adaptateur-bande/dp/B074V1RT3Q">Amazon</a></td>
    <td align="right">1</td>
    <td align="right">13,32 €</td>
    <td align="right">13,32 €</td>
  </tr>
  <tr>
    <td>Écran LCD</td>
    <td><a href="https://www.amazon.fr/gp/product/B01N3B8JMN">Amazon</a></td>
    <td align="right">1</td>
    <td align="right">6,67 €</td>
    <td align="right">6,67 €</td>
  </tr>
  <tr>
    <td>Bouton sélecteur menu</td>
    <td><a href="https://boutique.semageek.com/fr/896-encodeur-rotatif-24-positions-avec-validation-knob.html">Semageek</a></td>
    <td align="right">1</td>
    <td align="right">4,50 €</td>
    <td align="right">4,50 €</td>
  </tr>
  <tr>
    <td>Bouton arrêt d’urgence</td>
    <td><a href="https://www.amazon.fr/gp/product/B00H3CY432">Amazon</a></td>
    <td align="right">1</td>
    <td align="right">2,31 €</td>
    <td align="right">2,31 €</td>
  </tr>
  <tr>
    <td>Bouton poussoir + LED</td>
    <td><a href="https://boutique.semageek.com/fr/207-boutons-poussoirs-lumineux">Semageek</a></td>
    <td align="right">1</td>
    <td align="right">4,95 €</td>
    <td align="right">4,95 €</td>
  </tr>
  <tr>
    <td>Buzzer</td>
    <td><a href="https://www.amazon.fr/GLOGLOW-électronique-tonalité-Piézo-électrique-Sonnerie/dp/B07F5LB8K9">Amazon</a></td>
    <td align="right">1</td>
    <td align="right">5,58 €</td>
    <td align="right">5,58 €</td>
  </tr>
  <tr>
    <td>Câble 2 fils pour électrovannes 0,75mm² x10m</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">5,83 €</td>
    <td align="right">5,83 €</td>
  </tr>
  <tr>
    <td>Câbles 1 fil 1,5mm² plusieurs couleurs</td>
    <td>Récup</td>
    <td align="right">1</td>
    <td align="right">0,00 €</td>
    <td align="right">0,00 €</td>
  </tr>
  <tr>
    <td>Câble 3 fils alimentation souple 2,5mm² x5m</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">11,67 €</td>
    <td align="right">11,67 €</td>
  </tr>
  <tr>
    <td>Prise 220v mâle étanche</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">8,42 €</td>
    <td align="right">8,42 €</td>
  </tr>
  <tr>
    <td>Prise 220v femelle étanche</td>
    <td>Leroy Merlin</td>
    <td align="right">1</td>
    <td align="right">7,32 €</td>
    <td align="right">7,32 €</td>
  </tr>
  <tr>
    <td>Relai statique 40A</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">7,50 €</td>
    <td align="right">7,50 €</td>
  </tr>
  <tr>
    <td>Thermostat STC1000</td>
    <td><a href="https://www.amazon.fr/Proster-Température-lApprobation-Réfrigérateur-Congélateur/dp/B00KYL3W32">Amazon</a></td>
    <td align="right">1</td>
    <td align="right">14,16 €</td>
    <td align="right">14,16 €</td>
  </tr>
  <tr>
    <td>Thermoplongeur 3,5kW</td>
    <td><a href="microbrassage.com">Canard rouge</a></td>
    <td align="right">1</td>
    <td align="right">35,00 €</td>
    <td align="right">35,00 €</td>
  </tr>
  <tr>
    <td>Sonde température inox</td>
    <td><a href="https://sondes-thermiques.fr/sonde-a-thermistance-ntc/3692-sonde-ntc-10k-raccord-g12-plongeur-50-a-400mm.html">Guilcor</a></td>
    <td align="right">1</td>
    <td align="right">9,80 €</td>
    <td align="right">9,80 €</td>
  </tr>
  <tr>
    <th align="left" colspan="4">Sous-total</th>
    <th align="right">190,96 €</th>
  </tr>
  <tr>
    <td colspan="5"></td>
  </tr>
  <tr>
    <th colspan="5" align="left">Divers</th>
  </tr>
  <tr>
    <td>Frais de port</td>
    <td></td>
    <td align="right">1</td>
    <td align="right">60,00 €</td>
    <td align="right">60,00 €</td>
  </tr>
  <tr>
    <th align="left" colspan="4">Sous-total</th>
    <th align="right">60,00 €</th>
  </tr>
  <tr>
    <td colspan="5"></td>
  </tr>
  <tr>
    <th align="left" colspan="4">Total</th>
    <th align="right">1 482,25 €</th>
  </tr>
</table>

## Améliorations

À venir

## Références

À venir

## Questions fréquentes

### LiquidCrystal_I2C

Le programme utilise une version particulière de la bibliothèque LiquidCrystal_I2C.

La bibliothèque à utiliser est celle-ci :

https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

Si l'erreur persiste, il est possible qu'il y ait un conflit entre plusieurs bibliothèques. Il faut alors désinstaller toutes les versions de LiquidCrystal_I2C qui ont été précédemment installées sur la machine.

### Est-ce que vous utilisez toujours cette laveuse ?

(au 20 Mai 2022) nous utilisons toujours cette laveuse, et près de 50 brasseries l'ont aussi fabriquée ! Elle tourne plutôt bien, même si nous avons parfois quelques retours de fûts problématiques (environ une dizaine pour une saison, 1% de nos sorties, au vu des ~1000 fûts qu'on passe chaque année.

### Quelles sont les améliorations / changements que vous avez faits ?

Nous avons fait peu de changement sur la laveuse depuis sa création, mais voici une petite liste :

- Nous avons ajouté des vannes de vidange sur les cuves pour pouvoir les vider plus facilement et manuellement ;
- Nous avons changé de modèle de pompe pour en mettre une plus puissante pour être sur que les liquides soient envoyés suffisamment puissamment dans les fûts.
- Nous sommes en train (20 mai 2022) d'installer une centrale vapeur dans la brasserie, et l'idée est de remplacer les cycles d'acide par de la vapeur d'eau, ce qui devrait permettre d'avoir une désinfection plus efficace.
