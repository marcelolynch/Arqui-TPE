#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getnum.h>
#include <2048Back.h>

short_t** nuevoJuego(short_t dif, short_t*dim)
{
        short_t** tablero;
        switch(dif){
                case 1: *dim=FACIL; break;
                case 2: *dim=INTERMEDIO; break;
                case 3: *dim=DIFICIL; break;
        }
        tablero = crearTablero(*dim);
        generarNum(tablero,*dim);
        generarNum(tablero,*dim);
        return tablero;
}

short_t** crearTablero(short_t dim)
{
    short_t** tablero;
    int i;
    tablero=malloc(dim*sizeof(*tablero));
        if(tablero==NULL)
                return NULL;

        for(i=0; i<dim; i++){
                tablero[i]=malloc(dim*sizeof(**tablero));
                if(tablero[i]==NULL){
                        return NULL;
                }
        }
        return tablero;
}

/* Para generar un numero en el tablero se crea una lista en "vacios" que contiene
todas las posiciones libres del tablero. Las coordenadas x,y se anotan con un numero
de dos digitos de forma tal que dividiendo por 10 se obtiene una coordenada y aplicando
el modulo de 10 se obtiene la otra.
*/
void generarNum(short_t** tablero,short_t dim)
{

        int i,j,k=0;
        unsigned int pos;
        int* vacios=malloc(12*12);
        int* aux=malloc(12*12);
        
        for(i=0; i<dim; i++){
                for(j=0; j<dim; j++){
                        if(tablero[i][j]==LIBRE){
                                if(k%BLOQUE==0){
                                        aux = realloc(vacios,(k+BLOQUE)*sizeof(*vacios));
                                        if(aux==NULL){
                                                return;
                                        }
                                         vacios=aux;
                                }
                                vacios[k++]=i*10+j;
                        }
                }
        }
        pos = rand() % k;
        
        if(rand() % 100 <90)
          tablero[vacios[pos]/10][vacios[pos]%10]=2;
        else
          tablero[vacios[pos]/10][vacios[pos]%10]=4;
        free(vacios);
        return;
}

/* La funcion mover recibe en x,y la direccion en la que se moveran los numero.
Ej: x=0 y=1 mueve para abajo,   x=0 y=-1 mueve para arriba

Utilizamos las variables "inf" y "sup" para recorrer la matriz desde la posicion
superior izquierda hasta la inferior derecha cuando el movimiento es hacia abajo
o hacia la derecha. En caso de que el movimiento sea hacia arriba o hacia la izquierda
la matriz se recorre de forma inversa a la anterior.

En cada posicion se verifica si la posicion a la que se esta intentando mover esta
dentro de la matriz usando la macro Rango(). Si esto se cumple se verifica si se cumplen
las condiciones para avanzar una posicion o duplicarse. Si se cumple alguno de estos
casos ae realiza un ciclo para que todos los numeros que estan detras tambien avancen.
En caso de que no se cumplan no pasa nada.

Al duplicar una ficha se le suma 1 para evitar que una ficha se duplique mas de una
vez en un solo movimiento. Esto finalmente se corrige buscando todos los numeros
impares del tablero y restandoles 1.
*/
int moverPiezas(short_t**tablero,short_t**tabaux,short_t dim,unsigned_t*puntos,unsigned_t*puntosAux,int y, int x)
{
    int i,j,n,m,inf,sup,indice,flagMovio=0;
    if(x>0 || y>0){
        inf = 0; sup = dim; indice = 1;
    }
    else{
        sup = 0-1; inf = dim-1; indice = -1;
    }
    for(i=inf;i!=sup;i+=indice)
    {
        for(j=inf;j!=sup;j+=indice)
        {
            if(Rango(i+x,j+y))
            {
                if(validar(tablero,i,j,x,y,dim)) /*Analiza si se puede duplicar */
                {
                    backup(tablero,tabaux,dim,puntos,puntosAux,flagMovio);
                    *puntos += tablero[i+x][j+y] = 2*tablero[i+x][j+y] + 1;
                    (*puntos)--;
                    n=i-x;
                    m=j-y;
                    flagMovio=1;
                }
                else if(tablero[i][j]!=LIBRE && tablero[i+x][j+y] == LIBRE)/*Analiza si se puede mover */
                {
                    backup(tablero,tabaux,dim,puntos,puntosAux,flagMovio);
                    n=i;
                    m=j;
                    flagMovio=1;
                }
                else /*Si llega hasta aca es porque no puede hacer nada */
                    n = m = -5;
                while(Rango(n,m)) /*Arrastra los numeros que estan detras luego de una accion */
                {
                    tablero[n+x][m+y]=tablero[n][m];
                    n-=x;
                    m-=y;
                }
                if(n!=-5) /*Llena el espacio de los bordes con un cero */
                    tablero[n+x][m+y]=LIBRE;
            }
        }
    }
    for(i=0;i<dim;i++) /*Corrige los numero impares */
        for(j=0;j<dim;j++)
            if(tablero[i][j]%2!=0)
                tablero[i][j]--;
    return flagMovio;
}

