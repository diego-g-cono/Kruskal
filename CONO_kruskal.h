#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VERTICES 6

typedef int tipo_nombre;
typedef int vertice;

typedef struct 
{
    vertice u;
    vertice v;
    int costo ;
} Arista;

typedef struct rama 
{
    Arista a;
    struct rama *sig;
} Rama;

typedef struct 
{
    int cuenta;
    int primer_elemento;
} Encabezado;

typedef struct
{
    tipo_nombre nombre_conjunto ;
    int siguiente_elemento ;
} Nombre;

typedef struct 
{
    Nombre nombres [VERTICES];
    Encabezado encabezamientos_conjunto [VERTICES];
} Conjunto_CE;

void combina(tipo_nombre, tipo_nombre, Conjunto_CE*);
void kruskal(Rama*);
void inserta(int, int, int, Rama**);