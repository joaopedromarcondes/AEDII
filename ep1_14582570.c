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

void mostrarFila(FILA* f, FILE* fp) {
    NO* aux = f->inicio;
    fprintf(fp, "FILA: ");
    while (aux) {
        fprintf(fp, "%d ", aux->vertice);
        aux = aux->prox;
    }
    fprintf(fp, "\n");
}

// Mostra o caminhos ate o vertice atual recursivamente.
void mostrar_caminhos(int* antecessores, int tam, int atual, FILE* fp) {
    if (atual >= tam || atual < 0) return;
    mostrar_caminhos(antecessores, tam, antecessores[atual], fp);
    fprintf(fp, "%d ", atual);
}

void mostrar_componentes_conectados(int* componentes_conectados, int nv, FILE* fp) {
    fprintf(fp, "Componentes Conectados: \n");
    int i, j, cont = 0;
    for (i = 0; i < nv && cont < nv; i++) {
        fprintf(fp, "C%d: ", i+1);
        for (j = 0; j < nv; j++) {
            if (componentes_conectados[j] == i) {
                fprintf(fp, "%d ", j);
                cont++;
            }
        }
        fprintf(fp, "\n");
    }
}

void mostrar_busca(int* antecessores, int nv, bool eh_BP, int* ordem, FILE* fp) {
    char bp_or_bl = 'P';
    int k;
    if (!eh_BP) bp_or_bl = 'L';
    fprintf(fp, "B%c: \n", bp_or_bl);
    for (k=0; k < nv; k++) {
        fprintf(fp, "%d ", ordem[k]);
    }
    fprintf(fp, "\n\nCaminhos B%c: \n", bp_or_bl);
    for (k=0; k < nv; k++) {
        mostrar_caminhos(antecessores, nv, k, fp);
        fprintf(fp, "\n");
    }
}

void mostrar_vertices_de_articulacao(bool* vertices_de_articulacao, int nv, FILE* fp) {
    fprintf(fp, "Vertices de articulacao:\n");
    int i;
    for (i = 0; i < nv; i++) {
        if (vertices_de_articulacao[i]) fprintf(fp, "%d ", i);
    }
    fprintf(fp, "\n");
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

void visitaBL(Grafo* g, int v, cor* cores, int* antecessores, int* ordem_bl, int* contador) {
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
        ordem_bl[*contador] = atual;
        (*contador)++;
    }

}


void buscaLargura(Grafo* g, int* antecessores, int* ordem_bl) {
    int nv = obtemNrVertices(g);
    cor* cores = (cor*) malloc(sizeof(cor)*nv);
    int* contador = (int*) malloc(sizeof(int));
    *contador = 0;
    int i;
    for (i = 0; i < nv; i++) {
        cores[i] = BRANCO;
        antecessores[i] = -1;
    }

    int j;
    for (j = 0; j < nv; j++) {
        if (cores[j] == BRANCO) {
            visitaBL(g, j, cores, antecessores, ordem_bl, contador);
        }
    } 
}

int main(int argc, char *argv[ ] ) {
    if (argc != 3) {
        fprintf(stderr, "ERRO na passagem de valores por linha decomando.\n");
        return 1;
    }
    char* arquivo_de_entrada = argv[1];
    char* arquivo_de_saida = argv[2];

    FILE *fp;

    fp = fopen(arquivo_de_entrada, "r");
    int nv, na; // nv - numero de vertices, na - numero de arestas
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

    fclose(fp);

    fp = fopen(arquivo_de_saida, "w");
 
    
    int* componentes_conectados = (int*) malloc(sizeof(int)*nv);
    int* antecessores_bp = (int*) malloc(sizeof(int)*nv);
    int* ordem_bp = (int*) malloc(sizeof(int)*nv);
    bool* vertices_de_articulacao = (bool*) malloc(sizeof(bool)*nv);

    int* antecessores_bl = (int*) malloc(sizeof(int)*nv);
    int* ordem_bl = (int*) malloc(sizeof(int)*nv);
    
    buscaLargura(&g, antecessores_bl, ordem_bl);
    buscaProfundidade(&g, componentes_conectados, antecessores_bp, ordem_bp, vertices_de_articulacao);

    mostrar_busca(antecessores_bl, nv, false, ordem_bl, fp);
    fprintf(fp, "\n");
    mostrar_busca(antecessores_bp, nv, true, ordem_bp, fp);
    fprintf(fp, "\n");
    mostrar_componentes_conectados(componentes_conectados, nv, fp);
    fprintf(fp, "\n");
    mostrar_vertices_de_articulacao(vertices_de_articulacao, nv, fp); 
    
    return 0;
}