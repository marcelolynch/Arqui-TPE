#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getnum.h>
#include <2048Back.h>

int dosMilCuarentaYOcho()
{
        short_t**tablero;   /*Se crea un tablero sobre el que jugar */
        short_t**tableroAux;    /*y uno auxiliar para hacer undo */
        short_t dim,undo;
        int continua=1,puedeUndo=0,condicion;
        unsigned_t puntos=0,puntosAux;

        do{
            condicion=SIGUE;
            tablero=inicio(&dim,&undo,&puntos); /*Se inicializan todas las variables y el tablero*/
            if (tablero != NULL) {
                tableroAux=crearTablero(dim);
                imprimir(tablero,dim);
                printf("\nUndos restantes = %d",undo);
                printf("\nPuntuacion: %d\n",puntos);

                while(continua)
                {
                        continua=juega(tablero,tableroAux,dim,&undo,&puntos,&puntosAux,&puedeUndo,&condicion);
                        if(continua)
                        {
                            imprimir(tablero,dim);
                            printf("\nUndos restantes = %d",undo);
                            printf("\nPuntuacion: %d\n",puntos);
                        }
                }
                        comoSale(&condicion,tablero,tableroAux,dim,&puntos,&puedeUndo,&continua);
             }
          }while(condicion==GANO);
}

short_t** inicio(short_t* dim,short_t*undo,unsigned_t*puntos){
        int opcion;
        short_t** tablero = NULL;
        do{
            do{
                    printf("\n1- Nuevo Juego\n");
                    printf("2- Salir\n\n");
                    opcion=getint("");

            }while(opcion !=1 && opcion !=2 );

            if(opcion==1){

                    do{
                            printf("\nElija la dificultad:\n");
                            printf("1- Facil\n");
                            printf("2- Intermedio\n");
                            printf("3- Dificil\n");
                            opcion=getint("");
                    }while(opcion!=1 && opcion!=2 && opcion!=3);
                    switch(opcion)
                    {
                        case 1: *undo=8; break;
                        case 2: *undo=4; break;
                        case 3: *undo=2; break;
                    }
                    tablero = nuevoJuego(opcion, dim);
            }
            else {
                    if ( salirInicial() )
                        return NULL;
            }
       }while(tablero==NULL);
        return tablero;
}

void imprimir(short_t**tablero,short_t dim)
{
        int i,j;
        printf("\n%10c",' ');
        for(i=0; i<dim; i++){
                for(j=0; j<dim; j++){
                        if(tablero[i][j])
                            printf("   %d ",tablero[i][j]);
                        else
                            printf("    %c",'-');
                }
                printf("\n%10c",' ');
        }
}

int juega(short_t**tabin,short_t**tabaux,short_t dim,short_t*undo,unsigned_t*puntos,unsigned_t*puntosAux,int*puedeUndo,int*condicion)
    {
        int i,indicador=1,movio=0;
        static int direccion[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
        switch (i=pedirAccion()) {
          case 0:case 1:case 2:case 3:
                    movio=moverPiezas (tabin,tabaux,dim,puntos,puntosAux,direccion[i][0],direccion[i][1]);
                    *puedeUndo=1;
                    break;
            case 4:
                        indicador = salir(*puntos, *undo, tabin, dim);
                    break;
            case 5:
                    if (*puedeUndo){
                        if (deshacer(tabin,tabaux,dim,undo)){
                                  *puedeUndo=0;
                            *puntos = *puntosAux;
                        } else {
                            printf("\nSin 'undos' disponibles \n");
                        }
                    } else {
                        printf("\nNo se puede realizar dos 'undos' seguidos o en el primer movimiento \n");
                    }
                    break;
                }
              chequearMovio(tabin,dim,undo,condicion,&indicador,movio);
              return indicador;
    }

 int pedirAccion()
{
        char accion[5];
        int i,estado,j=7;
        printf("\nIngrese un movimiento (w a s d) \n");
        printf("'undo' para deshacer \n'quit' para salir\n");
        do{
        for(i=0,estado=1; i<5 && estado; i++)
        {
                accion[i]=getchar();
                putchar(accion[i]);
                if (accion[i] == '\n' || accion[i]==' ')
                {
                        estado=0;
                        i--;
                }
       }
       printf("\n");
        if (i==5)
                while (getchar() != '\n');

        if(i==1 || i==4)
                accion[i]='\0';

        if (accion[0]=='d' && accion[1]=='\0')
                j=0;
        else if(accion[0]=='a' && accion[1]=='\0')
                j=1;
        else if(accion[0]=='s' && accion[1]=='\0')
                j=2;
        else if(accion[0]=='w' && accion[1]=='\0')
                j=3;
        else if(accion[0]=='q' && accion[1]=='u' && accion[2]=='i' && accion[3]=='t' && accion[4]=='\0')
                j=4;
        else if(accion[0]=='u' && accion[1]=='n' && accion[2]=='d' && accion[3]=='o' && accion[4]=='\0')
                j=5;

        }while(j<0 || j>5);

        return j;
}

int comoSale(int*condicion,short_t**tablero,short_t**tableroAux,short_t dim,unsigned_t*puntos,int*puedeUndo,int*continua){
int sigue;
  if (*condicion == PERDIO)
      printf ("Muy mal, perdiste!\n\n");
  else if (*condicion == GANO) {
      printf ("Muy bien, ganaste!\n\n");
      printf("¿Desea volver a jugar?\n");
      do{
        printf("1- Si\n");
        printf("2- No\n\n");
        sigue = getint("");
      }while(sigue!=1 && sigue!=2);

      if (sigue==2) {
        *condicion=SALIR;
      }
      else {
          resetear(tablero,tableroAux,dim,puntos,continua,puedeUndo);
        }
  }  else
      printf ("Fin del juego\n");

  }

int salirInicial()
{
        int opcion;
        do
        {
                printf("\nEsta seguro que desea salir?\n");
                printf("1- Si\n");
                printf("2- No\n");
                opcion = getint("");
        }while (opcion!=1 && opcion!=2);

        return (opcion==1)?1:0;
 }

int salir(unsigned_t puntos, short_t undo, short_t** tablero, short_t dim)
{
        return 0;
 }

