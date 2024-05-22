# Projet IG PLC

*Equipe :*  
*BADIANE Seydina*  
*BOSSE Christian*  
*CHAUVELON Quentin*  

## Tests

Outre les applications de tests qui nous étaient fournies et qui nous ont permis de tester notre implémentation de l'API au fur et à mesure du développement, nous avons également écrit nos propres applications de tests pour vérifier le fonctionnement de notre code, non pas sur des cas génériques mais plutôt sur des cas plus spécifiques pour s'assurer du bon respect de toutes les contraintes du sujet. Certains éléments ont aussi été testés en modifiant les fichiers de tests qui nous étaient donnés sans pour autant créer un fichier spécifique.
Les tests que nous avons ajoutés nous ont aussi permis de vérifier le bon fonctionnement des extensions que nous avons réalisées.
Voici la liste des tests que nous avons écrits et ce qu'ils nous ont permis de tester :

### Text

`make text`

Ce fichier nous a permis de tester l'affichage de texte dans divers conditions : couleur, ancrage, overlap...
Une toplevel a aussi été ajoutée et peut être déplacée pour s'assurer que le texte est redessiné correctement lorsque l'on passe dessus

### Image

`make image`

Ce fichier nous a permis de tester l'affichage d'images dans divers conditions : image plus grande/plus petite que le widget, rectangle (subpart) dans ou en dehors de l'image, ancrage de l'image...
Une toplevel a aussi été ajoutée et peut être déplacée pour s'assurer que l'image est redessinée correctement lorsque l'on passe dessus

### Resize toplevel

`make resize_toplevel`

Ce fichier nous a permis de tester que le redimensionnement d'une toplevel se faisait bien sur l'axe spécifié

### Placer

`make placer`

Ce fichier nous a permis de tester les différents paramètres du placer : position relative/absolue, taille relative/absolue, ancrage...

### Radio button

`make radiobutton`

Ce fichier reproduit l'exemple des radio buttons donnés dans le sujet (Figure 4.6). Il nous a permis de tester le bon fonctionnement des radio buttons sur un test simple

### Radio buttons

`make radiobuttons`

Ce fichier nous a permis de tester dans un premier temps la configuration correcte des radio buttons (ainsi que les valeurs par défaut de leur groupe), en fonction de l'ordre d'appel des diverses fonctions. Puis dans un second temps, il nous a permis de tester les différentes fonctions de l'API (récupérer le radio button sélectionné dans un groupe, savoir si un radio button est sélectionné, récupérer le texte du radio button sélectionné...)
Plus d'informations sont disponibles dans le fichier `tests/radio_buttons.c`

### Custom tags

`make tags`

Ce fichier nous a permis de tester les tags personnalisés et notamment la création d'un tag par l'utilisateur, y associer une fonction et l'ajouter à n'importe quel widget. Il nous permet aussi de nous assurer que l'on peut supprimer un tag de certains widgets (ex: enlever le comportement par défaut d'un bouton)

### Parsing

`make parsing`

Ce fichier reproduit l'exemple du parser donné dans le sujet (Figure 4.6). Il nous a permis de tester le bon fonctionnement du parser sur un test simple

### Parsing ko

`make parsing_ko`

Ce fichier nous a permis de tester le parser sur divers exemples avec une syntaxe invalide. Tous les exemples ne peuvent pas être testés d'un coup, il faut commenter/décommenter les lignes (de 70 à 77) au fur et à mesure pour tester chaque exemple

### Tags parsing

`make tags_parsing`

Ce fichier nous a permis de tester le parser sur un fichier plus complexe. En effet, `tags_parsing.txt` reproduit une hiérarchie de widgets similaire à celle de [Custom tags](#custom-tags) grâce au parser. Cela nous a ainsi permis de tester la création et configuration des divers widgets, ainsi que de s'assurer qu'ils fonctionnent correctement par la suite, par exemple en y ajoutant des callbacks (les mêmes que dans [Custom tags](#custom-tags))

### Personal Information Grid

`make pi_grid`

Ce fichier reproduit l'exemple du gestionnaire de géométrie en grille donné dans le sujet (Figure 4.7) et ajoute certaines cellules notamment pour tester l'étalement d'une cellule sur plusieurs lignes/colonnes, l'ancrage, maximiser la taille d'une ligne/colonne...

### Grid playground

`make grid_playground`

Ce fichier nous a permis de tester les différentes fonctionnalités des grilles de manière dynamique (il est possible de jouer avec les paramètres de la grille, ajouter des cellules, etc... pour voir le résultat en temps réel). Il permet de tester l'ancrage, margin, espacement, anchrage/remplissage d'un widget dans une cellule, maximisation de la taille d'une ou plusieurs lignes/colonnes (en cliquant sur les boutons Row ou Column en en-tête de ligne/colonne)...
Plus d'informations sont disponibles dans le fichier `tests/grid_playground.c`

### Content Rect Visualization

`make content_rect_visualization`

Ce fichier nous a permis de vérifier que le content rect de chaque widget était bien calculée. Ainsi, on peut voir en couleur la zone du content rect des divers widgets et vérifier qu'elle est bien celle que nous attendions

### Offscreen picking surface

Bien qu'il n'y ait pas de fichier de test à part entière. Nous avons ajouté une fonctionnalité permettant de visualiser l'offscreen de picking pour chaque application. Il suffit d'appuyer sur ctrl+g pour afficher/masquer l'offscreen de picking. Cette fontionnalité est plus un outil de debug qu'un vrai test et peut ainsi contenir quelques bugs. Nous déconseillons ainsi de relâcher la touche ctrl avant de masquer l'offscreen de picking, car cela peut provoquer des bugs d'affichage (appuyer sur ctrl+g pour l'afficher, relâcher g, puis appuyer à nouveau sur g pour le masquer).