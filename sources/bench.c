#include "include.h"

// MINIMUM 0 - MAXIMUM 1000 
int NBR_THREADS_CALC = 1;
int NBR_THREADS_VIEW = 0;
int NBR_THREADS_CHECK = 0;
int SYNC_THREADS_CV = 0;
int SYNC_THREADS_CS = 0;

// Mutex, verrou zone critique pour multithread
static pthread_mutex_t mutex_stock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_stock_view = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_stock_check = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_stock_stats = PTHREAD_MUTEX_INITIALIZER;

// condition de temporisation pour le multitread
pthread_cond_t cond_stock = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_stock_go = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_stock_view = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_stock_view_sync = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_stock_check = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_stock_check_sync = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_stock_end = PTHREAD_COND_INITIALIZER;

GtkWidget * lab_StatsPos[6][53];
int count_lab_StatsPos[6][53] = {0};

struct BenchTime
{
	int start;
	int end;
	int last;
	int actu;
} BTime;

struct BenchCount
{
	int result;
	int Trie_new;
	int Trie_last;
	int View_Trie;
} BCount;

int MODE_MONO_T = 0;
int tab_VstatsPC = 0;

int T_GO = FALSE;
int ST_End = 0;
int ST_EndView = 0;
int Count_Trie = 0;
int Count_ViewTrie = -1;
int Count_CheckTrie = -1;

//static volatile char result_char[100] = "0";


pthread_t ThreadBench_Launch;
pthread_attr_t ThreadBench_Attrib_Launch;

pthread_t ThreadViewCarte[1000];
pthread_attr_t ThreadViewCarte_Attrib[1000];

pthread_t ThreadBench[1000];
pthread_attr_t ThreadBench_Attrib[1000];

pthread_t ThreadStats;
pthread_attr_t ThreadStats_Attrib;

pthread_t ThreadCheck[1000];
pthread_attr_t ThreadCheck_Attrib[1000];

// Sauvegarde Action BTN ( 0=Stop; 1=Start; 2=LOADING );
int btn_action = 1;

G_MODULE_EXPORT void on_btn_benchRand_Start_clicked(GtkButton * Bstart, gpointer s_data)
{
		GtkAdjustment  * Adjuste_nbr_tm   = (GtkAdjustment *) gtk_builder_get_object (g_interface, "adjuste_nbr_tm");
		GtkAdjustment  * Adjuste_nbr_tv	  = (GtkAdjustment *) gtk_builder_get_object (g_interface, "adjuste_nbr_tv");
		GtkAdjustment  * Adjuste_nbr_tc	  = (GtkAdjustment *) gtk_builder_get_object (g_interface, "adjuste_nbr_tc");
		

		GtkRadioButton * RB_sync_no = (GtkRadioButton *) gtk_builder_get_object (g_interface, "rb_sync_no");
		GtkRadioButton * RB_sync_mv = (GtkRadioButton *) gtk_builder_get_object (g_interface, "rb_sync_mv");
		GtkRadioButton * RB_sync_mc = (GtkRadioButton *) gtk_builder_get_object (g_interface, "rb_sync_mc");
		
		if(Adjuste_nbr_tm->value == 0)
		{
			NBR_THREADS_CALC = 1;
		}
		else
		{
			NBR_THREADS_CALC = Adjuste_nbr_tm->value;
		}

		NBR_THREADS_VIEW = Adjuste_nbr_tv->value;	
		NBR_THREADS_CHECK = Adjuste_nbr_tc->value;

		if(RB_sync_no->check_button.toggle_button.active == 1)
		{
			SYNC_THREADS_CV = 0;
			SYNC_THREADS_CS = 0;
		}
		else if(RB_sync_mv->check_button.toggle_button.active == 1)
		{
			SYNC_THREADS_CV = 1;
			SYNC_THREADS_CS = 0;
		}
		else if(RB_sync_mc->check_button.toggle_button.active == 1)
		{
			SYNC_THREADS_CV = 0;
			SYNC_THREADS_CS = 1;
		}

		if(btn_action == 1)
		{	
			btn_action = 2;

			// Spécial pour les mono threads ( qui ne doivent pas attendre de signal de synchro )
			if(NBR_THREADS_CALC == 1 )
			{
				MODE_MONO_T = 1;
			}
			else
			{
				MODE_MONO_T = 0;
			}

			//system("CLS");

			//gdk_threads_enter();
				gtk_button_set_label(Bstart,"...");
			//gdk_threads_leave();

			pthread_create(&ThreadBench_Launch, NULL, Blaunch, NULL);		
		}
		else if(btn_action == 0)
		{
			btn_action = 2;
			BTime.end = time(NULL);
			ST_End = BTime.end;

			printf("\n\nAttente de dechargement des Threads Melange ...\n\n");

			// on envoi un signal de deblocage a tous les threads synchronisé

			pthread_cond_broadcast (&cond_stock);
			pthread_cond_broadcast (&cond_stock_view);
			pthread_cond_broadcast (&cond_stock_check);

			if(SYNC_THREADS_CV == 1 && NBR_THREADS_VIEW > 0)
			{
				pthread_cond_broadcast (&cond_stock_view_sync);
			}
			if(SYNC_THREADS_CS == 1 && NBR_THREADS_CHECK > 0)
			{
				pthread_cond_broadcast (&cond_stock_check_sync);
			}

			//gdk_threads_enter();
				gtk_button_set_label(Bstart,"...");
			//gdk_threads_leave();
			
		}
}

