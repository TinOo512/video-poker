#ifndef signal_h 
#define signal_h

#include "include.h"

	G_MODULE_EXPORT void onClick_Jouer(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_buttonAnnuler_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_buttonValider_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void onClick_Aide(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void onClick_Credits(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void onClick_Carte1(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void onClick_Carte2(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void onClick_Carte3(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void onClick_Carte4(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void onClick_Carte5(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	extern void onClick_Carte(int pos);

	G_MODULE_EXPORT void on_window_size_allocate(GtkWidget *widget, GdkEvent *event, gpointer s_data);

	G_MODULE_EXPORT void on_btnDistrib_clicked(GtkButton * button, gpointer s_data);

	G_MODULE_EXPORT void on_adjusteMise_value_changed(GtkAdjustment * adjusteMise, gpointer s_data);

	G_MODULE_EXPORT void on_buttonMenu_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_buttonAide_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_annuler_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_window_delete_event(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_retour_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void onClick_Retour(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_window_delete(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_btnQuitCredit_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

	G_MODULE_EXPORT void on_btnAideClose_clicked(GtkWidget *widget, GdkEventButton *event, gpointer s_data);

#endif