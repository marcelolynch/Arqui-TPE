#ifndef _2048BACK_H_INCLUDED
#define _2048BACK_H_INCLUDED

enum {PERDIO=0,GANO,SIGUE,NOJUGO,SALIR};

#define FACIL 8
#define INTERMEDIO 6
#define DIFICIL 4
#define LIBRE 0
#define BLOQUE 4

#define RAND_MAX 32767

#define GANADOR(dim) ((dim==8)?1024:2048)
#define randInt(izq, der) (int)((rand() / ( (double) RAND_MAX + 1)) * (der - izq + 1) + izq)
#define Rango(a,b) a>=0 && a<dim && b>=0 && b<dim

typedef unsigned int unsigned_t;
typedef unsigned short int short_t;

short_t** inicio(short_t* dim,short_t*undo,unsigned_t*puntos);

short_t** nuevoJuego(short_t dif, short_t*dim);

short_t** cargarJuego(short_t*dim, short_t*undo, unsigned_t*puntos);

short_t** crearTablero(short_t dim);

void generarNum(short_t** tablero,short_t dim);

void imprimir(short_t**tablero,short_t dim);

int moverPiezas(short_t**tablero,short_t**tabaux,short_t dim,unsigned_t*puntos,unsigned_t*puntosAux,int y, int x);

void copiar(short_t**tabin, short_t**tabout,short_t dim);

int juega(short_t**tabin,short_t**tabaux,short_t dim,short_t*undo,unsigned_t*puntos,unsigned_t*puntosAux,int*puedeUndo,int*condicion);

int pedirAccion();

int deshacer(short_t**tabin,short_t**tabaux,short_t dim,short_t*undo);

int salir(unsigned_t puntos, short_t undo, short_t** tablero, short_t dim);

int salirInicial();

int chequearAlrededores (short_t **matriz, short_t dim, int i, int j);

int chequearTablero (short_t** matriz,short_t dim,short_t undo);

void guardar(unsigned_t puntos, short_t undo, short_t** tablero, short_t dim);

int validar (short_t**tablero,int i,int j,int x,int y,short_t dim);

void backup (short_t**tablero,short_t**tabaux,short_t dim,unsigned_t*puntos,unsigned_t*puntosAux,int flagMovio);

void resetear(short_t**matA,short_t**matB,short_t dim, unsigned_t*puntos, int*continua, int*puedeUndo);

int comoSale(int*condicion,short_t**matA,short_t**matB,short_t dim,unsigned_t*puntos,int*puedeUndo,int*continua);

void chequearMovio(short_t**tabin,short_t dim,short_t*undo,int*condicion,int*indicador,int movio);

int checkNombre(char nombre[]);

#endif

