/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa4
Matrix.c - Matrix ADT implementation file
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Matrix.h"
#include "List.h"


//Private Entry obj 
typedef struct EntryObj* Entry;

typedef struct EntryObj {
    int col;
    double val;
} EntryObj;

//Matrix obj 
typedef struct MatrixObj {
    int size;
    int NNZ;
    List *row;
} MatrixObj;

//Constructor-destructor for Entry obj
Entry newEntry(int col, double val) {
    if(col < 0) {
        fprintf(stderr, "Matrix Entry error: calling newEntry() using invalid column reference\n");
        exit(EXIT_FAILURE);
    }

    Entry E = malloc(sizeof(EntryObj));
    assert(E != NULL);
    E -> col = col;
    E -> val = val;
    return (E);
}

void deleteEntry(Entry *E) {
    if(E != NULL && *E != NULL) {
        free(*E);
        *E = NULL;
    }
}

//Constructor-destructor for Matrix
Matrix newMatrix(int n) {
    if(n < 0) {
        fprintf(stderr, "Matrix error: calling newMatrix() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix M = malloc(sizeof(MatrixObj));
    if(M == NULL) {
        fprintf(stderr, "Matrix error: calling newMatrix() with bad malloc\n");
        exit(EXIT_FAILURE);
    }
    M -> row = malloc(sizeof(List) * (n + 1));
    M -> NNZ = 0;
    M -> size = n;

    for(int i = 1; i <= n; i++) {
        M -> row[i] = newList();
    }    
    
    return M;
}

void freeMatrix(Matrix *M) {
    if(M == NULL) {
        fprintf(stderr, "Matrix error: calling freeMatrix() on invalid Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    int i, s;
    Entry E;
    s = (*M) -> size;

    for(i = 1; i <= s; i++) {
        for(moveFront((*M) -> row[i]); index((*M) -> row[i]) >= 0; moveNext((*M) -> row[i])) {
            E = (Entry)get((*M) -> row[i]);
            deleteEntry(&E);
        }
        freeList(&(*M) -> row[i]);
    }

    free((*M) -> row);
    free((*M));
    
    M = NULL;
}

//Access functions
int size(Matrix M) {
    if(M == NULL) {
        fprintf(stderr, "Matrix error: calling size() on invalid Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    return M -> size;
}

int NNZ(Matrix M) {
    if(M == NULL) {
        fprintf(stderr, "Matrix error: calling NNZ() on invalid Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    return M -> NNZ;
}

int equals(Matrix A, Matrix B) {
    if(A == NULL || B == NULL) {
        fprintf(stderr, "Matrix error: calling equals() on invalid Matrix reference(s)\n");
        exit(EXIT_FAILURE);
    }
    if(A -> size != B -> size) {
        return 0;
    }
    if(A -> NNZ != B -> NNZ) {
        return 0; 
    }
    if(A == B) {
        return 1;
    }

    Entry X, Y;
    List c, d;
    int i;

    for(i = 1; i <= A -> size; i++) {
         if(length(A -> row[i]) != length(B -> row[i])) {
                return 0;
            }
        c = A -> row[i];
        d = B -> row[i];
        moveFront(c);
        moveFront(d);
        while(index(c) != -1 && index(d) != -1) {
            X = (Entry)(get(A -> row[i]));
            Y = (Entry)(get(B -> row[i]));
            if(X -> val != Y -> val) {
                return 0;
            } else {
                moveNext(c);
                moveNext(d);
            }
        }
    }
    return 1;
}

//Manipulation procedures

void makeZero(Matrix M) {
    if(M == NULL) {
        fprintf(stderr, "Matrix error: calling makeZero() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    M -> NNZ = 0;
    for(int i = 1; i <= M -> size; i++) {
        List S = M -> row[i];
        for(moveFront(S); index(S) != -1; moveNext(S)) {
            Entry data = get(S);
            deleteEntry(&data);
        }
        clear(S);
    }
}

void changeEntry(Matrix M, int i, int j, double x) {
    if(M == NULL) {
        fprintf(stderr, "Matrix error: calling changeEntry() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    if(i < 1 || i > M -> size) {
        fprintf(stderr, "Matrix error: calling changeEntry() with invalid arguments\n");
        freeMatrix(&M);
        exit(EXIT_FAILURE);
    }
    if(j < 1 || j > M -> size) {
        fprintf(stderr, "Matrix error: calling changeEntry() with invalid arguments\n");
        freeMatrix(&M);
        exit(EXIT_FAILURE);
    }
    List L = M -> row[i];
    Entry E;

    L = M -> row[i];
    if(length(L) == 0 && x != 0) {
        append(L, newEntry(j, x));
        M -> NNZ++;
        return;
    }

    for(moveFront(L); index(L) != -1; moveNext(L)) {
        E = (Entry)get(M -> row[i]);
        if (index(L) != -1 && E -> col > j) {
            if(x != 0) {
                insertBefore(L, newEntry(j, x));
                 M -> NNZ++;
                 return;
            } else {
                //deleteEntry(&E);
                return;
            }

        } else if (index(L) != -1 && E -> col == j) {
            if(x != 0) {
                E -> val = x;
                return;
            } else {
                deleteEntry(&E);
                delete(L);
                M -> NNZ--;
                return;
            }
        } 
    }
        if(index(L) == -1) {
            if(x != 0) {
                append(L, newEntry(j, x));
                 M -> NNZ++;
                 return;
            } //else {
            //     //deleteEntry(&E);
            // }
    }
}

//Matrix Arithmetic Operations
Matrix copy(Matrix A) {
    if(A == NULL) {
        fprintf(stderr, "Matrix error: calling copyMatrix() on invalid Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix copy = newMatrix(size(A));
    Entry E;
    int i, col;
    double val;

    for(i = 1; i <= size(A); i++) {
        moveFront(A -> row[i]);
        while(index(A -> row[i]) >= 0) {
            E = (Entry)get(A -> row[i]);
            col = E -> col;
            val = E -> val;
            append(copy -> row[i], newEntry(col, val));
            copy -> NNZ++;
            moveNext(A -> row[i]);
        }
    }
    return copy;
}

Matrix transpose(Matrix A) {
    if(A == NULL) {
        fprintf(stderr, "Matrix error: calling transpose() on invalid Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix T = newMatrix(size(A));
    Entry E;
    int i, col;
    double val;

    for(i = 1; i <= size(A); i++) {
        moveFront(A -> row[i]);
        while(index(A -> row[i]) >= 0) {
            E = (Entry)get(A -> row[i]);
            col = E -> col;
            val = E -> val;
            changeEntry(T, col, i, val);
            moveNext(A -> row[i]);
        }
    }
    T -> NNZ = A -> NNZ;
    return T;
}

Matrix scalarMult(double x, Matrix A) {
    if(A == NULL) {
        fprintf(stderr, "Matrix error: calling scalarMult() on invalid Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix M = newMatrix(size(A));
    Entry E;
    int i, col;
    double val;

    if(x == 0) {
        deleteEntry(&E);
        return M;
    }

    for(i = 1; i <= size(A); i++) {
        moveFront(A -> row[i]);
        while(index(A -> row[i]) >= 0) {
            E = (Entry)get(A -> row[i]);
            val = ((E -> val) * x);
            col = E -> col;
            append(M -> row[i], newEntry(col, val));
            M -> NNZ++;
            moveNext(A -> row[i]);
        }
    }
    return M;
}

//sum() and diff() helper function
void vecSum(List A, List B, List C, int sign) {             
    double x, y, z;
    Entry a, b;

    moveFront(A);
    moveFront(B);
    moveFront(C);

    while(index(A) >= 0 && index(B) >= 0) {
        a = (Entry)get(A);
        b = (Entry)get(B);
        x = a -> val;
        y = (sign)*b -> val;

        if(a -> col == b -> col) {
            z = x + y;
            if(z != 0) {
                append(C, newEntry(a -> col, z));
            }
            moveNext(A);
            moveNext(B);
        }
            else if(a -> col < b -> col) {
                append(C, newEntry(a -> col, x));
                moveNext(A);
            }else {
                append(C, newEntry(b -> col, y));
                moveNext(B);
            }
        
    }

    while(index(A) >= 0) {
        a = (Entry)get(A);
        append(C, newEntry(a -> col, a -> val));
        moveNext(A);
    }

    while(index(B) >= 0) {
        b = (Entry)get(B);
        append(C, newEntry(b -> col, ((b -> val)*(sign))));
        moveNext(B);
    }
}

Matrix sum(Matrix A, Matrix B) {
    if(A == NULL || B == NULL) {
        fprintf(stderr, "Matrix error: calling sum() on invalid Matrix reference(s)\n");
        exit(EXIT_FAILURE);
    }
    if(size(A) != size(B)) {
        fprintf(stderr, "Matrix error: calling sum() on unequal Matrices\n");
        exit(EXIT_FAILURE);
    }

    Matrix Add;

    if(equals(A, B)) { 
        Add = scalarMult(2.0, A);
    } else {
         Add = newMatrix(size(A));
        for(int i = 1; i <= size(A); i++) {
            vecSum(A -> row[i], B -> row[i], Add -> row[i], 1);
            Add -> NNZ += length(Add -> row[i]);
        }
    }
    return Add;
}

Matrix diff(Matrix A, Matrix B) {
    if(A == NULL || B == NULL) {
        fprintf(stderr, "Matrix error: calling sum() on invalid Matrix reference(s)\n");
        exit(EXIT_FAILURE);
    }
    if(size(A) != size(B)) {
        fprintf(stderr, "Matrix error: calling sum() on unequal Matrices\n");
        exit(EXIT_FAILURE);
    }

    Matrix Diff = newMatrix(size(A));

    if(equals(A, B)) {
        makeZero(Diff);
        return Diff;
    } else {
        for(int i = 1; i <= size(A); i++) {
            vecSum(A -> row[i], B -> row[i], Diff -> row[i], -1);
            Diff -> NNZ += length(A -> row[i]);
        }
    }
    return Diff;
}

//product() helper function
double vecDot(List A, List B) {
    if(A == NULL || B == NULL) {
        fprintf(stderr, "Matrix error: calling helper function vecDot() w/ invalid List reference(s)\n");
        exit(EXIT_FAILURE);
    }

    double dp = 0.0;
    Entry a, b;

    moveFront(A);
    moveFront(B);

    while(index(A) >= 0 && index(B) >= 0) {
        a = (Entry)get(A);
        b = (Entry)get(B);

        if(a -> col == b -> col) {
            dp += ((a -> val)*(b -> val));
            moveNext(A);
            moveNext(B);
        } else if(a -> col < b -> col) {
            moveNext(A);
        } else {
            moveNext(B);
        }
    }
    return dp;
}

Matrix product(Matrix A, Matrix B) {
    if(A == NULL || B == NULL) {
        fprintf(stderr, "Matrix error: calling product() on invalid Matrix reference(s)\n");
        exit(EXIT_FAILURE);
    }
    if(size(A) != size(B)) {
        fprintf(stderr, "Matrix error: calling product() on unequal Matrix references\n");
        exit(EXIT_FAILURE);
    }

    Matrix M = newMatrix(size(A));
    Matrix T = transpose(B);
    int i, j;
    double x;

    for(i = 1; i <= size(A); i++) {
        if(length(A -> row[i]) > 0) {
        for(j = 1; j <= size(T); j++) {
            if(length(T -> row[j]) > 0) {
            x = vecDot(A -> row[i], T -> row[j]);
            if(x != 0) {
                append(M -> row[i], newEntry(j, x));
                M -> NNZ++;
            }
            }
        }
        }
    }
    freeMatrix(&T);
    return M;
}


//Other operations
void printMatrix(FILE* out, Matrix M) {
    if(M == NULL) {
        fprintf(stderr, "Matrix error: passing invalid Matrix to printMatrix()\n");
        exit(EXIT_FAILURE);
    }
    Entry E;
    int cont = 0;
    for (int i = 1; i <= size(M); i++) {
        cont = 0;
        moveFront((M->row[i]));
        if (index(M->row[i]) >= 0) {
            E = (Entry) get(M->row[i]);
            if (E) {
                fprintf(out, "%d: ", i);
                cont = 1;
            }
        }
        while (index((M->row[i])) >= 0) {
            E = (Entry) get(M->row[i]);
            if (E) {
                fprintf(out, "(%d, %.1f) ", E->col, E->val);
            }
            moveNext(M->row[i]);
        }
        if (cont == 1) {
            fprintf(out, "\n");
        }
    }
}
