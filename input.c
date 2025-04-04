/**
 * @file input.c
 * @author Sean Leana (smleana)
 * this file reads in input via the readLine function. Reads one line at a time.
 * returns the line if successful and null if not
 */

#include <stdio.h>
#include <stdlib.h>
#include "input.h"

/**
 * read line reads in a file and parses it until EOF or EOL
 * @param fp the file to be read
 * @return the string read
 */
char* readLine(FILE *fp)
{
    if (fp == NULL) {
        fp = stdin;
    }

    char ch = 0;
    if ((ch = fgetc(fp)) == EOF) {
        return NULL;
    } else {
        ungetc(ch, fp);
    }

    int capacity = 5;
    char *line = (char*) malloc(capacity * sizeof(char));
    int len = 0;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if (len >= capacity) {
            capacity *= 2;
            char *newLine = (char*) malloc(capacity * sizeof(char));
            for (int i = 0; i < len; i++) {
                newLine[i] = line[i];
            }
            free(line);
            line = newLine;
        }
        line[len++] = ch;
    }
    line[len] = '\0';
    return line;
}