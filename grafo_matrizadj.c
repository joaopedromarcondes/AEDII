#include "grafo_matrizadj.h"
#include <stdio.h>
#include <stdlib.h>

bool inicializaGrafo(Grafo* grafo, int nv) {
    if (nv <= 0 || grafo == NULL || nv >= MAXNUMVERTICES) {
        fprintf(stderr, "ERRO na chamada do inicializaGrafo, Numero de vertices maior que o maximo ou grafo == NULL");
        return false;
    }
    grafo->numVertices = nv;
    grafo->numArestas = 0;
    int i, j;
    for (i = 0; i < nv; i++) {
        for (j = 0; j < nv; j++) {
            grafo->mat[i][j] = ARESTA_NULA;
        }
    }
    return true;
}

int obtemNrVertices(Grafo* grafo) {
    return grafo->numVertices;
}

int obtemNrArestas(Grafo* grafo) {
    return grafo->numArestas;
}

bool verificaValidadeVertice(int v, Grafo *grafo) {
    if (v < 0 || v >= grafo->numVertices) {
        fprintf(stderr, "ERRO: Voce tentou acessar um vertice que nao existe");
        return false;
    }
    return true;
}

void insereAresta(int v1, int v2, Peso peso, Grafo *grafo) {
    if (verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)) {
        grafo->mat[v1][v2] = peso;
        grafo->numArestas++;
    }
}

bool existeAresta(int v1, int v2, Grafo *grafo) {
    if (verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)) {
        if (grafo->mat[v1][v2] != ARESTA_NULA) {
            return true;
        }
    }
    return false;
}

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo) {
    if (!existeAresta(v1, v2, grafo)) {
        return ARESTA_NULA;
    }
    return grafo->mat[v1][v2];
}

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo *grafo) {
    if (!existeAresta(v1, v2, grafo)) {
        return false;
    }
    peso = grafo->mat[v1][v2];
    grafo->mat[v1][v2] = ARESTA_NULA;
    grafo->numArestas--;
    return true;
} 

bool removeAresta(int v1, int v2, Grafo *grafo) {
    Peso *peso = 0;
    return removeArestaObtendoPeso(v1, v2, peso, grafo);
}

bool listaAdjVazia(int v, Grafo* grafo) {
    if (verificaValidadeVertice(v, grafo)) {
        int i;
        for (i=0; i<grafo->numVertices; i++) {
            if (grafo->mat[v][i] != ARESTA_NULA) return true;
        }
    }
    return false;
}

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual) {
    if (atual >= grafo->numVertices || atual < 0) atual = 0;
    if (verificaValidadeVertice(v, grafo)) {
        int i;
        for (i = atual; i < grafo->numVertices; i++) {
            if (grafo->mat[v][i] != ARESTA_NULA) {
                return i;
            }
        }
    }
    return VERTICE_INVALIDO;
}

Apontador primeiroListaAdj(int v, Grafo* grafo) {
    return proxListaAdj(v, grafo, -1);
}

int obtemVerticeDestino(Apontador p, Grafo* grafo) {
    if (p) {
        return p;
    }
    return (-1);
}

void imprimeGrafo(Grafo* grafo) {
    int i, j;
    for (i = 0; i < grafo->numVertices; i++) {
        for (j = 0; j < grafo->numVertices; j++) {
            printf("%d ", grafo->mat[i][j]);
        }
        printf("\n");
    }
}

void liberaGrafo(Grafo* grafo) {
    grafo->numVertices = 0;
    grafo->numArestas = 0;
}