/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa3
Graph.h - Graph ADT header file
*/
#ifndef GRAPH_H_INCLUDE_
#define GRAPH_H_INCLUDE_

#include "List.h"
#define NIL -1
#define UNDEF -2
#define WHITE -3
#define GRAY -4
#define BLACK -5

typedef struct GraphObj* Graph;

//Constructors-Destructors
Graph newGraph(int n);
void freeGraph(Graph *G);

//Access functions
int getOrder(Graph G);
int getSize(Graph G);
int getParent(Graph G, int u);//pre: 1<=u<=n=getOrder(G)
int getDiscover(Graph G, int u);//pre: 1<=u<=n=getOrder(G)
int getFinish(Graph G, int u);//pre: 1<=u<=n=getOrder(G)

//Manipulation procedures
void addArc(Graph G, int u, int v);//pre: 1<=u<=n, 1<=v<=n
void addEdge(Graph G, int u, int v);//pre: 1<=u<=n, 1<=v<=n
void DFS(Graph G, List L);//pre: length(L)==getOrder(G)

//Other functions
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE *out, Graph G);
#endif