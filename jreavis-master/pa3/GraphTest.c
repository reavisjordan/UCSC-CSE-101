/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa3
GraphTest.c - Graph ADT testing file
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"

int main(int argc, char* argv[])
{
    int n, i;
    n = 5;
    Graph G = newGraph(n);
    Graph T = NULL;
    List S = newList();
   

    printf("Order of G: %d\n", getOrder(G));
    for(i=1; i<=n; i++) append(S, i);

    addArc(G, 1, 2);
    addArc(G, 1, 5);
    addArc(G, 2, 3);
    addArc(G, 3, 1);
    addArc(G, 3, 4);
    addArc(G, 4, 5);
    printf("Adjacency list representation of G:\n");
    printGraph(stdout, G);

    printf("Size of G: %d\n", getSize(G));

    DFS(G, S);
    for(i = 1; i <= n; i++)
    {
        printf("Parent: %d\n", getParent(G, i));
        printf("Discover: %d\n", getDiscover(G, i));
        printf("Finish: %d\n", getFinish(G, i));
    }

    T = transpose(G);
    printf("Transpose of G: \n");
    printGraph(stdout, T);

    Graph C = copyGraph(G);
    printf("Copy of graph G: \n");
    printGraph(stdout, C);

    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
    freeGraph(&C);

    return 0;
}