/*
 * =============================================================================
 * Projeto: Componentes e Cortes em Grafos
 * Disciplina: Teoria dos Grafos - 2026-1
 * =============================================================================
 *
 * Membros do grupo:
 *   - Luis Fernando de Mesquita Pereira - RA 10410686
 *   - Rafael Santos Lourenço da Silva - RA 10403588
 *   - Edson Fu - RA 10419137
 *
 * Compilacao (ANSI C):
 *   gcc -ansi -pedantic -Wall -o grafos main.c
 * =============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 20

typedef struct No
{
    int vertice;     /* Indice do vertice vizinho */
    struct No *prox; /* Proximo no da lista */
} No;

typedef struct
{
    int ordem;             /* Numero de vertices (n) */
    int tamanho;           /* Numero de arestas (m)  */
    No *adj[MAX_VERTICES]; /* adj[i]: lista de vizinhos do vertice i */
} Grafo;

/* --- Construcao do grafo --- */
Grafo criarGrafo(int numVertices);
void adicionarAresta(Grafo *g, int u, int v);
void liberarGrafo(Grafo *g);

int eConexo(Grafo g);

/* --- Algoritmos principais --- */
int quantidadeComponentes(Grafo g);
int encontrarVerticesCorte(Grafo g, int *cortes, int *numCortes);
int encontrarArestasCorte(Grafo g, int *origem, int *destino, int *numArestas);

/* --- Impressao dos resultados (chamada na main) --- */
void imprimirComponentes(Grafo g);
void imprimirVerticesCorte(Grafo g);
void imprimirArestasCorte(Grafo g);

/* ------------------------------------------------------------------------- */

Grafo criarGrafo(int numVertices)
{
    Grafo g;
    int i;

    g.ordem = numVertices;
    g.tamanho = 0;

    for (i = 0; i < numVertices; i++)
    {
        g.adj[i] = NULL;
    }

    return g;
}

void adicionarAresta(Grafo *g, int u, int v)
{
    No *noU, *noV;

    /* Insere v na lista de adjacencia de u */
    noV = (No *)malloc(sizeof(No));
    noV->vertice = v;
    noV->prox = g->adj[u];
    g->adj[u] = noV;

    /* Insere u na lista de adjacencia de v (grafo nao dirigido) */
    noU = (No *)malloc(sizeof(No));
    noU->vertice = u;
    noU->prox = g->adj[v];
    g->adj[v] = noU;

    g->tamanho++;
}

void liberarGrafo(Grafo *g)
{
    int i;
    No *atual, *prox;

    for (i = 0; i < g->ordem; i++)
    {
        atual = g->adj[i];
        while (atual != NULL)
        {
            prox = atual->prox;
            free(atual);
            atual = prox;
        }
        g->adj[i] = NULL;
    }
}

/* ------------------------------------------------------------------------- *
 * eConexo                                                                   *
 * 1. Marca o vertice 0 com 1, os demais com 0.                              *
 * 2. Repete: se há aresta entre vertice marcado (1) e não marcado (0),      *
 *    marca o não marcado com 1.                                             *
 * 3. Se ao final todos estiverem marcados, o grafo é conexo.                *
 * Retorna 1 se conexo, 0 caso contrário.                                    *
 * ------------------------------------------------------------------------- */
int eConexo(Grafo g)
{
    int marcado[MAX_VERTICES];
    int mudou, i;
    No *p;

    if (g.ordem == 0)
        return 1;

    /* Inicializa todos os vertices como nao marcados */
    for (i = 0; i < g.ordem; i++)
        marcado[i] = 0;

    /* Marca o primeiro vertice */
    marcado[0] = 1;

    /* Propaga a marcacao enquanto houver novos vertices alcancados */
    do
    {
        mudou = 0;
        for (i = 0; i < g.ordem; i++)
        {
            if (marcado[i])
            {
                for (p = g.adj[i]; p != NULL; p = p->prox)
                {
                    int j = p->vertice;
                    if (!marcado[j])
                    {
                        marcado[j] = 1;
                        mudou = 1;
                    }
                }
            }
        }
    } while (mudou);

    /* Verifica se todos os vertices foram alcancados */
    for (i = 0; i < g.ordem; i++)
    {
        if (!marcado[i])
            return 0;
    }

    return 1;
}