void *Blaunch(void *args)
{
	int i = 0;
	int count_threadCalc = 0;
	int error_threadCalc = 0;

	int count_threadView = 0;
	int error_threadView = 0;

	int count_threadCheck = 0;
	int error_threadCheck = 0;

	GtkEntry  * Text_TS  = (GtkEntry *) gtk_builder_get_object (g_interface, "entry_ts");
	GtkEntry  * Text_VTS = (GtkEntry *) gtk_builder_get_object (g_interface, "entry_vts");
	GtkEntry  * Text_CC = (GtkEntry *) gtk_builder_get_object (g_interface, "entry_cc");
	GtkButton * Bstart   = (GtkButton *)gtk_builder_get_object (g_interface, "btn_benchRand_Start");

	ST_End = 0;
	ST_EndView = 0;
	Count_Trie = 0;
	Count_ViewTrie = -1;
	Count_CheckTrie = -1;
	BTime.last = 0;
	BTime.actu = 0;
	BTime.start = time(NULL);	
	T_GO = FALSE;

	printf("\n\nChargement des Threads de Calcul :\n\n");
	
	// Création des Threads de Calcul
	for(i=0;i<NBR_THREADS_CALC;i++)
	{	
		//pthread_attr_setschedpolicy(&ThreadBench_Attrib[i], SCHED_FIFO);
		if(pthread_create(&ThreadBench[i], NULL, BenchStart, NULL) == 0)
		{
			count_threadCalc++;
			printf("Thread Melange ID:%d - STATUS: OK\n",i);
		}
		else
		{
			
			error_threadCalc++;
			ST_End = time(NULL);
			printf("Thread Melange ID:%d - STATUS: ERREUR\n\n",i);		
			system("PAUSE");
			break;
		}

		if(error_threadCalc == 0)
		{
			printf("\n\nChargement des Threads de Melange : OK\n\n");
		}
		else
		{
			printf("\n\nChargement des Threads de Melange : %d ERREUR(S)\n\n",error_threadCalc);
		}
	}

	//system("CLS");

	if(error_threadCalc == 0 && NBR_THREADS_VIEW > 0)
	{

		printf("\n\nChargement des Threads View : ...\n\n");

		// Création des Threads de VU
		for(i=0;i<NBR_THREADS_VIEW;i++)
		{
			//pthread_attr_setschedpolicy(&ThreadViewCarte_Attrib[i], SCHED_FIFO);
			if(pthread_create(&ThreadViewCarte[i], &ThreadViewCarte_Attrib[i], ViewAllCartes, NULL) == 0)
			{
				count_threadView++;
				printf("Thread View ID:%d - STATUS: OK\n",i);
			}
			else
			{
				error_threadView++;
				ST_End = time(NULL);
				printf("Thread View ID:%d - STATUS: ERREUR\n\n",i);		
				system("PAUSE");
				break;
			}
		}

		if(error_threadView == 0)
		{
			printf("\n\nChargement des Threads View : OK\n\n");
		}
		else
		{
			printf("\n\nChargement des Threads View : %d ERREUR(S)\n\n",error_threadView);
		}

	}

	if(error_threadCalc == 0 && NBR_THREADS_CHECK > 0)
	{

		printf("\n\nChargement des Threads Check :\n\n");

		// Création des Threads de VU
		for(i=0;i<NBR_THREADS_CHECK;i++)
		{
			//pthread_attr_setschedpolicy(&ThreadCheck_Attrib[i], SCHED_FIFO);
			if(pthread_create(&ThreadCheck[i], &ThreadCheck_Attrib[i], StatsPosCarte, NULL) == 0)
			{
				count_threadCheck++;
				printf("Thread Check ID:%d - STATUS: OK\n",i);
			}
			else
			{
				error_threadCheck++;
				ST_End = time(NULL);
				printf("Thread Check ID:%d - STATUS: ERREUR\n\n",i);				
				system("PAUSE");
				break;
			}
		}

		if(error_threadCheck == 0)
		{
			printf("\n\nChargement des Threads Check : OK\n\n");
		}
		else
		{
			printf("\n\nChargement des Threads Check : %d ERREUR(S)\n\n",error_threadCheck);
		}

	}

	// Création du Thread de Stats
	pthread_create(&ThreadStats, NULL, BenchStats, NULL);

	btn_action = 0;
	//gdk_threads_enter();
		gtk_button_set_label(Bstart,"Stop");
	//gdk_threads_leave();
	
	// Attente de la Fin de chaque Threads de Calcul
	for(i=0;i<count_threadCalc;i++)
	{
		pthread_join(ThreadBench[i], NULL);
		if(i%2)
		{
			printf(" | Thread Melange ID:%d : TERMINER\n",i);
		}
		else
		{
			printf("Thread Melange ID:%d : TERMINER",i);
		}
	}

	printf("\n\nFIN Dechargement Threads Melange");

	// on envoi autent un signal a TOUS les threads qui attente ce signal pour enlever les blocage
	if(count_threadView > 0)
	{
		printf("\n\nAttente de dechargement des Threads View ...\n\n");
		
		pthread_cond_broadcast (&cond_stock_view);

		// Attente de la Fin de chaque Threads de VU
		for(i=0;i<count_threadView;i++)
		{
			pthread_join(ThreadViewCarte[i], NULL);
			if(i%2)
			{
				printf(" | Thread View ID:%d : TERMINER\n",i);			
			}
			else
			{
				printf("Thread View ID:%d : TERMINER",i);
			}
		}

		printf("\n\nFIN Dechargement Threads View");
	}

	

	if(count_threadCheck > 0)
	{
		printf("\n\nAttente de dechargement des Threads Check ...\n\n");

		pthread_cond_broadcast (&cond_stock_check);
	
		// Attente de la Fin de chaque Threads de VU
		for(i=0;i<count_threadCheck;i++)
		{
			pthread_join(ThreadCheck[i], NULL);
			if(i%2)
			{
				printf(" | Thread Check ID:%d : TERMINER\n",i);			
			}
			else
			{
				printf("Thread Check ID:%d : TERMINER",i);
			}
		}

		printf("\n\nFIN Dechargement Threads Check\n\n");
	}

	// On refait un tour d''aficahe des carte pour etre sur d'afficher les bonnes cartes
	//for(i=1;i<=5;i++)
	//{
	//	ViewCarte(i);
	//}

	printf("\n\nThread Stats : ");
	pthread_join(ThreadStats, NULL);
	printf("TERMINER\n\n");

	btn_action = 1;
	BTime.last = 0;
	////gdk_threads_enter();
		gtk_entry_set_text(Text_TS,"0");
		gtk_entry_set_text(Text_VTS,"0");
		gtk_entry_set_text(Text_CC,"0");
		gtk_button_set_label(Bstart,"Start");
	////gdk_threads_leave();
	
	return NULL;
}

