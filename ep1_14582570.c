#ifdef MATRIZ
#include "grafo_matrizadj.h"
#else
#include "grafo_listaadj.h"
#endif

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    BRANCO,
    CINZA,
    PRETO
} cor;

typedef struct NOaux {
    int vertice;
    struct NOaux* prox;
} NO;

typedef struct {
    NO* inicio;
    NO* fim;
} FILA;

void inicializarFila(FILA* f) {
    f->inicio = NULL;
    f->fim = NULL;
}

void adicionarFila(FILA* f, int vertice) {
    NO* novo = (NO*) malloc(sizeof(NO));
    novo->vertice = vertice;
    novo->prox = NULL;
    if (f->fim) {
        f->fim->prox = novo;
        f->fim = novo;
    } else {
        f->inicio = novo;
        f->fim = novo;
    }
}

int removerFila(FILA* f) {
    if (f->inicio) {
        int valor = f->inicio->vertice;
        NO* aux = f->inicio;
        f->inicio = f->inicio->prox;
        if (!f->inicio) f->fim = NULL;
        free(aux);
        return valor;
    }
    return -1;
}

bool verificaFilaVazia(FILA* f) {
    return (f->inicio);
}

void mostrarFila(FILA* f) {
    NO* aux = f->inicio;
    printf("FILA: ");
    while (aux) {
        printf("%d ", aux->vertice);
        aux = aux->prox;
    }
    printf("\n");
}

// Mostra o caminhos ate o vertice atual recursivamente.
void mostrar_caminhos(int* antecessores, int tam, int atual) {
    if (atual >= tam || atual < 0) return;
    mostrar_caminhos(antecessores, tam, antecessores[atual]);
    printf("%d ", atual);
}

void visitaBP(Grafo* g, int v, cor* cores, int* antecessores) {
    cores[v] = CINZA;
    Apontador atual = primeiroListaAdj(v, g);
    int u;
    printf("%d ", v);
    while (atual != VERTICE_INVALIDO) {
        u = obtemVerticeDestino(atual, g);
        if (u < 0 || u >= obtemNrVertices(g)) break;
        if (cores[u] == BRANCO) {
            antecessores[u] = v;
            visitaBP(g, u, cores, antecessores);
        }
        atual = proxListaAdj(v, g, atual);
    }
    
    cores[v] = PRETO;
}

void buscaProfundidade(Grafo* g) {
    int nv = obtemNrVertices(g);
    cor* cores = (cor*) malloc(sizeof(cor)*nv);
    int* antecessores = (int*) malloc(sizeof(int)*nv);
    int i;
    for (i = 0; i < nv; i++) {
        cores[i] = BRANCO;
        antecessores[i] = -1;
    }
    printf("BP:\n");
    int j;
    for (j = 0; j < nv; j++) {
        if (cores[j] == BRANCO) {
            visitaBP(g, j, cores, antecessores);
        }
    } 
    printf("\nCaminhos BP: \n");
    int k;
    for (k=0; k < nv; k++) {
        mostrar_caminhos(antecessores, nv, k);
        printf("\n");
    }
}

void visitaBL(Grafo* g, int v, cor* cores, int* antecessores) {
    FILA f;
    inicializarFila(&f);
    adicionarFila(&f, v);
    int atual;
    Apontador aux;
    int vaux;

    while (verificaFilaVazia(&f)) {
        atual = removerFila(&f);
        aux = primeiroListaAdj(atual, g);
        while (aux != VERTICE_INVALIDO) {
            vaux = obtemVerticeDestino(aux, g);
            if (cores[vaux] == BRANCO) {
                cores[vaux] = CINZA;
                adicionarFila(&f, vaux);
                antecessores[vaux] = atual;
            } 
            aux = proxListaAdj(atual, g, aux);
        }
        cores[atual] = PRETO;
        printf("%d ", atual);
    }

}


void buscaLargura(Grafo* g) {
    int nv = obtemNrVertices(g);
    cor* cores = (cor*) malloc(sizeof(cor)*nv);
    int* antecessores = (int*) malloc(sizeof(int)*nv);
    int i;
    for (i = 0; i < nv; i++) {
        cores[i] = BRANCO;
        antecessores[i] = -1;
    }
    printf("BL:\n");

    int j;
    for (j = 0; j < nv; j++) {
        if (cores[j] == BRANCO) {
            visitaBL(g, j, cores, antecessores);
        }
    } 
    int k;
    printf("\nCaminhos BL: \n");
    for (k=0; k < nv; k++) {
        mostrar_caminhos(antecessores, nv, k);
        printf("\n");
    }
}

int main() {
    

    FILE *fp;

    fp = fopen("entrada2.txt", "r");
    int nv, na; // nv - numero de vertices, na - numero de arestasg
    fscanf(fp, "%d %d\n", &nv, &na);

    Grafo g;
    inicializaGrafo(&g, nv);
    
    int i;
    int v1, v2, peso;
    for (i = 0; i < na; i++) {
        fscanf(fp, "%d %d %d\n", &v1, &v2, &peso);
        insereAresta(v1, v2, peso, &g);
        insereAresta(v2, v1, peso, &g);
    }
 
    /* imprimeGrafo(&g);
    Apontador adj = primeiroListaAdj(3, &g);
    printf("Primeiro adj: %d\n", obtemVerticeDestino(adj, &g));
    adj = proxListaAdj(3, &g, adj);
    printf("Segundo adj: %d\n", obtemVerticeDestino(adj, &g));
    adj = proxListaAdj(3, &g, adj);
    printf("Terceiro adj: %d\n", obtemVerticeDestino(adj, &g));
    adj = proxListaAdj(3, &g, adj);
    printf("Quarto adj: %d\n", obtemVerticeDestino(adj, &g));    */
    buscaProfundidade(&g);
    buscaLargura(&g);
    //imprimeGrafo(&g);
    return 0;
}