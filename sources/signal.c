#include "include.h"

G_MODULE_EXPORT void onClick_Jouer(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	GtkWidget * vboxMenu   = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxMenu");
	GtkWidget * vboxRegles = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxRegles");
	GtkWidget * vboxResult = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxResult");

	gtk_widget_hide(vboxMenu);
	gtk_widget_hide(vboxResult);

	SetBG("Menu.png");
	gtk_widget_show(vboxRegles);
}

G_MODULE_EXPORT void on_buttonAnnuler_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	GtkWidget * vboxMenu   = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxMenu");
	GtkWidget * vboxRegles = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxRegles");

	gtk_widget_hide(vboxRegles);
	gtk_widget_show(vboxMenu);
}

G_MODULE_EXPORT void on_buttonValider_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	
	GtkWidget * boxTable	= (GtkWidget *)gtk_builder_get_object(g_interface, "boxTable");
	GtkWidget * boxMenu		= (GtkWidget *)gtk_builder_get_object(g_interface, "boxMenu");
	GtkWidget * vboxRegles	= (GtkWidget *)gtk_builder_get_object(g_interface, "vboxRegles");

	GtkWidget * labelError	= (GtkWidget *)gtk_builder_get_object(g_interface, "labelError");

	GtkButton * button		= (GtkButton *)gtk_builder_get_object(g_interface, "btnDistrib");
	GtkEntry  * entryJetons = (GtkEntry *)gtk_builder_get_object(g_interface, "entryReglesNbJ");
	GtkEntry  * entryParties= (GtkEntry *)gtk_builder_get_object(g_interface, "entryReglesNbP");	
	
	GtkLabel * labelCheck	= (GtkLabel *)gtk_builder_get_object(g_interface, "labelCheck");
	GtkLabel * labelMise	= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsMiseV");
	GtkLabel * labelTotal	= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsTotalV");
	GtkLabel * labelGains	= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsGainsV");
	GtkLabel * labelParties = (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsPartiesV");

	int labelPartiesTMP = 0;

	const gchar * gcharEntryJetons  = gtk_entry_get_text(entryJetons);
	const gchar * gcharEntryParties = gtk_entry_get_text(entryParties);

	char charlabelParties[10] = {NULL};

	Total	  = atoi(gcharEntryJetons);
	nbParties = atoi(gcharEntryParties);

	if(Total == 0 || nbParties == 0)
	{
		gtk_widget_show(labelError);
	}
	else
	{
		nbPartiesTotal = nbParties;
		Mise = 0;

		SetMise(Total);

		gtk_button_set_label(button, "Miser");
		gtk_label_set_text(labelCheck,"Choisissez votre mise puis \ncliquez sur \"Miser\".");
		gtk_label_set_text(labelGains,"0");
		gtk_label_set_text(labelMise,"0");
		gtk_label_set_text(labelTotal,ToString(Total));

		if(nbParties < 0)
		{
			labelPartiesTMP = 0;
			gtk_label_set_text(labelParties, ToString(labelPartiesTMP));
		}
		else
		{
			labelPartiesTMP = nbPartiesTotal-nbParties;
			sprintf(charlabelParties, "%d/%d", labelPartiesTMP, nbPartiesTotal);
			gtk_label_set_text(labelParties, charlabelParties);
		}

		
		gtk_widget_hide(vboxRegles);
		gtk_widget_hide(boxMenu);
		SetBG("Table.png");
		gtk_widget_show(boxTable);		

	}
}

G_MODULE_EXPORT void onClick_Aide(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	GtkWidget * winAide = (GtkWidget *)gtk_builder_get_object(g_interface, "winAide");
	
	gtk_widget_show_all(winAide);
	gtk_widget_set_size_request(winAide,615,670);
}

G_MODULE_EXPORT void onClick_Credits(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	GtkWidget * dialogCredits	= (GtkWidget *)gtk_builder_get_object(g_interface, "dialogCredits");
	
	gtk_widget_show_all(dialogCredits);
	gtk_widget_set_size_request(dialogCredits,260,225);
}

G_MODULE_EXPORT void onClick_Carte1(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	onClick_Carte(1);
}

G_MODULE_EXPORT void onClick_Carte2(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	onClick_Carte(2);
}

G_MODULE_EXPORT void onClick_Carte3(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	onClick_Carte(3);
}

G_MODULE_EXPORT void onClick_Carte4(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	onClick_Carte(4);
}

G_MODULE_EXPORT void onClick_Carte5(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	onClick_Carte(5);
}

