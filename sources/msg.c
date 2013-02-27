#include "include.h"

// Fonction d'affichage des messages d'erreur de type GError
void Msg_GError(GError * error)
{
	printf("\n###########################################\n\t\t! Erreur !\n###########################################\n\n");
	g_debug(error->message);
	printf("\n\n###########################################\n\n");
	
	system("PAUSE");
}

void Msg(char * Text)
{
	printf(Text);
}

char * ToString(int nbr)
{
	char * nbr_char = (char*)malloc(sizeof nbr * sizeof(char));

	if(nbr_char == NULL)
	{
		printf("\n\n FUNCTION 'ToString' ERREUR ALLOCATION MEMOIRE\n\n");
		system("PAUSE");
		return NULL;
	}
	else
	{
		itoa(nbr,nbr_char,10);	
		return nbr_char;
	}
	
}