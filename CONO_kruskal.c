#include "CONO_kruskal.h"

//esta funcion devuelve la cola de prioridad ordenada
void inserta(int i, int j, int costo, Rama **arbol)
{
    Rama *nodo;
    nodo = (Rama*)malloc(sizeof(Rama));
    Rama *aux, *aux2;
    int x, y, z;

    if(*arbol == NULL)//primer elemento
    {
        nodo->a.u = i;
        nodo->a.v = j;
        nodo->a.costo = costo;
        nodo->sig = NULL;
        *arbol = nodo;
        return;
    }
    else
    {
        aux = *arbol;
        //segundo elemento en adelante
        while (aux->sig != NULL)
        {
            if(costo < aux->a.costo)
            {
                x = aux->a.u;
                y = aux->a.v;
                z = aux->a.costo; 
                aux->a.u = i;
                aux->a.v = j;
                aux->a.costo = costo;
                aux2 = aux->sig;
                aux->sig = nodo;
                nodo->sig = aux2;
                nodo->a.u = x;
                nodo->a.v = y;
                nodo->a.costo = z;
                return;
            }
            aux = aux->sig;
        }
        //ultimo elemento 
        if(aux->a.costo<costo)
        {
            aux->sig = nodo;
            nodo->sig = NULL;
            nodo->a.u = i;
            nodo->a.v = j;
            nodo->a.costo = costo;
            return;
        }
        else
        {
            x = aux->a.u;
            y = aux->a.v;
            z = aux->a.costo; 
            aux->a.u = i;
            aux->a.v = j;
            aux->a.costo = costo;
            aux->sig = nodo;
            nodo->sig = NULL;
            nodo->a.u = x;
            nodo->a.v = y;
            nodo->a.costo = z;
            return;   
        }
    }
}

//esta funcion modifica encabezados y nombres de elementos
void combina(tipo_nombre u, tipo_nombre v, Conjunto_CE *conjunto)
{
    int aux = 0;
    //verifico quien tiene la mayor cuenta (es la del elemento del conjunto que pertenece)
    if (conjunto->encabezamientos_conjunto[conjunto->nombres[u].nombre_conjunto].cuenta >= conjunto->encabezamientos_conjunto[conjunto->nombres[v].nombre_conjunto].cuenta)
    {
        conjunto->encabezamientos_conjunto[conjunto->nombres[u].nombre_conjunto].cuenta += conjunto->encabezamientos_conjunto[conjunto->nombres[v].nombre_conjunto].cuenta;
        if(conjunto->nombres[u].siguiente_elemento == -1)
            conjunto->nombres[u].siguiente_elemento = v;   
        conjunto->encabezamientos_conjunto[v].cuenta = -1;
        conjunto->encabezamientos_conjunto[v].primer_elemento = -1;
        aux = conjunto->nombres[v].nombre_conjunto;
        conjunto->nombres[v].nombre_conjunto = conjunto->nombres[u].nombre_conjunto;
        
        //actualizo los conjuntos de todos los elementos del conjunto reemplazado
        for (int i = 0; i < VERTICES; i++)
        {
            if (conjunto->nombres[i].nombre_conjunto == aux)
            {
                conjunto->nombres[i].nombre_conjunto = conjunto->nombres[u].nombre_conjunto;
            }   
        } 
    }
    else
    {
        conjunto->encabezamientos_conjunto[conjunto->nombres[v].nombre_conjunto].cuenta += conjunto->encabezamientos_conjunto[conjunto->nombres[u].nombre_conjunto].cuenta;
        if(conjunto->nombres[v].siguiente_elemento == -1)
            conjunto->nombres[v].siguiente_elemento = u;
        conjunto->encabezamientos_conjunto[u].cuenta = -1;
        conjunto->encabezamientos_conjunto[u].primer_elemento = -1;
        aux = conjunto->nombres[u].nombre_conjunto;
        conjunto->nombres[u].nombre_conjunto = conjunto->nombres[v].nombre_conjunto;

        //actualizo los conjuntos de todos los elementos del conjunto reemplazado
        for (int i = 0; i < VERTICES; i++)
        {
            if (conjunto->nombres[i].nombre_conjunto == aux)
            {
                conjunto->nombres[i].nombre_conjunto = conjunto->nombres[v].nombre_conjunto;
            }   
        } 
    }
    
}

