/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa2
FindPath.c - Graph ADT client file
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Graph.h"

int main(int argc, char* argv[])
{
    int n, u, v;
    FILE *in, *out;
    List L;
    Graph G;

    if(argc != 3)
    {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);   
    }

    in = fopen(argv[1], "r");
    if(in == NULL)
    {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if(out == NULL)
    {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }

    fscanf(in, " %d", &n);

    G = newGraph(n);
    u = 1;
    v = 1;

    printf("test 1\n");

    while(u != 0 && v != 0)
    {
        fscanf(in, "%d ", &u);
        fscanf(in, "%d ", &v);

        if(u == 0 || v == 0)
        {
            break;
        }

        addEdge(G, u, v);
    }

    printf("test 2\n");

    printGraph(out, G);
    fprintf(out, "\n");
    L = newList();

    printf("test 3\n");
    u = 1;
    v = 2;

    while(u != 0 && v != 0)
    {
        printf("test 4\n");
        fscanf(in, "%d ", &u);
        fscanf(in, "%d ", &v);

        if(u == 0 && v == 0)
        {
            break;
        }

        BFS(G, u);
        getPath(L, G, v);
        moveFront(L);

        printf("test 5\n");

        if(get(L) == NIL || getDist(G, v) == INF)
        {
            printf("test 6\n");
            fprintf(out, "The distance from %d to %d is infinity\n", u, v);
            fprintf(out, "No %d-%d path exists\n", u, v);
            fprintf(out, "\n");
        } else {
            printf("test 7\n");
            fprintf(out, "The distance from %d to %d is %d\n", u, v, getDist(G, v));
            fprintf(out, "A shortest %d-%d path is: ", u, v);
            printList(out, L);
            fprintf(out, "\n");
        }
        clear(L);
    }

    fclose(in);
    fclose(out);
    freeGraph(&G);
    freeList(&L);

    return 0; 
}