void *BenchStart(void *args)
{
	
	while(1)
	{
		
		//verrouile une zone de ressource critique
		pthread_mutex_lock(&mutex_stock);

			// attente du signal de départ
			if(T_GO == FALSE)
			{
				printf("Attente 'cond_stock_go' ...\n");
				pthread_cond_wait (&cond_stock_go, &mutex_stock);
				printf("Attente 'cond_stock_go' ... OK\n");
				T_GO = TRUE;
			}		
			else if(MODE_MONO_T == 0 && ST_End == 0)
			{
				printf("Attente 'cond_stock' ...\n");
				pthread_cond_wait (&cond_stock, &mutex_stock);
				printf("Attente 'cond_stock' ... OK\n");
			}

			// Synchronise les Threads de Calcul et les Threads de VU
			if(SYNC_THREADS_CV == 1 && NBR_THREADS_VIEW > 0 && ST_End == 0 && Count_ViewTrie != -1)
			{					
				printf("Attente 'cond_stock_view_sync' ...\n");
				pthread_cond_wait (&cond_stock_view_sync, &mutex_stock);
				printf("Attente 'cond_stock_view_sync' ... OK\n");
			}

			// Synchronise les Threads de Calcul et les Threads de Check
			if(SYNC_THREADS_CS == 1 && NBR_THREADS_CHECK > 0 && ST_End == 0 && Count_CheckTrie != -1)
			{					
				printf("Attente 'cond_stock_check_sync' ...\n");
				pthread_cond_wait (&cond_stock_check_sync, &mutex_stock);
				printf("Attente 'cond_stock_check_sync' ... OK\n");
			}
			
			if(ST_End == 0)
			{
			
				MelCarte();

				//verrouile une zone de ressource critique
				pthread_mutex_lock(&mutex_stock_stats);
					Count_Trie++;
				pthread_mutex_unlock(&mutex_stock_stats);

				// Envoi un signal a StatsPosCarte pour lui dire qu'un nouveau trie à été fait
				if(NBR_THREADS_CHECK > 0)
				{
					printf("Envoi Signal 'cond_stock_check'\n");
					pthread_cond_signal (&cond_stock_check);
				}

				if(NBR_THREADS_VIEW > 0)
				{
					// Envoi un signal a ViewAllCartes pour lui dire qu'un nouveau trie à été fait
					printf("Envoi Signal 'cond_stock_view'\n");
					pthread_cond_signal (&cond_stock_view);
				}
			}
			
			if(MODE_MONO_T == 0)
			{
				// envoi du signal de syncro MultiThread
				printf("Envoi Signal 'cond_stock'\n");
				pthread_cond_signal (&cond_stock);
			}

		// libère la zone de ressource critique
		pthread_mutex_unlock(&mutex_stock);
		
		if(ST_End != 0)
		{
			break;
		}
	}

	return NULL;
}

