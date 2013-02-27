#include "include.h"

//// Mutex, verrou zone critique pour multithread
//static pthread_mutex_t mutex_ViewCarte = PTHREAD_MUTEX_INITIALIZER;

void SwitchCarte(int pos)
{
	GThread *T_SwitchCarte;
	GError  *err1 = NULL;

	if( (T_SwitchCarte = g_thread_create((GThreadFunc)Thread_SwitchCarte, (void *)pos, TRUE, &err1)) == NULL)
	{
	     printf("Thread SwitchCarte Erreur : %s !!\n", err1->message);
	     g_error_free(err1);
	}
}

void *Thread_SwitchCarte(void *args)
{
	GtkWidget * carte , * image = NULL, * eventCarte = NULL;
	GdkPixbuf * pixbuf= NULL;
	int id_carte=0, pos=(int)args;
	char name_carte[7] = "carte", file_carte[50];

	id_carte = PCartes[pos];
	
	if(FCartes[pos] == 1)
	{
		strcpy(file_carte,IMG_FOLDER);
		strcat(file_carte, "Back.png");
	}
	else
	{
		// créaction du chemin du fichié img de la carte
		strcpy(file_carte,IMG_FOLDER);
		strcat(file_carte, IMG_Cartes[id_carte]);
		strcat(file_carte, ".png");
	}

	strcat(name_carte, ToString(pos));

	carte		= (GtkWidget *) gtk_builder_get_object (g_interface, name_carte);
	image		= (GtkWidget *) gtk_builder_get_object (g_interface, ToString(pos));
	eventCarte  = (GtkWidget *) gtk_builder_get_object (g_interface, ToString(pos));

	pixbuf = gdk_pixbuf_new_from_file_at_scale(file_carte,SizeCarte[0],SizeCarte[1],TRUE,NULL);
	gtk_image_set_from_pixbuf(GTK_IMAGE(WidCartes[pos]),pixbuf);
	gtk_widget_queue_draw(WidCartes[pos]);

	if(id_carte == PCartes[1] || id_carte == PCartes[2] || id_carte == PCartes[3] || id_carte == PCartes[4] || id_carte == PCartes[5])
	{
		if(FCartes[pos] == 1)
		{
			FCartes[pos] = 0;
		}
		else
		{
			FCartes[pos] = 1;
		}
	}

	if(AUTH_CONSOLE)
	{
		printf("Position : %d - Carte : %s\n",pos,IMG_Cartes[id_carte]);
	}

	
	gtk_widget_set_sensitive(eventCarte,TRUE);

	return NULL;
}

void *Thread_LaunchViewCarte(void * args)
{
	GtkButton* button = NULL;	
	GtkLabel* labelCheck = NULL;
	GThread *T_AppelViewCarte[6];
	GError  *err1 = NULL;
	gchar * sUtf8 = NULL;
	int pos = 1;

	for(pos=1;pos<6;pos++){
		if(TourEtape == 20)
		{
			if (FCartes[pos] == 0)
			{
				err1 = NULL;
				if( (T_AppelViewCarte[pos] = g_thread_create((GThreadFunc)Thread_ViewCarte, (void *)pos, TRUE, &err1)) == NULL)
				{
					printf("Thread ViewCarte [ %d ] Erreur : %s !!\n", pos, err1->message);
					g_error_free(err1);
				}
				else
				{
					g_thread_join(T_AppelViewCarte[pos]);
				}
			}
		}
		else
		{
			err1 = NULL;
			if( (T_AppelViewCarte[pos] = g_thread_create((GThreadFunc)Thread_ViewCarte, (void *)pos, TRUE, &err1)) == NULL)
			{
				printf("Thread ViewCarte [ %d ] Erreur : %s !!\n", pos, err1->message);
				g_error_free(err1);
			}
			else
			{
				g_thread_join(T_AppelViewCarte[pos]);
			}
		}
	}

	if(cFace == 5 && TourEtape == 20)
	{
		printf("checkCartes ...\n");
		checkCartes();
		button = (GtkButton *)gtk_builder_get_object(g_interface, "btnDistrib");
		gtk_button_set_label(button, "Rejouer");
		TourEtape = 3;
		gtk_widget_set_sensitive(GTK_WIDGET(button),TRUE);
	}
	else if(TourEtape == 10)
	{	
		button = (GtkButton *)gtk_builder_get_object(g_interface, "btnDistrib");
		gtk_button_set_label(button, "Cartes");

		sUtf8 = g_locale_to_utf8("Choisissez les cartes à \nremplacer puis cliquez sur \n\"Cartes\".", -1, NULL, NULL, NULL);
		labelCheck = (GtkLabel *)gtk_builder_get_object(g_interface, "labelCheck");
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);	

		TourEtape = 2;
		gtk_widget_set_sensitive(GTK_WIDGET(button),TRUE);
	}

	return NULL;
}

