#include <stdio.h>
#include "getnum.h"
#include <string.h>

#define BORRA_BUFFER while (getchar() != '\n')

/* lee un entero */
int 
getint(char mensaje[], ... )
{
	int n, salir = 0;
	va_list ap;
		
	do	
	{
		va_start(ap, mensaje);
		vprintf(mensaje, ap);
		va_end(ap);

		if ( scanf("%d",&n) != 1)
		{
			printf("\nDato incorrecto\n");
			BORRA_BUFFER;
		}
		else{
			salir = 1;
		}
	} while (! salir);

	return n;
}
