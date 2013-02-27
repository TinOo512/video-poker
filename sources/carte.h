#ifndef carte_h
#define carte_h

	extern void SwitchCarte(int pos);

	extern void *Thread_SwitchCarte(void *args);

	extern void *Thread_LaunchViewCarte(void * args);

	extern void ViewCarte2(int pos);

	extern void *Thread_ViewCarte(void * args);

	extern void *Thread_LaunchCacheCarte(void * args);

	extern void *Thread_CacheCarte(void * args);

	extern void CacheCarte2(int pos);

	extern void MelCarte(void);

	extern void AttribCarte(void);

	extern void checkCartes(void);

#endif