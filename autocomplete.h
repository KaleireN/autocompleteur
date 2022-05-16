#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

void autocomplete(word *dic, size_t nb_words, int **matrix);
void strautocomplete(word *dic, size_t nb_words, int **matrix, char *prev, char *act, char *row, int rsize, int pos, char c);
#endif