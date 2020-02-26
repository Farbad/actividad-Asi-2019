
#include <ctype.h>
#include "utils.h"

/************************* MODULO DE FUNCIONES DE INTERES *****************/

/*---------------------- Pasar a mayusculas una cadena -------------------*/

char *toupper_str(char *mn)
{
register int i;
	for(i=0;mn[i];i++)
		mn[i] = toupper(mn[i]);
	return(mn);
}

/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/

int get_key()
{
char buf[256];
int opt;

	fgets(buf,sizeof(buf),stdin);
	printf("He recibido <%s>\n",buf);
	if(sscanf(buf,"%d",&opt) == 1)
		return(opt);
	return(-1);

}

/*------------------------------------------------------------------------*/

/**************************************************************************/

