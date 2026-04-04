/*
 * =============================================================================
 * Projeto: Componentes e Cortes em Grafos
 * Disciplina: Teoria dos Grafos - 2026-1
 * =============================================================================
 *
 * Membros do grupo:
 *   - Luis Fernando de Mesquita Pereira - RA 10410686
 *   - [Nome Completo 2]
 *   - [Nome Completo 3]
 * 
 * Compilacao (ANSI C):
 *   gcc -ansi -pedantic -Wall -o grafos main.c
 * =============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 20  

typedef struct No {
    int vertice;       /* Indice do vertice vizinho */
    struct No *prox;   /* Proximo no da lista */
} No;

typedef struct {
    int ordem;                    /* Numero de vertices (n) */
    int tamanho;                  /* Numero de arestas (m)  */
    No *adj[MAX_VERTICES];        /* adj[i]: lista de vizinhos do vertice i */
} Grafo;

/* --- Construcao do grafo --- */
Grafo criarGrafo(int numVertices);
void  adicionarAresta(Grafo *g, int u, int v);
void  liberarGrafo(Grafo *g);

/* --- Algoritmo base  --- */
int eConexo(Grafo g);

/* --- Algoritmos principais --- */
int  quantidadeComponentes(Grafo g);
int  encontrarVerticesCorte(Grafo g, int *cortes, int *numCortes);
int  encontrarArestasCorte(Grafo g, int *origem, int *destino, int *numArestas);

/* --- Impressao dos resultados (chamada na main) --- */
void imprimirComponentes(Grafo g);
void imprimirVerticesCorte(Grafo g);
void imprimirArestasCorte(Grafo g);

Grafo criarGrafo(int numVertices) {
    Grafo g;
    int i;

    g.ordem   = numVertices;
    g.tamanho = 0;

    for (i = 0; i < numVertices; i++) {
        g.adj[i] = NULL;
    }

    return g;
}

void adicionarAresta(Grafo *g, int u, int v) {
    No *noU, *noV;

    noV          = (No *) malloc(sizeof(No));
    noV->vertice = v;
    noV->prox    = g->adj[u];
    g->adj[u]    = noV;

    noU          = (No *) malloc(sizeof(No));
    noU->vertice = u;
    noU->prox    = g->adj[v];
    g->adj[v]    = noU;

    g->tamanho++;
}

void liberarGrafo(Grafo *g) {
    int i;
    No *atual, *prox;

    for (i = 0; i < g->ordem; i++) {
        atual = g->adj[i];
        while (atual != NULL) {
            prox  = atual->prox;
            free(atual);
            atual = prox;
        }
        g->adj[i] = NULL;
    }
}

int eConexo(Grafo g) {
    // TODO: Implementar 
    return 1;
}

int quantidadeComponentes(Grafo g) {
    /* TODO: implementar */
    (void) g;
    return -1;
}

int encontrarVerticesCorte(Grafo g, int *cortes, int *numCortes) {
    /* TODO: implementar */
    (void) g;
    (void) cortes;
    *numCortes = 0;
    return 0;
}

int encontrarArestasCorte(Grafo g, int *origem, int *destino, int *numArestas) {
    /* TODO: implementar */
    (void) g;
    (void) origem;
    (void) destino;
    *numArestas = 0;
    return 0;
}

void imprimirComponentes(Grafo g) {
    int qtd = quantidadeComponentes(g);
    /* TODO: imprimir resultado */
    (void) qtd;
    printf("Quantidade de componentes conexas: [TODO]\n");
}

void imprimirVerticesCorte(Grafo g) {
    int cortes[MAX_VERTICES];
    int numCortes = 0;
    encontrarVerticesCorte(g, cortes, &numCortes);
    /* TODO: imprimir resultado */
    printf("Vertices de corte: [TODO]\n");
}

void imprimirArestasCorte(Grafo g) {
    int origem[MAX_VERTICES];
    int destino[MAX_VERTICES];
    int numArestas = 0;
    encontrarArestasCorte(g, origem, destino, &numArestas);
    /* TODO: imprimir resultado */
    printf("Arestas de corte (pontes): [TODO]\n");
}

int main(void) {
    Grafo g;

    /* --- Inicializacao do grafo fixo --- */
    g = criarGrafo(7);
    adicionarAresta(&g, 0, 1);
    adicionarAresta(&g, 0, 3);
    adicionarAresta(&g, 1, 2);
    adicionarAresta(&g, 1, 4);
    adicionarAresta(&g, 4, 5);
    adicionarAresta(&g, 4, 6);

    printf("=== Analise do Grafo ===\n\n");

    /* --- Conexidade --- */
    if (eConexo(g)) {
        printf("O grafo E conexo.\n");
    } else {
        printf("O grafo NAO e conexo.\n");
    }

    /* --- Componentes conexas --- */
    imprimirComponentes(g);

    /* --- Vertices de corte --- */
    imprimirVerticesCorte(g);

    /* --- Arestas de corte (pontes) --- */
    imprimirArestasCorte(g);

    /* --- Liberacao de memoria --- */
    liberarGrafo(&g);

    return 0;
}