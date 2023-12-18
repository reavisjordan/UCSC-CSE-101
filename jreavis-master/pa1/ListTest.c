/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa1
ListTest.c - test file for List ADT functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"


int main(int argc, char* argv[])
{   
    List A = newList(); 
    int i;

    for(i = 1; i <= 20; i++)
    {
        append(A, i);
    }
    printf("List A:\n");
    printList(stdout, A);
    printf("List length: %d\n", length(A));
    moveFront(A);
    printf("Index of front element: %d\n", index(A));
    printf("Value of front element: %d\n", front(A));
    moveNext(A);
    printf("Value of next element: %d\n", get(A));
    moveBack(A);
    printf("Index of back element: %d\n", index(A));
    printf("Value of back element: %d\n", back(A));
    movePrev(A); 
    printf("Value of previous element: %d\n", get(A));

    List B = copyList(A);
    printf("List B:\n");
    printList(stdout, B);
    equals(A, B);
    clear(B);
    equals(A, B);

    moveFront(A);
    set(A, 5);
    printf("New front element for List A: %d\n", front(A));
    printList(stdout, A);

    printf("Adding 0 to List A:\n");
    prepend(A, 0);
    printList(stdout, A);

    moveFront(A);
    printf("Inserting -1 before the front...\n");
    insertBefore(A, -1);
    printList(stdout, A);

    moveBack(A);
    printf("Inserting 21 after the back element...\n");
    insertAfter(A, 21);
    printList(stdout, A);
    moveNext(A);

    printf("Deleting front and back elements...\n");

    deleteFront(A);
    deleteBack(A);

    printList(stdout, A);
    moveFront(A);
    printf("Deleting front cursor element...\n");
    delete(A);
    printList(stdout, A);


    freeList(&A);
    freeList(&B);

    List X = newList();
    List Y = newList();

    append(X, 1);
    append(Y, 2);

    printf("List X:\n");
    printList(stdout, X);

    printf("List Y:\n");
    printList(stdout, Y);

    List Z = concatList(X, Y);

    printf("List Z:\n");
    printList(stdout, Z);

    freeList(&X);
    freeList(&Y);
    freeList(&Z);

    return 0;
}