/* ------------------------------------------------------------------------- *
 * quantidadeComponentes                                                      *
 * Conta as componentes conexas do grafo usando o algoritmo de marcação.     *
 * Cada vez que encontra um vértice não visitado, inicia uma nova BFS        *
 * (usando o mesmo algoritmo de eConexo) e incrementa o contador.           *
 * Retorna o número de componentes conexas.                                  *
 * ------------------------------------------------------------------------- */
int quantidadeComponentes(Grafo g)
{
    int marcado[MAX_VERTICES];
    int componentes, mudou, i, j;
    No *p;

    for (i = 0; i < g.ordem; i++)
        marcado[i] = 0;

    componentes = 0;

    for (i = 0; i < g.ordem; i++)
    {
        if (!marcado[i])
        {
            /* Novo vertice nao visitado: nova componente */
            componentes++;
            marcado[i] = 1;
            do
            {
                mudou = 0;
                for (j = 0; j < g.ordem; j++)
                {
                    if (marcado[j])
                    {
                        for (p = g.adj[j]; p != NULL; p = p->prox)
                        {
                            if (!marcado[p->vertice])
                            {
                                marcado[p->vertice] = 1;
                                mudou = 1;
                            }
                        }
                    }
                }
            } while (mudou);
        }
    }

    return componentes;
}

/* ------------------------------------------------------------------------- *
 * encontrarVerticesCorte                                                     *
 * Um vértice v é de corte se sua remoção aumenta o número de componentes.  *
 * Para cada vértice i, executa o algoritmo de marcacao ignorando i e        *
 * conta as componentes. Se o número superar o original, i é de corte.      *
 * Preenche o vetor cortes[] e define *numCortes.                            *
 * Retorna o número de vertices de corte encontrados.                        *
 * ------------------------------------------------------------------------- */
int encontrarVerticesCorte(Grafo g, int *cortes, int *numCortes)
{
    int marcado[MAX_VERTICES];
    int C, compSemV, mudou, i, j, k;
    No *p;

    C = quantidadeComponentes(g);
    *numCortes = 0;

    for (i = 0; i < g.ordem; i++)
    {
        /* Inicializa marcação excluindo o vértice i */
        for (j = 0; j < g.ordem; j++)
            marcado[j] = 0;
        marcado[i] = 1;

        compSemV = 0;

        /* Conta componentes do subgrafo sem o vértice i */
        for (j = 0; j < g.ordem; j++)
        {
            if (!marcado[j])
            {
                compSemV++;
                marcado[j] = 1;

                do
                {
                    mudou = 0;
                    for (k = 0; k < g.ordem; k++)
                    {

                        if (k != i && marcado[k])
                        {
                            for (p = g.adj[k]; p != NULL; p = p->prox)
                            {
                                if (!marcado[p->vertice])
                                {
                                    marcado[p->vertice] = 1;
                                    mudou = 1;
                                }
                            }
                        }
                    }
                } while (mudou);
            }
        }

        /* Se o numero de componentes aumentou, i é vértice de corte */
        if (compSemV > C)
        {
            cortes[*numCortes] = i;
            (*numCortes)++;
        }
    }

    return *numCortes;
}

/* ------------------------------------------------------------------------- *
 * encontrarArestasCorte (pontes)                                             *
 * Uma aresta (u,v) é de corte se sua remoção aumenta o número de           *
 * componentes. Para cada aresta, executa o algoritmo de marcação pulando    *
 * essa aresta e compara o resultado com o número original de componentes.   *
 * Preenche origem[] e destino[] com os extremos de cada ponte.             *
 * Retorna o numero de arestas de corte encontrados                         *
 * ------------------------------------------------------------------------- */
