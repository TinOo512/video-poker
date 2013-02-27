// CACHER LA CONSOLE
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup") 

#include "include.h"

/********************************************************************/
/*					INITIALISATION DES VAR GLOBAL					*/
/********************************************************************/

// Variables Global pour GtkBuilder
GtkBuilder  *  g_interface   = NULL;

// Autorise l'execution des message console
const int AUTH_CONSOLE = FALSE;

// Constantes
const char IMG_FOLDER[10] = "./Images/";

// Stokage des Nom d'images de chaque carte et son ID correspondant
char IMG_Cartes[53][5] = {0};

// THE paquet de cartes
int PCartes[53] = {0};

// Sauvegarde la face actuelle de chaque carte ( 0 = dos , 1 = face )
int FCartes[6] = {0}, cFace = 0;

// Sauvegarde des différent time(NULL) afin de supprimer les srand identique ( dans le cas d'une attente inférieure a 1s )
// !!! Aggrandir ce Tableau au besoin !!!!
// colonne 1 : id des différent Time ( peut etre agrandi )
// colonne 2 : 0 == time(NULL) | 1 == compteur time identique
int SauveTime[3][2] = {0};

// Total des gains & Mise
int Total = 0, Mise = 0;

// nb parties autorisé & total
int nbParties = 0, nbPartiesTotal = 0;

// compteur des etapes de chaque tour
int TourEtape = 1;

// widget des 5 cartes
GtkWidget * WidCartes[6] = {NULL};

// sauvegarde de la taille de chaque cartes
int SizeCarte[2] = {70,105};
int SizeWin[2] = {800,600};

// sauvegarde le lien du background Menu
char * BackMenu = "";

/********************************************************************/
/********************************************************************/

int main (int argc, char* argv[])
{	
	GtkWidget	*  p_win	= NULL;
	GtkWidget	*  winAide	= NULL;
	
	GError      *  g_erreur = NULL;
	GdkGeometry hints;
	int i = NULL;

	if(!g_thread_supported())
	{
		// Initialisation de GTK Threads
		g_thread_init(NULL);
		gdk_threads_init();
		//gdk_threads_enter();
		printf("g_thread supported\n");

	}
	else
	{
		printf("g_thread NOT supported\n");
		system("PAUSE");
		return EXIT_FAILURE;
	}

	// Initialisation de GTK+
	gtk_init(&argc, &argv);

	// Initialisation de GtkBuilder
	g_interface = gtk_builder_new();	
		
	// Chargement de la feuille de style
	gtk_rc_parse("./style.rc");

	if (g_interface != NULL)
	{
		// Chargement de l'interface graphique XML via GtkBuilder
		gtk_builder_add_from_file(g_interface, "Interface.glade", &g_erreur);

		if (g_erreur == NULL)
		{
			// Recuparation d'un pointeur sur la fenetre principal window
			p_win = (GtkWidget *)gtk_builder_get_object(g_interface, "window");
			winAide = (GtkWidget *)gtk_builder_get_object(g_interface, "winAide");
			
			hints.min_width = 700;
			hints.min_height = 525;
			hints.min_aspect = 1.3333333333333333;
			hints.max_aspect = 1.3333333333333333;

			gtk_window_set_geometry_hints (GTK_WINDOW (p_win), GTK_WIDGET (p_win), &hints, (GdkWindowHints)(GDK_HINT_ASPECT | GDK_HINT_MIN_SIZE));			

			// Auto Connection des signaux de l'interface graphique
			gtk_builder_connect_signals(g_interface, NULL);

			/********************************************************************/
			/*							CORE PROGRAMME							*/
			/********************************************************************/
				
				// Affichage de tous les widgets de la fenetre principal window et lancement de la boucle GTK

				//gtk_widget_set_double_buffered(p_win,FALSE);
				
				AttribCarte();

				SetBG("Menu.png");

				gtk_widget_show_all(p_win);
				//gtk_window_fullscreen(GTK_WINDOW(p_win));
				
				gtk_main();

			/********************************************************************/
			/*						  FIN CORE PROGRAMME						*/
			/********************************************************************/

		}
		else
		{
			Msg_GError(g_erreur);
			return EXIT_FAILURE;
		}
	}
	else
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
