#include "grafo_listaadj.h"
#include <stdio.h>
#include <stdlib.h>

bool inicializaGrafo(Grafo* grafo, int nv) {
    if (nv <= 0 || grafo == NULL) {
        fprintf(stderr, "ERRO na chamada do inicializaGrafo, Numero de vertices maior que o maximo ou grafo == NULL");
        return false;
    }
    grafo->numVertices = nv;
    grafo->numArestas = 0;
    if (!(grafo->listaAdj = (Aresta**) calloc(nv, sizeof(Aresta*)))) {
        fprintf(stderr, "ERRO na alocacao de memoria em InicializaGrafo");
        return false;
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
        grafo->numArestas++;
        Apontador novo = (Apontador) malloc(sizeof(Apontador));
        novo->peso = peso;
        novo->vdest = v2;
        novo->prox = grafo->listaAdj[v1];
        grafo->listaAdj[v1] = novo;
    }
}

bool existeAresta(int v1, int v2, Grafo *grafo) {
    if (verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)) {
        Apontador q = grafo->listaAdj[v1];
        while (q && q->vdest != v2) q = q->prox;
        if (q) return true;
    }
    return false;
}

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo) {
    if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo))) {
        return ARESTA_NULA;
    }
    Apontador q = grafo->listaAdj[v1];
    while (q) {
        if (q->vdest == v2) {
            return q->peso;
        }
    }
    return ARESTA_NULA;
}

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo *grafo) {
    if (!existeAresta(v1, v2, grafo)) {
        return false;
    }
    
    Apontador q = grafo->listaAdj[v1];
    Apontador ant = NULL;
    while (q) {
        if (q->vdest == v2) {
            break;
        }
        ant = q;
        q = q->prox;
    }
    if (q) {
        *peso = q->peso;
        if (ant) {
            ant->prox = q->prox;
        } else {
            grafo->listaAdj[v1] = q->prox;
        }
        free(q);
        q = NULL;
        return true;
    }
    return false;
} 

bool removeAresta(int v1, int v2, Grafo *grafo) {
    Peso *peso = 0;
    return removeArestaObtendoPeso(v1, v2, peso, grafo);
}

bool listaAdjVazia(int v, Grafo* grafo) {
    if (verificaValidadeVertice(v, grafo)) {
        return (grafo->listaAdj[v] == NULL);
    }
    return false;
}

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual) {
    if (verificaValidadeVertice(v, grafo)) {
        if (atual != NULL) {
            Apontador a = atual->prox;
            if (a)
                return (a);
        }
    }
    return VERTICE_INVALIDO;
}

Apontador primeiroListaAdj(int v, Grafo* grafo) {
    if (!verificaValidadeVertice(v, grafo)) return VERTICE_INVALIDO;
    return (grafo->listaAdj[v]);
}

int obtemVerticeDestino(Apontador p, Grafo* grafo) {
    if (p) {
        return p->vdest;
    }
    return (-1);
}

void imprimeGrafo(Grafo* grafo) {
    int i;
    Apontador q;
    for (i = 0; i < grafo->numVertices; i++) {
        q = grafo->listaAdj[i];
        while (q) {
            printf("%d %d %d\n", i, q->vdest, q->peso);
            q = q->prox;
        }
    }
}

void liberaGrafo(Grafo* grafo) {
    int i;
    Apontador p;

    for (i = 0; i < grafo->numVertices; i++) {
        while ((p = grafo->listaAdj[i])) {
            grafo->listaAdj[i] = p->prox;
            //p->prox = NULL;
            free(p);
        }
    }
    grafo->numVertices = 0;
    grafo->numArestas = 0;
    free(grafo->listaAdj);
    grafo->listaAdj = NULL;
}