void ViewCarte2(int pos)
{
	GtkImage * carte = NULL;
	int id_carte = 0;
	char name_carte[7] = "carte", file_carte[50]="";

	id_carte = PCartes[pos];
	
	strcat(name_carte, ToString(pos));
	carte = (GtkImage *) gtk_builder_get_object (g_interface, name_carte);

	// créaction du chemin du fichié img de la carte
	strcpy(file_carte,IMG_FOLDER);
	strcat(file_carte, IMG_Cartes[id_carte]);
	strcat(file_carte, ".png");

	gtk_image_set_from_file(carte,file_carte);

	if(id_carte == PCartes[1] || id_carte == PCartes[2] || id_carte == PCartes[3] || id_carte == PCartes[4] || id_carte == PCartes[5]){
		FCartes[pos] = 1;
	}

	if(AUTH_CONSOLE)
	{
		printf("Position : %d - Carte : %s\n",pos,IMG_Cartes[id_carte]);
	}
}

void *Thread_ViewCarte(void * args)
{
	GdkPixbuf * pixbuf	 = NULL;
	GtkWidget * eventImg = NULL;

	char name_carte[7]="carte", file_carte[50]="";
	int id_carte=0, pos=(int)args;

	Sleep(200);

	if(TourEtape == 10)
	{
		id_carte = PCartes[pos];
	}
	else
	{
		id_carte = PCartes[pos+5];
	}				
	
	if(AUTH_CONSOLE)
	{
		printf("Position : %d - Carte : %s\n",pos,IMG_Cartes[id_carte]);
	}

	// créaction du chemin du fichié img de la carte
	strcpy(file_carte,IMG_FOLDER);
	strcat(file_carte, IMG_Cartes[id_carte]);
	strcat(file_carte, ".png");

	if(TourEtape == 10)
	{
		eventImg = (GtkWidget *) gtk_builder_get_object (g_interface, ToString(pos));
		pixbuf=gdk_pixbuf_new_from_file_at_scale(file_carte,SizeCarte[0],SizeCarte[1],TRUE,NULL);
		WidCartes[pos] = gtk_image_new_from_pixbuf(pixbuf);
		gtk_widget_set_size_request(eventImg,SizeCarte[0],SizeCarte[1]);
		gtk_widget_show(WidCartes[pos]);
		gtk_container_add(GTK_CONTAINER(eventImg), WidCartes[pos]);		
		gtk_widget_set_size_request(eventImg,-1,-1);
	}
	else
	{
		pixbuf=gdk_pixbuf_new_from_file_at_scale(file_carte,SizeCarte[0],SizeCarte[1],TRUE,NULL);
		gtk_image_set_from_pixbuf(GTK_IMAGE(WidCartes[pos]),pixbuf);
		gtk_widget_queue_draw(WidCartes[pos]);
	}

	g_object_unref(pixbuf);


	if(id_carte == PCartes[1] || id_carte == PCartes[2] || id_carte == PCartes[3] || id_carte == PCartes[4] || id_carte == PCartes[5]){
		FCartes[pos] = 1;
	}

	if(TourEtape == 20)
	{
		cFace++;			
	}			

	//Sleep(200);

	return NULL;
}

