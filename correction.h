#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

#ifndef CORRECTION_H
#define CORRECTION_H

size_t levenshtein(char *str1, char *str2);
void printmat(size_t *m, size_t c, size_t r);
int find_correction(int** mat,word* dic, size_t nb_words, char* word, char* prev);

#endif