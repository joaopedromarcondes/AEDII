#ifdef MATRIZ
#include "grafo_matrizadj.h"
#else
#include "grafo_listaadj.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

void mostrar_componentes_conectados(int* componentes_conectados, int nv) {
    printf("Componentes Conectados: \n");
    int i, j, cont = 0;
    for (i = 0; i < nv && cont < nv; i++) {
        printf("C%d: ", i+1);
        for (j = 0; j < nv; j++) {
            if (componentes_conectados[j] == i) {
                printf("%d ", j);
                cont++;
            }
        }
        printf("\n");
    }
}

void mostrar_busca(int* antecessores, int nv, bool eh_BP, int* ordem) {
    char bp_or_bl = 'P';
    int k;
    if (!eh_BP) bp_or_bl = 'L';
    printf("B%c: \n", bp_or_bl);
    for (k=0; k < nv; k++) {
        printf("%d ", ordem[k]);
    }
    printf("\nCaminhos B%c: \n", bp_or_bl);
    for (k=0; k < nv; k++) {
        mostrar_caminhos(antecessores, nv, k);
        printf("\n");
    }
}

void mostrar_vertices_de_articulacao(bool* vertices_de_articulacao, int nv) {
    printf("Vertices de articulacao:\n");
    int i;
    for (i = 0; i < nv; i++) {
        if (vertices_de_articulacao[i]) printf("%d ", i);
    }
    printf("\n");
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

void visitaBP(Grafo* g, int v, cor* cores, int* antecessores, int* componentes_conectados, int contador, int* tempo_de_encontro, int* ordem, bool* vertices_de_articulacao, int* low, int* tempo_de_descoberta) {
    cores[v] = CINZA;
    Apontador atual = primeiroListaAdj(v, g);
    int u;
    ordem[*tempo_de_encontro] = v;
    tempo_de_descoberta[v] = *tempo_de_encontro;
    low[v] = *tempo_de_encontro;
    //printf("vertice: %d low: %d\n", v, low[v]);
    (*tempo_de_encontro)++;
    int contador_de_filhos = 0;
    while (atual != VERTICE_INVALIDO) {
        u = obtemVerticeDestino(atual, g);
        if (u < 0 || u >= obtemNrVertices(g)) break;
        if (cores[u] == BRANCO) {
            antecessores[u] = v;
            visitaBP(g, u, cores, antecessores, componentes_conectados, contador, tempo_de_encontro, ordem, vertices_de_articulacao, low, tempo_de_descoberta);
            contador_de_filhos++;
            low[v] = min(low[u], low[v]);

            if ( antecessores[v] != -1 && low[u] >= tempo_de_descoberta[v]) {
                vertices_de_articulacao[v] = true;
            }
        } else if (cores[u] == CINZA) {
            if (antecessores[v] != u) {
                low[v] = tempo_de_descoberta[u];
            }
        }
        atual = proxListaAdj(v, g, atual);
    }
    componentes_conectados[v] = contador;
    cores[v] = PRETO;

    if (antecessores[v] == -1 && contador_de_filhos > 1) {
        vertices_de_articulacao[v] = true;
    }
    
    //printf("vertice: %d tempo: %d low: %d\n", v, tempo_de_descoberta[v], low[v]);
}

void buscaProfundidade(Grafo* g, int* componentes_conectados, int* antecessores, int* ordem, bool* vertices_de_articulacao) {
    int nv = obtemNrVertices(g);
    cor* cores = (cor*) malloc(sizeof(cor)*nv);
    int* low = (int*) malloc(sizeof(int)*nv);
    int* tempo_de_descoberta = (int*) malloc(sizeof(int)*nv);
    int i;
    for (i = 0; i < nv; i++) {
        cores[i] = BRANCO;
        antecessores[i] = -1;
        vertices_de_articulacao[i] = false;
        tempo_de_descoberta[i] = -1;
        low[i] = -1;
    }
    int j, contador = 0;
    int* tempo_de_encontro = (int*) malloc(sizeof(int));
    *tempo_de_encontro = 0;
    for (j = 0; j < nv; j++) {
        if (cores[j] == BRANCO) {
            visitaBP(g, j, cores, antecessores, componentes_conectados, contador, tempo_de_encontro, ordem, vertices_de_articulacao, low, tempo_de_descoberta);
            contador++;
        }
    } 
    
}

void visitaBL(Grafo* g, int v, cor* cores, int* antecessores, int* ordem_bl) {
    FILA f;
    inicializarFila(&f);
    adicionarFila(&f, v);
    int atual;
    Apontador aux;
    int vaux;
    int contador = 0;

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
        ordem_bl[contador] = atual;
        contador++;
    }

}


void buscaLargura(Grafo* g, int* antecessores, int* ordem_bl) {
    int nv = obtemNrVertices(g);
    cor* cores = (cor*) malloc(sizeof(cor)*nv);
    int i;
    for (i = 0; i < nv; i++) {
        cores[i] = BRANCO;
        antecessores[i] = -1;
    }

    int j;
    for (j = 0; j < nv; j++) {
        if (cores[j] == BRANCO) {
            visitaBL(g, j, cores, antecessores, ordem_bl);
        }
    } 
    int k;
}

int main() {
    

    FILE *fp;

    fp = fopen("entrada1.txt", "r");
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
    int* componentes_conectados = (int*) malloc(sizeof(int)*nv);
    int* antecessores_bp = (int*) malloc(sizeof(int)*nv);
    int* ordem_bp = (int*) malloc(sizeof(int)*nv);
    bool* vertices_de_articulacao = (bool*) malloc(sizeof(bool)*nv);

    int* antecessores_bl = (int*) malloc(sizeof(int)*nv);
    int* ordem_bl = (int*) malloc(sizeof(int)*nv);
    
    buscaLargura(&g, antecessores_bl, ordem_bl);
    buscaProfundidade(&g, componentes_conectados, antecessores_bp, ordem_bp, vertices_de_articulacao);

    mostrar_busca(antecessores_bl, nv, false, ordem_bl);
    
    mostrar_busca(antecessores_bp, nv, true, ordem_bp);
    mostrar_componentes_conectados(componentes_conectados, nv);
    mostrar_vertices_de_articulacao(vertices_de_articulacao, nv);
    
    return 0;
}