void *ViewAllCartes(void *args)
{
	int i = 1;
	
	int *PCartes_Temp;
	
	while(1)
	{			
		//verrouile une zone de ressource critique
		pthread_mutex_lock(&mutex_stock_view);

			// attente du signal de syncronisation ( décharge du cpu )
			printf("Attente 'cond_stock_view' ...\n");
			pthread_cond_wait (&cond_stock_view, &mutex_stock_view);
			printf("Attente 'cond_stock_view' ... OK\n");

			PCartes_Temp = PCartes;
				
			//gdk_threads_enter();
				for(i=1;i<=5;i++)
				{
					ViewCarte(i);
				}
			//gdk_threads_leave();

			//verrouile une zone de ressource critique
			pthread_mutex_lock(&mutex_stock_stats);
				Count_ViewTrie++;
			pthread_mutex_unlock(&mutex_stock_stats);	
		
			if(SYNC_THREADS_CV == 1)
			{
				// signal de syco thread calcul et thread view;
				printf("Envoi Signal 'cond_stock_view_sync'\n");
				pthread_cond_signal (&cond_stock_view_sync);
			}		
			
		// libère la zone de ressource critique
		pthread_mutex_unlock(&mutex_stock_view);

		if(ST_End != 0)
		{
			break;
		}
	}

	return NULL;
}

