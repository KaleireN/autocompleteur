#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

void autocomplete(word *dic, size_t nb_words, int **matrix);

#endif