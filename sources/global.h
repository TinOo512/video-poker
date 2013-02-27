#ifndef global_h
#define global_h

	// Variables Global pour GtkBuilder
	extern GtkBuilder  *  g_interface;

	// Autorise l'execution des message console
	extern const int AUTH_CONSOLE;

	// Constantes
	extern const char IMG_FOLDER[10];

	// Stokage des Nom d'images de chaque carte et son ID correspondant
	extern char IMG_Cartes[53][5];

	// THE paquet de cartes
	extern int PCartes[53];

	// Sauvegarde la face actuelle de chaque carte ( 0 = dos , 1 = face )
	extern int FCartes[6], cFace;

	// Sauvegarde des différent time(NULL) afin de supprimer les srand identique ( dans le cas d'une attente inférieure a 1s )
	// !!! Aggrandir ce Tableau au besoin !!!!
	// colonne 1 : id des différent Time ( peut etre agrandi )
	// colonne 2 : 0 == time(NULL) | 1 == compteur time identique
	extern int SauveTime[3][2];

	// Total des gains & Mise
	extern int Total, Mise;

	// nb parties autorisé & total
	extern int nbParties, nbPartiesTotal;

	// compteur des etapes de chaque tour
	extern int TourEtape;

	// widget des 5 cartes
	extern GtkWidget * WidCartes[6];

	// sauvegarde de la taille de chaque cartes
	extern int SizeCarte[2];
	extern int SizeWin[2];

	// sauvegarde le lien du background Menu
	extern char * BackMenu;

#endif