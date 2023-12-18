/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa1
Lex.c - client List ADT program
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "List.h"

#define MAX_LEN 300

int main(int argc, char* argv[])
{   
    FILE *in, *out;
    char** str = NULL;
    char line[MAX_LEN];
    List L = newList(); 
    int i, s_len, line_count = 0; 

    //Check command line for number of arguments
    if(argc != 3)
    {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    //open "in" file for reading
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if(in == NULL || out == NULL)
    {
        fprintf(stderr, "Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    while(fgets(line, MAX_LEN, in) != NULL)
    {
        line_count++;
    }

    rewind(in);
    str = malloc(line_count * sizeof(char *));
    for(i = 0; i < line_count; i++)
    {
        fgets(line, MAX_LEN, in);
        s_len = strlen(line);
        str[i] = malloc(s_len + 1 * sizeof(char));
        strcpy(str[i], line);
    }

    for(i = 0; i < line_count; i++)
    {
        moveFront(L);
        while(index(L) >= 0)
        {
            int data = get(L);
            if(strcmp(str[i], str[data]) <= 0)
            {
                insertBefore(L, i);
                break;
            } 
            moveNext(L);
        }
        if(index(L) == -1)
        {
            append(L, i);
        }
    }

    for(moveFront(L); index(L) >= 0; moveNext(L))
    {
        fprintf(out, "%s", str[get(L)]);
    }

    fclose(out);
    fclose(in); 
    freeList(&L);
    for(i = 0; i < line_count; i++)
    {
        free(str[i]);
    }
    free(str);

    return 0; 

}
