/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa3
FindComponents.c - Graph ADT client file
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Graph.h"
#include "List.h"

int main(int argc, char *argv[]) {
    int i, n, u, v, comp;
    FILE *out, *in;
    Graph G, T;
    List S = newList();

    if(argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "r");
    if(in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if(out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
    }

    fscanf(in, " %d", &n);
    G = newGraph(n);
    u = 1;
    v = 1;
    comp = 0;

    while(fgetc(in) != 0) {
        fscanf(in, "%d", &u);
        fscanf(in, "%d", &v);

        if(u == 0 || v == 0) {
            break;
        }

        addArc(G, u, v);
    }

    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);

    for(i = 1; i <= getOrder(G); i++) {
        append(S, i);
    }

    fprintf(out, "\n");

    DFS(G, S);
    T = transpose(G);
    DFS(T, S);

    for(i = 1; i <= getOrder(T); i++) {
        if(getParent(T, i) == NIL) {
            comp++;
        }
    }

    fprintf(out, "G contains %d strongly connected components: \n", comp);

    List *SCC = malloc(sizeof(List) *(comp+1));
    for(i = 1; i <= comp; i++) {
        SCC[i] = newList();
    }

    int count = 1;
    int f, len;
    f = front(S);
    len = length(S); 
    for(i = 1; i <= len - 1; i++)
    {
        int data = back(S);
        prepend(SCC[count], data);
        printList(stdout, SCC[count]);
        deleteBack(S);
        if(getParent(T, data) == NIL)
        {
            count++;
        }
    }

    prepend(SCC[count], f);
    printList(stdout, S);

    for(i = 1; i <= comp; i++) {
        fprintf(out, "Component %d: ", i);
        printList(out, SCC[i]);
    }

    for(i = 1; i < comp; i++) {
        freeList(&SCC[i]);
    }
    free(SCC);
    freeGraph(&G);
    freeGraph(&T);
    freeList(&S);
    fclose(in);
    fclose(out);

    return 0;
}