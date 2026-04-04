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

/* --- Algoritmo base  --- */
int eConexo(Grafo g);

/* --- Algoritmos principais --- */
int quantidadeComponentes(Grafo g);
int encontrarVerticesCorte(Grafo g, int *cortes, int *numCortes);
int encontrarArestasCorte(Grafo g, int *origem, int *destino, int *numArestas);

/* --- Impressao dos resultados (chamada na main) --- */
void imprimirComponentes(Grafo g);
void imprimirVerticesCorte(Grafo g);
void imprimirArestasCorte(Grafo g);

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

    noV = (No *)malloc(sizeof(No));
    noV->vertice = v;
    noV->prox = g->adj[u];
    g->adj[u] = noV;

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

int eConexo(Grafo g)
{
    int marcado[MAX_VERTICES] = {0};
    int mudou;

    marcado[0] = 1;
    do
    {
        mudou = 0;

        for (int i = 0; i < g.ordem; i++)
        {
            if (marcado[i])
            {
                for (No *p = g.adj[i]; p != NULL; p = p->prox)
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

    for (int i = 0; i < g.ordem; i++)
    {
        if (!marcado[i])
            return 0;
    }

    return 1;
}

int quantidadeComponentes(Grafo g)
{
    int marcado[MAX_VERTICES] = {0};
    int componentes = 0;

    for (int i = 0; i < g.ordem; i++)
    {
        if (!marcado[i])
        {
            componentes++;
            marcado[i] = 1;
            int mudou;
            do
            {
                mudou = 0;
                for (int j = 0; j < g.ordem; j++)
                {
                    if (marcado[j])
                    {
                        for (No *p = g.adj[j]; p != NULL; p = p->prox)
                        {
                            int viz = p->vertice;

                            if (!marcado[viz])
                            {
                                marcado[viz] = 1;
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

int encontrarVerticesCorte(Grafo g, int *cortes, int *numCortes)
{
    /* TODO: implementar */
    (void)g;
    (void)cortes;
    *numCortes = 0;
    return 0;
}

int encontrarArestasCorte(Grafo g, int *origem, int *destino, int *numArestas)
{
    /* TODO: implementar */
    (void)g;
    (void)origem;
    (void)destino;
    *numArestas = 0;
    return 0;
}

/* --- Verificacao de grafo hamiltoniano --- */
int Hamiltoniano(Grafo g);
int buscaCicloHamiltoniano(Grafo g, int v, int visitado[], int contador, int inicio);
int buscaCicloHamiltoniano(Grafo g, int v, int visitado[], int contador, int inicio)
{
    No *p;
    
    if (contador == g.ordem)
    {
        /* Verifica se existe aresta de volta ao inicio */
        for (p = g.adj[v]; p != NULL; p = p->prox)
            if (p->vertice == inicio)
                return 1;
        return 0;
    }
    
    for (p = g.adj[v]; p != NULL; p = p->prox)
    {
        int viz = p->vertice;
        if (!visitado[viz])
        {
            visitado[viz] = 1;
            if (buscaCicloHamiltoniano(g, viz, visitado, contador + 1, inicio))
                return 1;
            visitado[viz] = 0;
        }
    }
    return 0;
}

int Hamiltoniano(Grafo g)
{
    int visitado[MAX_VERTICES];
    int i;
    
    if (g.ordem==0) return 0;
    
    /* Tenta comecar de cada vertice*/
    for (i=0;i<g.ordem;i++)
    {
        int j;
        for (j =0;j<g.ordem;j++)
            visitado[j]=0;
        visitado[i]=1;
        if (buscaCicloHamiltoniano(g,i,visitado,1,i))
            return 1;
    }
    return 0;
}


void imprimirComponentes(Grafo g)
{
    int qtd =quantidadeComponentes(g);
    int marcado[MAX_VERTICES] = {0};
    int atual =0;

    for (int i=0; i<g.ordem;i++)
    {
        if (!marcado[i])
        {
            atual++;
            printf("Componente %d:",atual);

            marcado[i] = 1;
            printf(" %d",i);

            int mudou;
            do
            {
                mudou =0;

                for (int j=0; j<g.ordem; j++)
                {
                    if (marcado[j])
                    {
                        for (No *p = g.adj[j]; p != NULL; p = p->prox)
                        {
                            int v = p->vertice;

                            if (!marcado[v])
                            {
                                marcado[v] =1;
                                printf(" %d",v);
                                mudou =1;
                            }
                        }
                    }
                }

            } while (mudou);

            printf("\n");
        }
    }

    printf("Quantidade de componentes conexas: %d\n",qtd);
}

void imprimirVerticesCorte(Grafo g)
{
    int cortes[MAX_VERTICES];
    int numCortes = 0;
    encontrarVerticesCorte(g, cortes, &numCortes);
    /* TODO: imprimir resultado */
    printf("Vertices de corte: [TODO]\n");
}

void imprimirArestasCorte(Grafo g)
{
    int origem[MAX_VERTICES];
    int destino[MAX_VERTICES];
    int numArestas = 0;
    encontrarArestasCorte(g, origem, destino, &numArestas);
    /* TODO: imprimir resultado */
    printf("Arestas de corte (pontes): [TODO]\n");
}

int main(void)
{
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
    if (eConexo(g))
    {
        printf("O grafo E conexo.\n");
    }
    else
    {
        printf("O grafo NAO e conexo.\n");
    }
    /* --- Verificacao de grafo hamiltoniano --- */
     if (Hamiltoniano(g))
       printf("O grafo é hamiltoniano (possui ciclo que visita todos os vertices).\n");
      else
        printf("O grafo nao é hamiltoniano.\n");

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
