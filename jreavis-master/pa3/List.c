/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa3
List.c - List ADT implementation file
*/

//**CODE CITED FROM: Queue.C and Queue.h website examples as well as Mahyar Vayabi's pseudocode**//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"


//Private node type
typedef struct NodeObj* Node;

//Private NodeObj type
typedef struct NodeObj
{
    int data; 
    Node left;
    Node right;
} NodeObj;

//Node constructor
Node node_create(int data)
{
    Node N = malloc(sizeof(NodeObj));
    assert(N != NULL);
    N -> data = data;
    N -> left = NULL;
    N -> right = NULL;
    return (N); 
}

//Node destructor
void node_delete(Node* N)
{
    if(N != NULL && *N != NULL)
    {
        free(*N);
        *N = NULL; 
    }
}

//Private ListObj type
typedef struct ListObj
{
    int length;
    int curs_pos;
    Node curs;
    Node front;
    Node back;
} ListObj;

//Constructors-Destructors
List newList()
{
    List L = malloc(sizeof(ListObj));
    assert(L != NULL);
    L -> length = 0;
    L -> curs_pos = -1;
    L -> curs = NULL;
    L -> front = L -> back = NULL;
    return L; 
}

void freeList(List *L)
{
    if(L == NULL)
    {
        printf("List error: calling freeList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node n, temp;

    n = (*L) -> front; 
    for(int i = 0; i < (*L) -> length; i++)
    {
        temp = n;
        n = n -> right; 
        free(temp);
    }
    free(*L);
    L = NULL;

    //printf("Freed list successfully!\n");
}

int length(List L)
{
    if(L == NULL)
    {
        printf("List error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L -> length;
}

int index(List L)
{
    if(L == NULL)
    {
        printf("List error: calling index() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L -> curs_pos; 
}

int front(List L)
{
    if(L == NULL)
    {
        printf("List error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(length(L) <= 0)
    {
        freeList(&L);
        printf("List error: calling front() on empty List reference\n");
        exit(EXIT_FAILURE);
    }

    return L -> front -> data;
}

int back(List L)
{
    if(L == NULL)
    {
        printf("List error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(length(L) <= 0)
    {
        freeList(&L);
        printf("List error: calling back() on empty List reference\n");
        exit(EXIT_FAILURE);
    }

    return L -> back -> data; 
}

int get(List L)
{
     if(L == NULL)
    {
        printf("List error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(!(L -> length > 0))
    {
        freeList(&L);
        printf("List error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(!(index(L) >= 0))
    {
        freeList(&L);
        printf("List error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L -> curs -> data;
}

bool equals(List A, List B)
{
    if(A == NULL || B == NULL)
    {
        printf("List error: calling equals() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(A -> length == 0 && B -> length == 0)
    {
        return true;
    }

    if(A -> length != B -> length)
    {
        printf("False\n");
        return false;
    }
        Node a_front = A -> front; 
        Node b_front = B -> front;

    for(int i = 0; i < A -> length; i++)
    {
        if(a_front -> data == b_front -> data)
        {
            a_front = a_front -> right;
            b_front = b_front -> right; 
        } else {
            printf("False\n");
            return false;
        }
         
    }
    printf("True\n");
    return true; 
}


//Manipulation procedures
void clear(List L)
{
    if(L == NULL)
    {
        printf("List error: calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    while(L -> length != 0)
    {
        deleteFront(L);
    }

    L -> curs= NULL;
    L -> curs_pos = -1;
    L -> front = NULL;
    L -> back = NULL;
}

void set(List L, int x)
{
    if((!L -> length) > 0)
    {
        printf("List error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if((!L -> curs) > 0)
    {
        printf("List error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    L -> curs -> data = x;
}

void moveFront(List L)
{
    if(L == NULL)
    {
        printf("List error: calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if((L -> length > 0))
    {
        L -> curs = L -> front;
        L -> curs_pos = 0;
    }

}

void moveBack(List L)
{
    if(L == NULL)
    {
        printf("List error: calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if((L -> length > 0))
    {
        L -> curs = L -> back;
        L -> curs_pos = L -> length - 1; 
    }
}

void movePrev(List L)
{
    if(L == NULL)
    {
        printf("List error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(L -> curs_pos > 0)
    {
        L -> curs = L -> curs -> left;
        L -> curs_pos--;
    } else if(L -> curs_pos == 0)
    {
        L -> curs_pos =- 1;
        L -> curs = NULL;
    }    
}

void moveNext(List L)
{
    if(L == NULL)
    {
        printf("List error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(L -> curs_pos >= 0 && L -> curs_pos < L -> length -1)
    {
        L -> curs = L -> curs -> right;
        L -> curs_pos++;
    } else if(L -> curs_pos == L -> length -1)
    {
        L -> curs = NULL;
        L -> curs_pos = -1;
    }
}

void prepend(List L, int x)
{
    if(L == NULL)
    {
        printf("List error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = node_create(x);

    if(L -> length <= 0)
    {
        L -> front = N;
        L -> back = L -> front;
        L -> length++;
    } else {
        N -> right = L -> front;
        L -> front -> left = N;
        L -> front = N;
        L -> length++;
    }

    if((L -> curs_pos) != -1)
    {
        L -> curs_pos++; 
    }
}

void append(List L, int x) 
{
    if(L == NULL)
    {
        printf("List error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = node_create(x);

    if(L -> length <= 0)
    {
        L -> back = N;
        L -> front= N;
        L -> length++;
    } else{
        N -> left = L -> back;
        L -> back -> right = N; 
        L -> back = N;
        L -> length++; 
    }
}

void insertBefore(List L, int x)
{
    if(L == NULL)
    {
        printf("List error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(L -> length == 0)
    {
        freeList(&L);
        printf("List error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(L -> curs_pos == -1)
    {
        freeList(&L);
        printf("List error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(L -> curs -> left == NULL)
    {
        prepend(L, x);
    } else {
        Node N = node_create(x);
        N -> left = L -> curs -> left; 
        N -> right = L -> curs;
        L -> curs -> left -> right = N;
        L -> curs -> left = N;
        L -> length++;
        L -> curs_pos++;
    }
}

void insertAfter(List L, int x)
{
    if(L == NULL)
    {
        printf("List error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(L -> length == 0)
    {
        freeList(&L);
        printf("List error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(L -> curs_pos == -1)
    {
        freeList(&L);
        printf("List error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

   

    if(L -> curs -> right == NULL)
    {
        append(L, x);
    } else{
        Node N = node_create(x);
        N -> right = L -> curs -> right;
        N -> left = L -> curs;
        L -> curs -> right -> left = N;
        L -> curs -> right = N;
        L -> length++;
    }
}

void deleteFront(List L)
{
    if(L == NULL)
    {
        printf("List error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(L -> length < 0)
    {
        printf("List error: calling deleteFront() on empty List reference\n");
        exit(EXIT_FAILURE);
    }

    if(L -> curs_pos == 0)
    {
        L -> curs = NULL;
    } 

    if(L -> length == 1)
    {
        Node N = L -> front;
        L -> front = L -> back = NULL;
        node_delete(&N);
    } else{
        Node N = L -> front;
        L -> front = L -> front -> right;
        node_delete(&N);
        L -> front -> left = NULL;
    }

    L -> length--;
    L -> curs_pos--;
}

void deleteBack(List L)
{
    if(L == NULL)
    {
        printf("List error: calling deleteBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(L -> length < 0)
    {
        printf("List error: calling deleteBack() on empty List reference\n");
        exit(EXIT_FAILURE);
    }

    if(L -> curs_pos == L -> length - 1)
    {
        L -> curs = NULL;
        L -> curs_pos = -1;
    }

    if(L -> length == 1)
    {
        Node N = L -> back;
        L -> front = L -> back = NULL;
        node_delete(&N);
    } else {
        Node N = L -> back;
        L -> back = L -> back -> left;
        node_delete(&N);
        L -> back -> right = NULL;
    }

    L -> length--;
}

void delete(List L)
{
    if(L == NULL)
    {
        printf("List error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(L -> curs_pos == -1)
    {
        printf("List error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(index(L) == 0)
    {
        deleteFront(L);
    } else if (index(L) == L -> length -1)
    {
        deleteBack(L);
    } else{
        L -> curs -> left -> right = L -> curs -> right;
        L -> curs -> right -> left = L -> curs -> left;
        node_delete(&L -> curs);
        L -> length--;
        L -> curs_pos--;
    }
    
}

//Other operations

void printList(FILE* out, List L)
{
    if(L == NULL)
    {
        printf("List error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node new = NULL;

    for(new = L -> front; new != NULL; new = new -> right)
    {
        fprintf(out, "%d ", new -> data);
    }
    fprintf(out, "\n");
}

List copyList(List L)
{
    if(L == NULL)
    {
        printf("List error: calling copyList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    List M = newList();
    Node temp = L -> front;

    while(temp != NULL)
    {
        append(M, temp -> data);
        temp = temp -> right;
    }

    node_delete(&temp);
    return M;
}

List concatList(List A, List B)
{

    if(A == NULL || B == NULL)
    {
        printf("List error: calling concatList() on invalid List reference\n");
        exit(EXIT_FAILURE);
    }
    
    List C = newList();
    Node a_front = A -> front;
    Node b_front = B -> front;

    while(a_front != NULL)
    {
        append(C, a_front -> data);
        a_front = a_front -> right;
    }

    while(b_front != NULL)
    {
        append(C, b_front -> data);
        b_front = b_front -> right;
    }

    node_delete(&a_front);
    node_delete(&b_front);
    return C;
}