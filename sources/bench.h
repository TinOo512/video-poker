#ifndef bench_h
#define bench_h

	G_MODULE_EXPORT void on_btn_benchRand_Start_clicked(GtkButton * Bstart, gpointer s_data);

	extern void *Blaunch(void *args);

	extern void *BenchStart(void *args);

	extern void *ViewAllCartes(void *args);

	extern void *StatsPosCarte(void *args);

	extern void *BenchStats(void *args);

#endif