void *Thread_LaunchCacheCarte(void * args)
{
	GError			* err1				    = NULL;
	GThread			* T_AppelCacheCarte[6]  = {NULL};
	GtkLabel		* labelCheck			= NULL;
	GtkButton		* button				= (GtkButton *)gtk_builder_get_object(g_interface, "btnDistrib");
	GtkLabel		* labelMise				= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsMiseV");
	GtkLabel		* labelTotal			= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsTotalV");
	GtkLabel		* labelGains			= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsGainsV");

	int pos = 1, gains = 0;
	const gchar * labelButton = NULL;

	for(pos=5;pos>0;pos--)
	{
		if( (T_AppelCacheCarte[pos] = g_thread_create((GThreadFunc)Thread_CacheCarte, (void*)pos, TRUE, &err1)) == NULL)
		{
			printf("Thread CacheCarte [ %d ] Erreur : %s !!\n", pos, err1->message);
			g_error_free(err1);
		}
		else
		{
			g_thread_join(T_AppelCacheCarte[pos]);
		}
	}
	
	gtk_label_set_text(labelGains,"0");
	gtk_label_set_text(labelMise,"0");
	gtk_label_set_text(labelTotal,ToString(Total));			

	if(end() == 0)
	{
		nbParties--;
		
		SetMise(Total);

		gtk_button_set_label(button, "Miser");

		labelCheck = (GtkLabel *)gtk_builder_get_object(g_interface, "labelCheck");
		
		gtk_label_set_text(labelCheck,"Choisissez votre mise puis \ncliquez sur \"Miser\".");
	}

	TourEtape = 1;
	
	gtk_widget_set_sensitive(GTK_WIDGET(button),TRUE);

	return NULL;
}

void *Thread_CacheCarte(void * args)
{
	GtkWidget * carte = NULL;
	char name_carte[8] = "";
	int pos = (int)args;

	Sleep(100);

	strcpy(name_carte,"carte");
	strcat(name_carte, ToString(pos));

	carte = (GtkWidget *) gtk_builder_get_object (g_interface, name_carte);

	//gtk_widget_hide(WidCartes[pos]);
	//
	//Sleep(50);
	
	gtk_widget_destroy(WidCartes[pos]);

	//Sleep(100);

	return NULL;
}

void CacheCarte2(int pos)
{
	GtkImage * carte = NULL;
	char file_carte[50], name_carte[8];
	int id_carte = 0;

	strcpy(name_carte,"carte");
	strcat(name_carte, ToString(pos));

	carte = (GtkImage *) gtk_builder_get_object (g_interface, name_carte);

	strcpy(file_carte,IMG_FOLDER);
	strcat(file_carte, "Back.png");
	
	gtk_image_set_from_file(carte,file_carte);

	id_carte = PCartes[pos];

	if(id_carte == PCartes[1] || id_carte == PCartes[2] || id_carte == PCartes[3] || id_carte == PCartes[4] || id_carte == PCartes[5]){
		FCartes[pos] = 0;
	}
}

