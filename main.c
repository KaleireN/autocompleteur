#include "dictionary.h"
#include "autocomplete.h"
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

word *dictionary;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        errx(1, "this program require at least 1 argument");
    }


    size_t nb_words = get_number_of_line(argv[1]);
    dictionary = build_dictionary(argv[1], nb_words);
    int **matrix = malloc(sizeof(int *) * nb_words);
    for(size_t i = 0; i < nb_words; i++)
    {
        matrix[i] = calloc(nb_words+1, sizeof(int));
    }

    train(matrix, "english_sample", dictionary, nb_words);
    autocomplete(dictionary, nb_words, matrix);

    for(size_t i = 0; i < nb_words; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}