void onClick_Carte(int pos)
{
	GThread   * T_SwitchCarte	= NULL;
	GError	  * err1			= NULL;
	GtkWidget * widget			= (GtkWidget *)gtk_builder_get_object(g_interface, ToString(pos));

	if(TourEtape == 2)
	{
		gtk_widget_set_sensitive(widget,FALSE);

		if(AUTH_CONSOLE)
		{
			printf("Click Images : %s\n",widget->name);
		}
		
		if( (T_SwitchCarte = g_thread_create((GThreadFunc)Thread_SwitchCarte, (void *)pos, TRUE, &err1)) == NULL)
		{
			 printf("Thread SwitchCarte Erreur : %s !!\n", err1->message);
			 g_error_free(err1);
		}

	}
}

G_MODULE_EXPORT void on_window_size_allocate(GtkWidget *widget, GdkEvent *event, gpointer s_data)
{
	GdkPixbuf * pixbuf= NULL;
	int pos=1, id_carte=0, posTemp=0;
	char file_carte[50];


	if( SizeWin[0] != widget->allocation.width || SizeWin[1] != widget->allocation.height)
	{
		
		SizeWin[0] = widget->allocation.width;
		SizeWin[1] = widget->allocation.height;
		SizeCarte[0] = (((widget->allocation.width * 10) / 800 * 70) / 10);
		SizeCarte[1] = (((widget->allocation.height * 10) / 600 * 105) / 10);

		if(BackMenu != NULL)
		{
			SetBG(BackMenu);
		}

		if(TourEtape == 2 || TourEtape == 3 )
		{			
			for(pos=1;pos<=5;pos++)
			{
				id_carte = PCartes[pos];
	
				if(FCartes[pos] == 0 && TourEtape == 2)
				{
					strcpy(file_carte,IMG_FOLDER);
					strcat(file_carte, "Back.png");
				}
				else
				{
					if(FCartes[pos] == 0 && TourEtape == 3)
					{
						posTemp = pos + 5;
						id_carte = PCartes[posTemp];
					}
					// créaction du chemin du fichié img de la carte
					strcpy(file_carte,IMG_FOLDER);
					strcat(file_carte, IMG_Cartes[id_carte]);
					strcat(file_carte, ".png");
				}
				
				pixbuf = gdk_pixbuf_new_from_file_at_scale(file_carte,SizeCarte[0],SizeCarte[1],TRUE,NULL);
				gtk_image_set_from_pixbuf(GTK_IMAGE(WidCartes[pos]),pixbuf);
				g_object_unref(pixbuf);
				gtk_widget_queue_draw(WidCartes[pos]);
			}
		}

		printf("X:%d:%d -- Y:%d:%d\n",widget->allocation.width, SizeCarte[0],widget->allocation.height,SizeCarte[1]);
		gtk_widget_queue_draw(widget);
	}
}

