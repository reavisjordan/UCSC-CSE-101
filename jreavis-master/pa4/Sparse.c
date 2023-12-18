#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Matrix.h"

int main(int argc, char *argv[]) {
    int n, i, nnza, nnzb, row, col;
    double val;
    FILE* out;
    FILE* in;
    Matrix A, B, C, D, E, F, G, H, I, J;

    if(argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "r");
    if(!in) {
        printf("Error opening file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if(!out) {
        printf("Error opening file %s for writing\n", argv[2]);
        exit(2);
    }

    fscanf(in, " %d %d %d", &n, &nnza, &nnzb);

    A = newMatrix(n);
    B = newMatrix(n);

    for(i = 1; i <= nnza; i++) {
        fscanf(in, "%d %d %lf", &row, &col, &val);
        changeEntry(A, row, col, val);
    }

    for(i = 1; i <= nnzb; i++) {
        fscanf(in, "%d %d %lf", &row, &col, &val);
        changeEntry(B, row, col, val);
    }

    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);
    fprintf(out, "\n");

    fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    fprintf(out, "\n");

    fprintf(out, "(1.5)*A =\n");
    C = scalarMult(1.5, A);
    printMatrix(out, C);
    fprintf(out, "\n");

    fprintf(out, "A+B =\n");
    D = sum(A, B);
    printMatrix(out, D);
    fprintf(out, "\n");

    fprintf(out, "A+A =\n");
    E = sum(A, A);
    printMatrix(out, E);
    fprintf(out, "\n");

    fprintf(out, "B-A =\n");
    F = diff(B, A);
    printMatrix(out, F);
    fprintf(out, "\n");

    fprintf(out, "A-A =\n");
    G = diff(A, A);
    printMatrix(out, G);
    fprintf(out, "\n");

    fprintf(out, "Transpose(A) =\n");
    H = transpose(A);
    printMatrix(out, H);
    fprintf(out, "\n");

    fprintf(out, "A*B =\n");
    I = product(A, B);
    printMatrix(out, I);
    fprintf(out, "\n");

    fprintf(out, "B*B =\n");
    J = product(B, B);
    printMatrix(out, J);
    fprintf(out, "\n");

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);

    fclose(in);
    fclose(out);

    return 0;
}