void MelCarte(void)
{
	int i=0, n_carte=1, i_coupe=0, coupe=0, u=0, packet[53]={0}, packet_temp[53]={0}, packet_check[53]={0}, temp_time=time(NULL);
	POINT point;

	GetCursorPos(&point);	

	if(temp_time == SauveTime[0][0])
	{
		SauveTime[0][1]++;
		temp_time = ( temp_time * ( SauveTime[0][1] + 1 ) );
	}
	else
	{
		SauveTime[0][0] = temp_time;
		SauveTime[0][1] = 0;
	}

	srand(temp_time+point.x+point.y);

	// 1er mélange des cartes
	for(i=1;i<53;i++)
	{
		while(packet_temp[i] == 0)
		{
			n_carte = (int)((rand() % 2809) / 53);
			
			if((n_carte != 0) && (packet_check[n_carte] == 0))
			{
				packet_temp[i] = n_carte;
				packet_check[n_carte] = 1;
			}
		}
	}

	GetCursorPos(&point);

	srand((time(NULL)*point.x)-point.y+rand());

	// Génération d'une coupe entre 3 et 37
	i_coupe = 0;
	while(i_coupe<3)
	{
		i_coupe = (int)((rand() % 119) / 3.14159265);
	}

	coupe = 52 - i_coupe;

	n_carte = 1;
	for(i=coupe;i<53;i++)
	{
		PCartes[n_carte] = packet_temp[i];
		n_carte++;
	}

	n_carte = 1;
	for(i=(54-coupe);i<53;i++)
	{
		PCartes[i] = packet_temp[n_carte];
		n_carte++;
	}

	if(AUTH_CONSOLE)
	{
		printf("\nOrdre des Cartes :\n\n");
	
		for(i=1;i<53;i++)
		{
			printf("%d|",PCartes[i]);
		}

		printf("\n\n");
	}
}

void AttribCarte(void)
{
	int i=0, cartes_color=1, cartes_num=0, cartes_check[5][14]={0};

	for(cartes_color=1;cartes_color<5;cartes_color++)
	{

		for(cartes_num=1;cartes_num<14;cartes_num++)
		{
			i++;
				
			cartes_check[cartes_color][cartes_num] = cartes_num;

			strcpy(IMG_Cartes[i], ToString(cartes_color));
			strcat(IMG_Cartes[i], "_");
			strcat(IMG_Cartes[i], ToString(cartes_num));

			if(AUTH_CONSOLE)
			{
				printf("%d : %s\n",i,IMG_Cartes[i]);
			}
					
		}
	}
}