void *StatsPosCarte(void *args)
{
	int i = 1;
	int ri = 0;
	
	int *PCartes_Temp;
	
	while(1)
	{
		//verrouile une zone de ressource critique
		pthread_mutex_lock(&mutex_stock_check);

			printf("Attente 'cond_stock_check' ...\n");
			pthread_cond_wait (&cond_stock_check, &mutex_stock_check);
			printf("Attente 'cond_stock_check' ... OK\n");

			if(ST_End == 0)
			{
				PCartes_Temp = PCartes;

				//gdk_threads_enter();

					for(ri=1;ri<6;ri++)
					{				
						gtk_label_set_text(GTK_LABEL(lab_StatsPos[ri][PCartes_Temp[ri]]),ToString(count_lab_StatsPos[ri][PCartes_Temp[ri]]++));
					}

				//gdk_threads_leave();

				//verrouile une zone de ressource critique
				pthread_mutex_lock(&mutex_stock_stats);
					Count_CheckTrie++;
				pthread_mutex_unlock(&mutex_stock_stats);
			}

			if(SYNC_THREADS_CS == 1)
			{
				// signal de syco thread calcul et thread view;
				printf("Envoi Signal 'cond_stock_check_sync'\n");
				pthread_cond_signal (&cond_stock_check_sync);
			}
			
		// libère la zone de ressource critique
		pthread_mutex_unlock(&mutex_stock_check);

		if(ST_End != 0)
		{
			break;
		}

	}

	return NULL;	
}

void *BenchStats(void *args)
{
	GtkEntry * Text_TS = (GtkEntry *) gtk_builder_get_object (g_interface, "entry_ts");
	GtkEntry * Text_VTS = (GtkEntry *) gtk_builder_get_object (g_interface, "entry_vts");
	GtkEntry * Text_CTS = (GtkEntry *) gtk_builder_get_object (g_interface, "entry_cc");
	
	GtkTable * tableBench = (GtkTable *) gtk_builder_get_object (g_interface, "tableBench");
	GtkTable * tablePosCarte = (GtkTable *) gtk_builder_get_object (g_interface, "tablePosCarte");

	char result_char[100] = "0";
	char result_char_view[100] = "0";
	char result_char_check[100] = "0";
	char carte_char[5] = "";
	char ri_char[2] = "";
	char pos_char[4] = "";
	
	int ri = 0;
	int ci = 0;

	// Remplissage (init) du Tableau de stats de position des cartes

	if(tab_VstatsPC == 0)
	{
		// Ajout dun label dans chaque case
		for(ri=0;ri<6;ri++)
		{
			for(ci=0;ci<53;ci++)
			{
				// on saute la première case
				if(!(ri == 0 && ci == 0))
				{
					if(ri > 0 && ci > 0)
					{
						lab_StatsPos[ri][ci] = gtk_label_new("0");
					}
					else
					{
						if(ci == 0)
						{
							
							itoa(ri,ri_char,10);
							strcpy(pos_char,"P");
							strcat(pos_char, ri_char);
							lab_StatsPos[ri][ci] = gtk_label_new(pos_char);
						}
						if(ri == 0)
						{
							//itoa(PCartes[ci],carte_char,10);
							lab_StatsPos[ri][ci] = gtk_label_new(IMG_Cartes[ci]);
						}
					}

					gtk_table_attach_defaults(GTK_TABLE(tablePosCarte),lab_StatsPos[ri][ci],ci,ci+1,ri,ri+1);

				}
			}
		}
		
		gtk_widget_show_all(GTK_WIDGET(tablePosCarte));
		tab_VstatsPC++;
	}

	//// envoi du signal de syncronisation MultiThread qui lance le BENCH
	//if(MODE_MONO_T == 0)
	//{
	//	pthread_cond_signal (&cond_stock);		
	//}
	printf("Envoi Signal 'cond_stock_go'\n");
	pthread_cond_broadcast(&cond_stock_go);
	//pthread_cond_signal (&cond_stock_go);

	while(1)
	{
		BTime.actu = time(NULL);

		if(BTime.actu != BTime.last)
		{
			//verrouile une zone de ressource critique
			pthread_mutex_lock(&mutex_stock_stats);

				itoa(Count_Trie,result_char,10);
				itoa(Count_ViewTrie,result_char_view,10);
				itoa(Count_CheckTrie,result_char_check,10);				
				
				//gdk_threads_enter();
					gtk_entry_set_text(Text_TS,result_char);				
					gtk_entry_set_text(Text_VTS,result_char_view);
					gtk_entry_set_text(Text_CTS,result_char_check);
				//gdk_threads_leave();
				
				if(BTime.last != 0)
				{
					Count_Trie = 0;
					Count_ViewTrie = 0;
					Count_CheckTrie = 0;
				}

			pthread_mutex_unlock(&mutex_stock_stats);
		}

		BTime.last = BTime.actu;

		if(ST_End != 0)
		{
			break;
		}
		else
		{
			Sleep(1001);
		}
	}

	return NULL;
}
