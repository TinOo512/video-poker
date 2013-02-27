#include "include.h"

void SetMise(gdouble max_mise)
{
	GtkAdjustment * adjust = (GtkAdjustment *) gtk_builder_get_object (g_interface, "adjusteMise");

	gtk_adjustment_set_upper(adjust, max_mise);
	//Sleep(10);
	gtk_adjustment_set_value(adjust, 0);
}

void GetMise(void)
{	GtkAdjustment * adjust		= (GtkAdjustment *) gtk_builder_get_object (g_interface, "adjusteMise");
	GtkLabel	  * labelMise	= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsMiseV");
	GtkLabel	  * labelTotal	= (GtkLabel *)gtk_builder_get_object(g_interface, "labStatsTotalV");

	Mise  = (int)gtk_adjustment_get_value(adjust);
	Total = Total - Mise;

	gtk_label_set_text(labelMise,ToString(Mise));
	gtk_label_set_text(labelTotal,ToString(Total));
}

int end(void){
	GtkWidget * boxTable   = (GtkWidget *)gtk_builder_get_object(g_interface, "boxTable");
	GtkWidget * boxMenu	   = (GtkWidget *)gtk_builder_get_object(g_interface, "boxMenu");

	GtkWidget * vboxMenu   = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxMenu");
	GtkWidget * vboxRegles = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxRegles");
	GtkWidget * vboxResult = (GtkWidget *)gtk_builder_get_object(g_interface, "vboxResult");

	GtkWidget * eventbtnM4 = (GtkWidget *)gtk_builder_get_object(g_interface, "eventbtnM4");

	if((nbPartiesTotal-nbParties+1) == nbPartiesTotal)
	{
		if((nbPartiesTotal-nbParties+1) == nbPartiesTotal && Total > 0)
		{
			//Game Win
			SetBG("gagne.png");
		}
		else
		{
			//Game Over
			SetBG("perdu.png");
		}

		gtk_widget_hide(eventbtnM4);
		gtk_widget_hide(vboxMenu);
		gtk_widget_hide(vboxRegles);
		gtk_widget_hide(boxTable);

		gtk_widget_show(vboxResult);
		gtk_widget_show(boxMenu);

		return TRUE;
	}
	else
	{
		if(Total == 0)
		{
			//Game Over
			SetBG("perdu.png");

			gtk_widget_hide(eventbtnM4);
			gtk_widget_hide(vboxMenu);
			gtk_widget_hide(vboxRegles);
			gtk_widget_hide(boxTable);

			gtk_widget_show(vboxResult);
			gtk_widget_show(boxMenu);
			
			return TRUE;
		}
	}

	return FALSE;
}

void SetBG(char * image)
{
	GdkPixbuf * pixbuf	 = NULL;
	GdkPixmap * pixmap	 = NULL;
	GtkStyle  * style	 = NULL; 
	GtkWidget * p_win	 = (GtkWidget *) gtk_builder_get_object (g_interface, "window");

	char * lien = (char*)calloc (sizeof IMG_FOLDER + sizeof image, sizeof(char));
	
	if(image != NULL)
	{
		strcpy(lien,IMG_FOLDER);
		strcat(lien,image);
	
		pixbuf = gdk_pixbuf_new_from_file_at_scale(lien,SizeWin[0],SizeWin[1],FALSE,NULL);
		gdk_pixbuf_render_pixmap_and_mask (pixbuf, &pixmap, NULL, 0); 
		g_object_unref(pixbuf);

		style = gtk_style_new(); 
		style->bg_pixmap[0] = pixmap;

		if(strcmp(BackMenu,image) != 0)
		{
			BackMenu = (char*)realloc(NULL, sizeof image * sizeof(char));
			strcpy(BackMenu,image);
		}

		gtk_widget_set_style (p_win, style);
	}
	else
	{
		BackMenu = NULL;
		gtk_widget_set_style(p_win, NULL);
	}
}