//esta funciol devuelve el arbol recubridor
void kruskal(Rama *arbol)
{
    int cuenta_max = 0;
    Conjunto_CE *conjunto;
    conjunto = (Conjunto_CE*)malloc(sizeof(Conjunto_CE));
    Rama *aux;
    aux = arbol;
    //inicializo el conjunto
    for (int i = 0; i < VERTICES; i++)
    {
        conjunto->nombres[i].nombre_conjunto = i;
        conjunto->nombres[i].siguiente_elemento = -1;
        conjunto->encabezamientos_conjunto[i].cuenta = 1;    
        conjunto->encabezamientos_conjunto[i].primer_elemento = i;
    }
    
    while (arbol->sig != NULL)
    {   
        //verifico si los elemento pertenecen al mismo conjunto
        if (cuenta_max >= (VERTICES-1) || conjunto->nombres[arbol->a.u].nombre_conjunto == conjunto->nombres[arbol->a.v].nombre_conjunto)
        {
            arbol->a.u = arbol->sig->a.u;
            arbol->a.v = arbol->sig->a.v;
            arbol->a.costo = arbol->sig->a.costo;
            arbol->sig = arbol->sig->sig;
        }
        else //si no forma ciclo lo pongo en el arbol recubridor
        {
            combina(arbol->a.u, arbol->a.v, conjunto);
            arbol = arbol->sig;
            cuenta_max++;
        }
    }
    //verifico si el último elemento pertece o no al arbol recubridor
    if (cuenta_max >= (VERTICES-1) || conjunto->nombres[arbol->a.u].nombre_conjunto == conjunto->nombres[arbol->a.v].nombre_conjunto)
    {
        arbol = aux;
        while (arbol->sig != NULL)
        {
            if (arbol->sig->sig == NULL)
            {
                arbol->sig = NULL;
                break;
            }
            arbol = arbol->sig;
        }
    }
    else
    {
        combina(arbol->a.u, arbol->a.v, conjunto);
    }
    //Libero memoria
    free(conjunto);
    arbol = aux;
}

int main()
{
    int M_Costos [VERTICES][VERTICES], costo_minimo = 0;
    Rama *arbol, *aux;
    arbol = NULL;
    for (int i = 0; i <= VERTICES-1; i++)
    {
        for (int j = i+1; j <= VERTICES-1; j++)
        {
            printf("Ingrese costo de lado entre vertices %d y %d: ",i+1 ,j+1);
            scanf("%d", &M_Costos[i][j]);
        }
    }
    
    for (int i = 0; i <= VERTICES-1; i++) //la mitad inf. de diagonal de matriz
    {
        for (int j = i+1; j <= VERTICES-1; j++)
        {
            if (M_Costos[i][j] != 0)
            {
                if(M_Costos[i][j] > 0)
                {
                    inserta(i, j, M_Costos[i][j], &arbol); //inserto en la lista enlazada
                }
            }
        }
    }

    kruskal(arbol);
    aux = arbol;
    printf("-----KRUSKAL-----\n");
    while (aux != NULL)
    {
        printf("(%d-%d,%d)\n", aux->a.u+1, aux->a.v+1, aux->a.costo);
        costo_minimo += aux->a.costo;
        aux = aux->sig;
    }
    printf("Costo: %d", costo_minimo);

    //Libero la memoria
    while (arbol != NULL)
    {
        aux = arbol->sig;
        free(arbol);
        arbol = aux;
    }

    return 0;
}