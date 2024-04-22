#include <stdbool.h>

#define MAXNUMVERTICES 100
#define ARESTA_NULA -1
#define VERTICE_INVALIDO NULL
typedef int Peso;


typedef struct str_aresta {
    int vdest;
    Peso peso;
    struct str_aresta* prox;
} Aresta;

typedef Aresta* Apontador;


typedef struct {
    Aresta** listaAdj;
    int numVertices;
    int numArestas;
} Grafo;

bool inicializaGrafo(Grafo* grafo, int nv);

int obtemNrVertices(Grafo* grafo);

int obtemNrArestas(Grafo* grafo);

bool verificaValidadeVertice(int v, Grafo *grafo);

void insereAresta(int v1, int v2, Peso peso, Grafo *grafo);

bool existeAresta(int v1, int v2, Grafo *grafo);

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo);

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo *grafo);

bool removeAresta(int v1, int v2, Grafo *grafo);

bool listaAdjVazia(int v, Grafo* grafo);

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual);

Apontador primeiroListaAdj(int v, Grafo* grafo);

int obtemVerticeDestino(Apontador p, Grafo* grafo);

void imprimeGrafo(Grafo* grafo);

void liberaGrafo(Grafo* grafo);