void checkCartes(void)
{
	GtkLabel * labelCheck	= (GtkLabel *)gtk_builder_get_object(g_interface, "labelCheck");
	GtkLabel * labelTotal	= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsTotalV");
	GtkLabel * labelGains	= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsGainsV");

	gchar * sUtf8;
	int i=1, i2=1, iCarte=0, min=0, max=0, max2=0, gains=0, infoQuinte=0, infoCartes[6][4]={NULL}, infoCouleurs[5]={0};

	// Struct combinaison cartes
	struct combinaisonCartes{
		int paire;
		int doublePaire;
		int brelan;
		int quinte;
		int flush;
		int full;
		int carre;
		int quiteFlush;
		int quiteFlushRoyale;
	} combi;

	combi.paire				= FALSE;
	combi.doublePaire		= FALSE;
	combi.brelan			= FALSE;
	combi.quinte			= FALSE;
	combi.flush				= FALSE;
	combi.full				= FALSE; 
	combi.carre				= FALSE;
	combi.quiteFlush		= FALSE; 
	combi.quiteFlushRoyale  = FALSE;

	// détection numero et couleur
	for(i=1;i<=5;i++)
	{
		iCarte = i;
		if(FCartes[i] == 0)
		{
			iCarte = i + 5;
		}

		infoCartes[i][1] = PCartes[iCarte];
		infoCartes[i][2] = 1;
		infoCartes[i][3] = 1;

		while(infoCartes[i][1] > 13)
		{
			infoCartes[i][2]++;
			infoCartes[i][1] = infoCartes[i][1] - 13;
		}

		infoCouleurs[infoCartes[i][2]]++;
		if(infoCouleurs[infoCartes[i][2]] == 5)
		{
			combi.flush = TRUE;
		}
	}

	// detection double , triple , ...
	for(i=1;i<=5;i++)
	{
		for(i2=1;i2<=5;i2++)
		{
			if(i2 != i && infoCartes[i][1] == infoCartes[i2][1] && combi.paire != i2)
			{
				infoCartes[i][3]++;
			}
		}

		switch(infoCartes[i][3])
		{
			case 4:
				combi.carre = TRUE;
				break;
			case 3:
				combi.brelan = TRUE;
				if(combi.paire == TRUE)
				{
					combi.full = TRUE;
				}
				break;
			case 2:
				if(combi.paire > 0)
				{
					combi.paire = i;
					combi.doublePaire = TRUE;
				}
				else
				{
					combi.paire = i;
					if(combi.brelan == TRUE)
					{
						combi.full = TRUE;
					}
				}
				break;
			default:
				break;
		}
	}

	if(combi.paire > 0)
	{
		combi.paire = TRUE;
	}

	// detection quinte
	if(combi.paire == FALSE && combi.doublePaire == FALSE && combi.brelan == FALSE && combi.carre == FALSE)
	{		
		// detection du plus petit numero
		min = 1;
		max = 1;

		for(i=2;i<=5;i++)
		{
			if(infoCartes[i][1] < infoCartes[min][1])
			{
				min = i;
			}
			else if(infoCartes[i][1] > infoCartes[max][1])
			{
				max = i;
			}
		}

		if(infoCartes[min][1] == 1 && infoCartes[max][1] == 13)
		{
			infoCartes[min][1] = 14;
			max2 = min;
		}

		for(i=1;i<=5;i++)
		{
			if(infoCartes[i][1] == (infoCartes[max2][1] - 1))
			{
				max2 = i;
				i=1;
				infoQuinte++;						
			}
		}

		if(infoQuinte == 4)
		{
			if(min == 1 && max == 13 && combi.flush == TRUE)
			{
				combi.quiteFlushRoyale = TRUE;
			}
			else if(combi.flush == TRUE)
			{
				combi.quiteFlush = TRUE;
			}
			else
			{
				combi.quinte = TRUE;
			}
		}
	}

	if(combi.quiteFlushRoyale == TRUE){
		gains = Mise * 21;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez une\nquinte flush royale !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez une quinte flush royale !");
		}
	}
	else if(combi.quiteFlush == TRUE){
		gains = Mise * 11;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez une\nquinte flush !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez une quinte flush !");
		}
	}
	else if(combi.carre == TRUE){
		gains = Mise * 9;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez un\ncarré !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez un carre !");
		}
	}
	else if(combi.full == TRUE){
		gains = Mise * 6;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez un\nfull !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez un full !");
		}
	}
	else if(combi.flush == TRUE){
		gains = Mise * 5;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez une\ncouleur !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez une couleur !");
		}
	}
	else if(combi.quinte == TRUE){
		gains = Mise * 4;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez une\nquinte !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez une quinte !");
		}
	}
	else if(combi.brelan == TRUE){
		gains = Mise * 3;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez un\nbrelan !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez un brelan !");
		}
	}
	else if(combi.doublePaire == TRUE){
		gains = Mise * 2;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez une\ndouble paire !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez une double paire !");
		}
	}
	else if(combi.paire == TRUE){
		gains = Mise * 1;
		Total = Total + gains;
		sUtf8 = g_locale_to_utf8("Bien joué ! Vous avez une\npaire !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nBien joue ! Vous avez une paire !");
		}
	}
	else{
		sUtf8 = g_locale_to_utf8("Désolé ! Vous n'avez rien !", -1, NULL, NULL, NULL);
		gtk_label_set_text(labelCheck, sUtf8);
		g_free(sUtf8);
		if(AUTH_CONSOLE){
			printf("\n\nDesole ! Vous n'avez rien !");
		}
	}

	gtk_label_set_text(labelGains,ToString(gains));
	gtk_label_set_text(labelTotal,ToString(Total));
}