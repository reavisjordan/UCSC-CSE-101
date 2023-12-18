/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa3
Graph.c - Graph ADT implementation file
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"

typedef struct GraphObj
{
    List* adj;
    int *color;
    int* parent;
    int* disc;
    int* finish;
    int order;
    int size;
} GraphObj;

//Constructors-destructors
Graph newGraph(int n)
{
    if(n < 1)
    {
        printf("Graph error: calling newGraph() with 0 vertices\n");
        exit(EXIT_FAILURE);
    }

    Graph G = malloc(sizeof(GraphObj));
    assert(G != NULL);
    G -> order = n;
    G -> size = 0;
    G -> adj = malloc(sizeof(List) *(n+1));
    G -> color = malloc(sizeof(int) *(n+1));
    G -> parent = malloc(sizeof(int) *(n+1));
    G -> disc = malloc(sizeof(int) *(n+1));
    G -> finish = malloc(sizeof(int) *(n+1));

    for(int i = 1; i <= n; i++)
    {
        G -> adj[i] = newList();
        G -> color[i] = WHITE;
        G -> parent[i] = NIL;
        G -> disc[i] = UNDEF;
        G -> finish[i] = UNDEF;
    }
    return G;
}

void freeGraph(Graph *G)
{
    int i, n;

    if(G == NULL)
    {
        printf("Graph error: calling freeGraph() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    n = (*G) -> order;

    for(i = 1; i <= n; i++)
    {
        freeList(&(*G) -> adj[i]);
    }

    free((*G) -> color);
    free((*G) -> parent);
    free((*G) -> disc);
    free((*G) -> finish);
    free((*G) -> adj);
    free(*G);
    G = NULL;
}

//Access functions
int getOrder(Graph G)
{
    if(G == NULL)
    {
        printf("Graph error: calling getOrder() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G -> order;
}

int getSize(Graph G)
{
    if(G == NULL)
    {
        printf("Graph error: calling getSize() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G -> size;
}

int getParent(Graph G, int u)
{
    if(G == NULL)
    {
        printf("Graph error: calling getParent() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if(u < 1 || u > G -> order)
    {
        printf("Graph error: calling getParent() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G -> parent[u];
}

int getDiscover(Graph G, int u)
{
    if(G == NULL)
    {
        printf("Graph error: calling getDiscover() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if(u < 1 || u > G -> order)
    {
        printf("Graph error: calling getDiscover() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G -> disc[u];
}

int getFinish(Graph G, int u)
{
    if(G == NULL)
    {
        printf("Graph error: calling getFinish() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if(u < 1 || u > G -> order)
    {
        printf("Graph error: calling getFinish() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G -> finish[u];
}

//Manipulation procedures
void addArc(Graph G, int u, int v)
{
     if(G == NULL)
    {
        printf("Graph error: calling addArc() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if(u < 1 || u > G -> order)
    {
        printf("Graph error: calling addArc() on NULL Graph reference\n");
        freeGraph(&G);
        exit(EXIT_FAILURE);
    }

    if(v < 1 || v > G -> order)
    {
        printf("Graph error: calling addArc() on NULL Graph reference\n");
        freeGraph(&G);
        exit(EXIT_FAILURE);
    }

    List A = G -> adj[u];

    if(length(A) == 0)
    {
        append(A, v);
    } else {
        moveFront(A);
        while(index(A) != -1)
        {
            if(get(A) == v) {
                    return;
                }
            if(get(A) > v)
            {
                insertBefore(A, v);
                break;
            }
            moveNext(A);
        }
        if(index(A) == -1)
        {
            append(A, v);
        }
    }
    G -> size++;
}

void addEdge(Graph G, int u, int v)
{
    if(G == NULL)
    {
        printf("Graph error: calling addEdge() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if(u < 1 || u > G -> order)
    {
        printf("Graph error: calling addEdge() on NULL Graph reference\n");
        freeGraph(&G);
        exit(EXIT_FAILURE);
    }

    if(v < 1 || v > G -> order)
    {
        printf("Graph error: calling addEdge() on NULL Graph reference\n");
        freeGraph(&G);
        exit(EXIT_FAILURE);
    }

    if(u != v)
    {
        addArc(G, u, v);
        addArc(G, v, u);
        G -> size--;
    }
}

void Visit(Graph G, List S, int x, int *time)
{
    if(G == NULL)
    {
        printf("Graph error: calling Visit() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(S == NULL)
    {
        printf("Graph error: calling Visit() on invalid List reference\n");
        exit(EXIT_FAILURE);
    }

    List A = G -> adj[x];
    G -> disc[x] = ++(*time);
    G -> color[x] = GRAY;
    for(moveFront(A); index(A) != -1; moveNext(A))
    {
        int y = get(A);
        if(G -> color[y] == WHITE)
        {
            G -> parent[y] = x;
            Visit(G, S, y, time);
        }
    }
        G -> color[x] = BLACK;
        G -> finish[x] = ++(*time);
        prepend(S, x);
}

void DFS(Graph G, List S)
{
    if(G == NULL)
    {
        printf("Graph error: calling DFS() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(S == NULL)
    {
        printf("Graph error: calling DFS() on invalid List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(S) != getOrder(G))
    {
        printf("Graph error: calling DFS(); List and Graph are not equal\n");
        printf("Length of list: %d\n", length(S));
        printf("Order of graph: %d\n", getOrder(G));
        freeGraph(&G);
        exit(EXIT_FAILURE);
    }


    int time = 0;
    int i, x;
    for(i = 1; i <= G -> order; i++)
    {
        G -> color[i] = WHITE;
        G -> parent[i] = NIL;
    }

    for(moveFront(S); index(S) != -1; moveNext(S))
    {
        x = get(S);
        if(G -> color[x] == WHITE)
        {
            Visit(G, S, x, &time);
        }
    }

    for(i = 1; i <= G -> order; i++)
    {
        deleteBack(S);
    }
}

//Other operations
Graph transpose(Graph G)
{
    if(G == NULL)
    {
        printf("Graph error: calling transpose() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    int i, x;

    int n = G -> order;
    Graph T = newGraph(n);

    for(i = 1; i <= G -> order; i++)
    {
        moveFront(G -> adj[i]);
        while(index(G -> adj[i]) != -1)
        {
            x = get(G -> adj[i]);
            addArc(T, x, i);
            moveNext(G -> adj[i]);
        }
    }

    return T;
}

Graph copyGraph(Graph G)
{
    if(G == NULL)
    {
        printf("Graph error: calling copyGraph() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }
    
    int n = G -> order;
    Graph T = newGraph(n);
    int i;

    for(i = 1; i <= G -> order; i++)
    {
        moveFront(G -> adj[i]);
        while(index(G -> adj[i]) != -1)
        {
            addArc(T, i, get(G -> adj[i]));
            moveNext(G -> adj[i]);
        }
    }

    return T;
}

void printGraph(FILE *out, Graph G)
{
    if(G == NULL || !G)
    {
        printf("Graph error: calling printGraph() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i <= G -> order; i++)
    {
        moveFront(G -> adj[i]);
        fprintf(out, "%d: ", i);
        while(index(G -> adj[i]) > -1)
        {
            fprintf(out, "%d ", get(G -> adj[i]));
            moveNext(G -> adj[i]);
        }
        fprintf(out, "\n");
    }
}