int encontrarArestasCorte(Grafo g, int *origem, int *destino, int *numArestas)
{
    int marcado[MAX_VERTICES];
    int C, compSemAresta, mudou, u, v, j, k;
    No *p, *q;

    C = quantidadeComponentes(g);
    *numArestas = 0;

    /* Itera sobre cada aresta uma unica vez (u < v) */
    for (u = 0; u < g.ordem; u++)
    {
        for (p = g.adj[u]; p != NULL; p = p->prox)
        {
            v = p->vertice;
            if (v <= u)
                continue;

            /* Inicializa marcação */
            for (j = 0; j < g.ordem; j++)
                marcado[j] = 0;

            compSemAresta = 0;

            /* Conta componentes do grafo sem a aresta (u, v) */
            for (j = 0; j < g.ordem; j++)
            {
                if (!marcado[j])
                {
                    compSemAresta++;
                    marcado[j] = 1;

                    do
                    {
                        mudou = 0;
                        for (k = 0; k < g.ordem; k++)
                        {
                            if (marcado[k])
                            {
                                for (q = g.adj[k]; q != NULL; q = q->prox)
                                {
                                    /* Ignora a aresta (u,v) nos dois sentidos */
                                    if ((k == u && q->vertice == v) ||
                                        (k == v && q->vertice == u))
                                        continue;

                                    if (!marcado[q->vertice])
                                    {
                                        marcado[q->vertice] = 1;
                                        mudou = 1;
                                    }
                                }
                            }
                        }
                    } while (mudou);
                }
            }

            /* Se o número de componentes aumentou, (u,v) é aresta de corte */
            if (compSemAresta > C)
            {
                origem[*numArestas] = u;
                destino[*numArestas] = v;
                (*numArestas)++;
            }
        }
    }

    return *numArestas;
}


void imprimirComponentes(Grafo g)
{
    int marcado[MAX_VERTICES];
    int mudou, i, j, qtd, atual;
    No *p;

    for (i = 0; i < g.ordem; i++)
        marcado[i] = 0;

    qtd = quantidadeComponentes(g);
    atual = 0;

    for (i = 0; i < g.ordem; i++)
    {
        if (!marcado[i])
        {
            atual++;
            printf("Componente %d:", atual);
            marcado[i] = 1;
            printf(" %d", i);

            do
            {
                mudou = 0;
                for (j = 0; j < g.ordem; j++)
                {
                    if (marcado[j])
                    {
                        for (p = g.adj[j]; p != NULL; p = p->prox)
                        {
                            if (!marcado[p->vertice])
                            {
                                marcado[p->vertice] = 1;
                                printf(" %d", p->vertice);
                                mudou = 1;
                            }
                        }
                    }
                }
            } while (mudou);

            printf("\n");
        }
    }
    printf("Quantidade de componentes conexas: %d\n", qtd);
}

void imprimirVerticesCorte(Grafo g)
{
    int cortes[MAX_VERTICES];
    int numCortes, i;
    numCortes = 0;
    encontrarVerticesCorte(g, cortes, &numCortes);
    printf("Vertices de corte (%d):", numCortes);
    for (i = 0; i < numCortes; i++)
        printf(" %d", cortes[i]);
    printf("\n");
}

void imprimirArestasCorte(Grafo g)
{
    int origem[MAX_VERTICES];
    int destino[MAX_VERTICES];
    int numArestas, i;
    numArestas = 0;
    encontrarArestasCorte(g, origem, destino, &numArestas);
    printf("Arestas de corte / pontes (%d):", numArestas);
    for (i = 0; i < numArestas; i++)
        printf(" (%d-%d)", origem[i], destino[i]);
    printf("\n");
}

int main(void)
{
    Grafo g;

    /* Inicializa o grafo com valores fixos */
    g = criarGrafo(7);
    adicionarAresta(&g, 0, 1);
    adicionarAresta(&g, 0, 3);
    adicionarAresta(&g, 1, 2);
    adicionarAresta(&g, 1, 4);
    adicionarAresta(&g, 4, 5);
    adicionarAresta(&g, 4, 6);
    printf("=== Analise do Grafo ===\n\n");

    /* Conexidade */
    if (eConexo(g))
    {
        printf("O grafo E conexo.\n");
    }
    else
    {
        printf("O grafo NAO e conexo.\n");
    }

    /* Componentes conexas */
    imprimirComponentes(g);

    printf("\n");

    /* Vértices de corte */
    imprimirVerticesCorte(g);

    /* Arestas de corte (pontes) */
    imprimirArestasCorte(g);

    /* Libera memória alocada */
    liberarGrafo(&g);

    return 0;
}