/* Verifica que se cumplan las condiciones para duplicar
    1) Que la ficha de al lado sea igual.
    2) Que no haya dos fichas al lado iguales.
        Ej: 222 -> mueve hacia derecha -> 042 (se evita este error)
                                          024 (correcto)
    3) Que si hay 3 fichar consecutivas iguales se pueda duplicar.
        Ej: 2222 -> mueve hacia derecha -> 0224 (se evita este error producido por el chequeo nº2)
                                           0044 (correcto)
Estos chequeos funcionan con cualquier cantidad de ceros entre estos numeros.
*/
int validar (short_t**tablero,int i,int j,int x,int y,short_t dim)
{
    int n,m;
    if ( tablero[i][j]!=LIBRE && tablero[i+x][j+y] == tablero[i][j])
    {
        for(n=i+2*x, m=j+2*y; Rango(n,m) && tablero[n][m]==LIBRE; n+=x, m+=y)
            ;
        if(!(Rango(n,m)) || tablero[n][m]!=tablero[i][j])
            return 1;

        else if(Rango(n+x,m+y))
        {
            for(n+=x, m+=y; Rango(n,m) && tablero[n][m]==LIBRE; n+=x, m+=y)
                ;
            if(Rango(n,m) && tablero[n][m]==tablero[i][j])
                return 1;
        }
    }
        return 0;
}

/* Antes de realizar el primer movimiento se deben guardar los puntos y la matriz por si luego
se desea hacer un undo.
 */
void backup (short_t**tablero,short_t**tabaux,short_t dim,unsigned_t*puntos,unsigned_t*puntosAux,int flagMovio)
{
      if(flagMovio==0){
        copiar(tablero,tabaux,dim);
        *puntosAux=*puntos;
    }
}

void copiar(short_t**tabin, short_t**tabout,short_t dim)
{
    int i,j;
    for(i=0;i<dim;i++)
       for(j=0;j<dim;j++)
            tabout[i][j]=tabin[i][j];
}

/* Recorre el tablero de izquierda a derecha, de arriba abajo y se fija si es
posible hacer algun movimiento o si en alguna posicion hay algun GANADOR.
*/
int chequearTablero (short_t** tablero,short_t dim,short_t undo)
{
    int i,j,gano=0,k=0,salida;
    for (i=0; i<dim && !gano; i++) {
        for (j=0; j<dim && !gano; j++){
            if (tablero[i][j]==GANADOR(dim))
                gano=1;
            else if ( chequearAlrededores(tablero,dim,i,j) )
                k=1;
        }
    }
    if (gano)
        salida=GANO;
    else if (k == 0 && undo==0)
        salida=PERDIO;
    else salida=SIGUE;

    return salida;
}
/* Como recorremos la matriz de izq a der, arriba a abajo, esta funcion se fija
si se puede mover hacia abajo o hacia la derecha, de esta manera tambien
cubre los movimientos hacia arriba y hacia la izquierda de las posiciones.
*/
int chequearAlrededores (short_t **tablero, short_t dim, int i, int j)
{
    int puedeMover=0;
    if ( i<dim-1 && j<dim-1){
        if ( tablero[i+1][j]== LIBRE || tablero[i][j+1] == LIBRE || tablero[i+1][j]==tablero[i][j] || tablero[i][j+1]==tablero[i][j] )
            puedeMover=1;
    } else if (i==dim-1 && j<dim-1){
        if ( tablero[i][j+1] == LIBRE || tablero[i][j+1]==tablero[i][j] )
            puedeMover=1;
    } else if (i<dim-1 && j==dim-1) {
        if (tablero[i+1][j]== LIBRE || tablero[i+1][j]==tablero[i][j] )
            puedeMover=1;
    }
    return puedeMover;
}

/* Al finalizar una partida se deben resetear las variables y liberar la memoria utilizada*/
void resetear(short_t**matA,short_t**matB,short_t dim, unsigned_t*puntos, int*continua, int*puedeUndo){
    int i;
    for (i=0;i<dim;i++){
      free(matA[i]);
      free(matB[i]);
    }
    free(matA);
    free(matB);
    *puntos=*puedeUndo=0;
    *continua=1;
}

void chequearMovio(short_t**tabin,short_t dim,short_t*undo,int*condicion,int*indicador,int movio)
{
    if (movio) {
        generarNum(tabin,dim);
        switch (chequearTablero(tabin,dim,*undo)) {
          case GANO:   *condicion=GANO;
                       *indicador=0;
                       break;
          case PERDIO: *condicion=PERDIO;
                       *indicador=0;
                       break;
          case SIGUE: *condicion=SIGUE;
                      break;
        }
    } else {
    *condicion=NOJUGO;
    }
}

int checkNombre(char nombre[])
{
    static char* reservadas[3] = {"2048","make","getn"};
    int i,flag=1;

    for(i=0; i<3; i++)
        if(strncmp(nombre,reservadas[i],4) == 0)
            flag=0;

    return flag;
}

int deshacer(short_t**tabin,short_t**tabaux,short_t dim,short_t*undo)
{
        if(*undo>0){
                *undo = *undo-1;
                copiar(tabaux,tabin,dim);
                return 1;
        }
        return 0;
}

