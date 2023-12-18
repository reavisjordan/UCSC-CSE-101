/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa2
Graph.c - Graph ADT implementation file
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"

//private Graph object type
typedef struct GraphObj
{
    List* adj; //neighbors of vertex "i"
    int* color; //"i"th element is the color (white, gray, black) of vertex "i"
    //color representations: white = -1; gray = 0; black = 1
    int* parent; //"i"th element is the parent of vertex "i"
    int* dist; //distance from most recent source to vertex "i"
    int order; //number of vertices
    int size; //number of edges
    int source; //(most recent) source vertex for BFS()
} GraphObj;

//Constructors-Destructors
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
    G -> source = NIL;
    G -> adj = malloc(sizeof(List) *(n+1));
    G -> color = malloc(sizeof(int) *(n+1));
    G -> parent = malloc(sizeof(int) *(n+1));
    G -> dist = malloc(sizeof(int) *(n+1));

    for(int i = 1; i <= n; i++)
    {
        G -> adj[i] = newList();
        G -> color[i] = WHITE;
        G -> parent[i] = NIL;
        G -> dist[i] = INF;
    }

    printf("Graph object created!\n");

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
    free((*G) -> dist);
    free((*G) -> adj);
    free(*G);
    G = NULL;
}

//Access functions
int getOrder(Graph G)
{
    if(G == NULL)
    {
        printf("Graph error: calling getOrder() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G -> order;
}

int getSize(Graph G)
{
    if(G == NULL)
    {
        printf("Graph error: calling getSize() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G -> size;
}

int getSource(Graph G)
{                       
    if(G == NULL)
    {
        printf("Graph error: Calling getSource() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if(G -> source == NIL)
    {
        printf("Graph error: must call BFS() before getSource()\n");
        return NIL;
    }         

    return G -> source;
}

int getParent(Graph G, int u)
{
    if(G == NULL)
    {
        printf("Graph error: Calling getParent() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if( u < 1 || u > getOrder(G))
    {
        printf("Graph error: calling getParent() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if(G -> source == NIL)
    {
        return NIL;
    }         

    return G -> parent[u];
}

int getDist(Graph G, int u)
{
    if(G == NULL)
    {
        printf("Graph error: Calling getDist() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if( u < 1 || u > getOrder(G))
    {
        printf("Graph error: calling getParent() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G -> dist[u];
}

void getPath(List L, Graph G, int u)
{
    if(L == NULL && G == NULL)
    {
        printf("Graph error: passing invalid List or Graph reference to getPath()\n");
        exit(EXIT_FAILURE);
    }

    if(G -> source == NIL)
    {
        printf("Graph error: must call BFS() before getPath()\n");
        exit(EXIT_FAILURE);
    } 

    if( u < 1 || u > getOrder(G))
    {
        printf("Graph error: calling getPath() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }   

    if(u == G -> source)
    {
        append(L, G -> source);
    } else if(G -> parent[u] == NIL)
    {
        append(L, NIL);
    } else {
        getPath(L, G, G -> parent[u]);
        append(L, u);
    }
}

//Manipulation procedures
void makeNull(Graph G)
{
    if(G == NULL)
    {
        printf("Graph error: calling makeNull() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }

    G -> size = 0;
    G -> source = NIL;

    for(int i = 1; i <= G -> order; i++)
    {
        G -> parent[i] = NIL;
        G -> dist[i] = INF;
        G -> color[i] = WHITE;
        clear(G -> adj[i]);
    }
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
        exit(EXIT_FAILURE);
    }

    if(v < 1 || v > G -> order)
    {
        printf("Graph error: calling addEdge() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if(u != v)
    {
        addArc(G, u, v);
        addArc(G, v, u);
        G -> size--;
    }
}

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
        exit(EXIT_FAILURE);
    }

    if(v < 1 || v > G -> order)
    {
        printf("Graph error: calling addArc() on NULL Graph reference\n");
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

void BFS(Graph G, int s)
{
    if(G == NULL)
    {
        printf("Graph error: calling BFS() on invalid Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(s < 1 || s > G -> order)
    {
        printf("Graph error: calling BFS() on invalid Graph reference\n");
        freeGraph(&G);
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i <= G -> order; i++)
    {
        G -> color[i] = WHITE;
        G -> dist[i] = INF;
        G -> parent[i] = NIL;
    }

    G -> source = s;
    G -> color[s] = GRAY;
    G -> dist[s] = 0;
    G -> parent[s] = NIL;

    List Q = newList();
    append(Q, s);
    while(length(Q) != 0)
    {
        int x = front(Q);
        deleteFront(Q);
        moveFront(G -> adj[x]);

        while(index(G -> adj[x]) != -1)
        {
            int y = get(G -> adj[x]);
            if(G -> color[y] == WHITE)
            {
                G -> color[y] = GRAY;
                G -> dist[y] = G -> dist[x] + 1;
                G -> parent[y] = x;
                append(Q, y);
            }
            moveNext(G -> adj[x]);
        }
        G -> color[x] = BLACK;
    }
    freeList(&Q);
}

void printGraph(FILE* out, Graph G)
{
    if(!G)
    {
        printf("Graph error: calling printGraph() on empty Graph reference\n");
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


