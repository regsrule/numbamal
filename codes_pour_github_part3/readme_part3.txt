Bienvenue sur NumBaMal Partie3

Ici on procede à la clusterisation et on termine par la renumérotation du graphe de base choisit:

Pour se faire, on lance la commande make run du le fichier makefile; ce qui execute la commande suivante:

./cluster dist_karate.txt soc-karate_prepared.txt

dist_karate.txt : fichier distance obtenu au prealable; obtenue à l'étape précédente.
soc-karate_prepared.txt : fichier du graphe de base, on l'utilise pour la renumérotation.


