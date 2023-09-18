



Projet de synthèse d'images
E5 Computer Graphics – Spring 2023
Thème : Rollercoaster

![image](https://github.com/EvonDev5799/OpenGL/assets/87436005/ccf97d62-d32c-48e8-8b70-b5b1039cadad)

Détails des fonctionnalités :

caméra mobile, Freefly utilisé, Trackball implémenté mais non exploité\
Tous les objets sont texturés avec double texture\
Une seule lumière, type soleil\
parcours de rollercoaster implémenté

Evolutions :

Le Projet implémente du multishadering avec un shader mono texture et un shader de multitexturing (à deux texture) tous les deux implémentant les fonctionnalités de lumière.\
Le Projet implémente un système de parcours de Rollercoaster avec bon nombre de fonctionnalités :\
-	Il est possible de créer un trajet à partir de points de passage. On peut ensuite se déplacer le long de ce trajet et récupérer diverses données sur la position comme la direction de déplacement ou la model matrix associé (translation sur la piste ainsi que rotation pour être dans l’axe de celle-ci). Si on projette le wagon avec cette matrice on obtient une configuration bonne de parcours long du trajet.

-	Il est possible de créer procéduralement un mesh de rail associé à des points de passages. Sachant que l’on dispose également d’une fonction qui permet d’avoir une piste décalée perpendiculairement à la piste d’origine on peut combiner les deux pour générer un mesh de rail.

-	Il est possible de créer procéduralement des piliers sous une trajectoire liée à des points de passages on peut donc créer des piliers soutenant la piste de Rollercoaster.


-	Il est possible de lisser les courbes d’un trajet en formant un nouveau trajet à partir d’un échantillonnage de courbes de Bézier, elles-mêmes formées à partir des segments du trajet d’origine.

-	Enfin pour calculer des mouvements réalistes pour le wagon, un système simple imite la physique de celui-ci et calcule une pseudo accélération et une pseudo vitesse.

Guide d'installation:
néçéssite un Système d'exploitation Linux et Cmake d'installé\

lancer la commande cmake puis make et executer "main.cpp" dans le dossier TPFinal