G_MODULE_EXPORT void on_btnDistrib_clicked(GtkButton * button, gpointer s_data)
{	
	
	GtkLabel* labelCheck = (GtkLabel *)gtk_builder_get_object(g_interface, "labelCheck");
	GtkLabel * labelParties = (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsPartiesV");
	GThread *T_LaunchViewCarte;
	GError  *err1 = NULL;
	const gchar * labelButton = NULL;
	int pos, labelTMP = 0;
	char charlabelParties[10] = {NULL};

	labelButton = gtk_button_get_label(button);

	switch(TourEtape)
	{
		case 1:

			GetMise();

			if(Mise > 0)
			{
				gtk_widget_set_sensitive(GTK_WIDGET(button),FALSE);
				TourEtape = 10;
				
				if(nbParties < 0){
					labelTMP = nbParties * -1;
					gtk_label_set_text(labelParties, ToString(labelTMP));
				}
				else{
					labelTMP = nbPartiesTotal-nbParties+1;
					sprintf(charlabelParties, "%d/%d", labelTMP, nbPartiesTotal);
					gtk_label_set_text(labelParties, charlabelParties);
				}

				// Melange des cartes
				MelCarte();	

				if( (T_LaunchViewCarte = g_thread_create((GThreadFunc)Thread_LaunchViewCarte, NULL, TRUE, &err1)) == NULL)
				{
					printf("Thread LaunchViewCarte Erreur : %s !!\n", err1->message);
					g_error_free(err1);
				}
			}

			break;
		case 2:

			gtk_widget_set_sensitive(GTK_WIDGET(button),FALSE);
			TourEtape = 20;
			cFace = 0;

			for(pos=1;pos<6;pos++)
			{
				if (FCartes[pos] == 1)
				{
					cFace++;
				}
			}

			if(cFace == 5)
			{
				printf("checkCartes ...\n");
				checkCartes();
				gtk_button_set_label(button, "Rejouer");
				TourEtape = 3;
				gtk_widget_set_sensitive(GTK_WIDGET(button),TRUE);
			}
			else
			{
				if( (T_LaunchViewCarte = g_thread_create((GThreadFunc)Thread_LaunchViewCarte, NULL, TRUE, &err1)) == NULL)
				{
					printf("Thread LaunchViewCarte Erreur : %s !!\n", err1->message);
					g_error_free(err1);
				}
			}

			break;
		case 3:
			gtk_widget_set_sensitive(GTK_WIDGET(button),FALSE);
			TourEtape = 30;
			Mise = 0;

			if( (T_LaunchViewCarte = g_thread_create((GThreadFunc)Thread_LaunchCacheCarte, NULL, TRUE, &err1)) == NULL)
			{
				printf("Thread LaunchCacheCarte Erreur : %s !!\n", err1->message);
				g_error_free(err1);
			}

			break;
		default:
			break;
	}
}

G_MODULE_EXPORT void on_adjusteMise_value_changed(GtkAdjustment * adjusteMise, gpointer s_data){
	GtkAdjustment * adjust = (GtkAdjustment *) gtk_builder_get_object (g_interface, "adjusteMise");
	gdouble dMise = Mise;

	if(Mise != 0){
		gtk_adjustment_set_value(adjust, dMise);
	}
}

G_MODULE_EXPORT void on_buttonMenu_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data){
	GtkWidget * boxTable	= (GtkWidget *)gtk_builder_get_object(g_interface, "boxTable");
	GtkWidget * boxMenu		= (GtkWidget *)gtk_builder_get_object(g_interface, "boxMenu");
	GtkWidget * vboxRegles	= (GtkWidget *)gtk_builder_get_object(g_interface, "vboxRegles");
	GtkWidget * vboxMenu	= (GtkWidget *)gtk_builder_get_object(g_interface, "vboxMenu");
	GtkWidget * eventbtnM4	= (GtkWidget *)gtk_builder_get_object(g_interface, "eventbtnM4");

	gtk_widget_hide(boxTable);
	gtk_widget_hide(vboxRegles);

	SetBG("Menu.png");

	if(nbParties != 0)
	{
		gtk_widget_show(eventbtnM4);
	}
	else
	{
		gtk_widget_hide(eventbtnM4);
	}

	gtk_widget_show(vboxMenu);
	gtk_widget_show(boxMenu);
}

G_MODULE_EXPORT void on_buttonAide_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data){
	GtkWidget * popAide = (GtkWidget *)gtk_builder_get_object(g_interface, "popAide");

	gtk_widget_show_all(popAide);
}

G_MODULE_EXPORT void on_annuler_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data){
	GtkWidget * dialogPopMenu = (GtkWidget *)gtk_builder_get_object(g_interface, "dialogPopMenu");

	gtk_widget_hide(dialogPopMenu);
}

G_MODULE_EXPORT void on_window_delete_event(GtkWidget *widget, GdkEventButton *event, gpointer s_data){
	GtkWidget * dialogPopMenu = (GtkWidget *)gtk_builder_get_object(g_interface, "dialogPopMenu");

	gtk_widget_show_all(dialogPopMenu);
}

G_MODULE_EXPORT void on_retour_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data){
	GtkWidget * boxTable = (GtkWidget *)gtk_builder_get_object(g_interface, "boxTable");
	GtkWidget * boxMenu	 = (GtkWidget *)gtk_builder_get_object(g_interface, "boxMenu");

	gtk_widget_hide(boxMenu);
	gtk_widget_show(boxTable);
}

G_MODULE_EXPORT void onClick_Retour(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	GtkWidget * boxMenu   = (GtkWidget *)gtk_builder_get_object(g_interface, "boxMenu");
	GtkWidget * boxTable   = (GtkWidget *)gtk_builder_get_object(g_interface, "boxTable");
	GtkWidget * vboxMenu   = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxMenu");	

	gtk_widget_hide(vboxMenu);
	gtk_widget_hide(boxMenu);
	SetBG("Table.png");
	gtk_widget_show(boxTable);
}

G_MODULE_EXPORT void on_window_delete(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	GtkWidget * dialogPopMenu = (GtkWidget *)gtk_builder_get_object(g_interface, "dialogPopMenu");
	GtkWidget * window		  = (GtkWidget *)gtk_builder_get_object(g_interface, "window");

	gtk_widget_show_all(dialogPopMenu);
	gtk_widget_show(window);
}

G_MODULE_EXPORT void on_btnQuitCredit_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	GtkWidget * dialogCredits = (GtkWidget *)gtk_builder_get_object(g_interface, "dialogCredits");
	
	gtk_widget_hide(dialogCredits);
}

G_MODULE_EXPORT void on_btnAideClose_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data)
{
	GtkWidget * winAide = (GtkWidget *)gtk_builder_get_object(g_interface, "winAide");
	
	gtk_widget_hide(winAide);
}