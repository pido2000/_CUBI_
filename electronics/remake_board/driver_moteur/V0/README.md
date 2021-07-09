### Description

Le projet "driver_moteur" est un prototype visant à séparer la carte électronique globale en 2 modules distincts pour faciliter le placement de cette carte à l'interieur du robot.
Pour une approche plus proche du Plug-And-Play, on voudrait aussi implanter des connecteurs magnétiques (Pogo pins) permettant de lier les 2 cartes facilement.

Un premier module s'occuperait de la gestion d'alimentation globale, avec le driver des moteurs et du régulateur 9V vers 5V.
Le deuxième module utiliserait le reste des composants traitant les données, avec le µC, le module Bluetooth, le buzzer et la centrale inertielle.

Ce projet est une piste d'amélioration (et d'optimisation) de la carte électronique.

### Inconvénient projet

Si l'on veut séparer la carte en 2 modules, cela implique un coût supplémentaire dû à la fabrication de 2 cartes distinctes + aux rajouts de connecteurs magnétiques liant celles-ci.

### Avantage projet

On peut connecter directement le module de gestion d'alim aux moteurs sans câbles et "plugger" l'autre module directement.
En effet, on peut difficilement créer une carte globale qui respecte les conditions de placement des connecteurs moteurs ET simplifie la mécanique du robot. 

### Notes à part

Nous n'avons créé qu'une seule version qui n'a pas encore été testée.
De plus, certaines soudures sont difficilement réalisables si l'on veut fabriquer le module soi-même.

Comme l'on possède 2 connecteurs Pogo de 8 broches, on a rajouté 4 pins disponibles pour commander des Leds supplémentaires.
Mais on peut tout à fait n'utiliser que 12 broches au total pour une version ultérieure du module.
