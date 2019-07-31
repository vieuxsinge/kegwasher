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

## Démo

Parce qu'une vidéo vaut parfois mieux qu'un long discours :

* [Laveuse en cours de fonctionnement](https://peertube.servebeer.com/videos/watch/00c410c9-e059-47cc-8bc2-f5a8decc9740)
* [Fonctionnement général](https://vimeo.com/318256040)
* [Description de la partie électronique](https://vimeo.com/318257333)


## Plomberie

Les cuves produits sont réalisées avec des marmites Bergland 50L bien connues des brasseurs amateurs. Des kits d'étanchéité Canard rouge et des raccords cannelés permettent de réaliser très simplement des entrées et sorties pour les produits.

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

Il faut ensuite [installer les dépendances](https://www.arduino.cc/en/Guide/Libraries) à notre programme, à savoir les bibliothèques  `Bounce2` et `LiquidCrystal_I2C`. Ces bibliothèques sont normalement disponibles depuis l'interface de l'IDE arduino.

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

À venir

## Améliorations

À venir

## Références

À venir




