#include <stdio.h>
#include <stdlib.h>

#ifndef DICTIONARY_H
#define DICTIONARY_H

/*
    word is a string containing the word
*/
typedef struct word{
    char* word;
}word;

/*
    start is included and end is excluded
*/
typedef struct interval{
    size_t start;
    size_t end;
}interval;

void to_lower_case(char *word);
int get_word(FILE *file, char *buf);
int is_part_of_word(char c);
ssize_t find_word(word *dic, char *word, size_t nb_words);
interval *find_patern(word *dic, char *patern, size_t nb_words);
size_t get_number_of_line(char *path);
word* build_dictionary(char* path, size_t nb_words);
void train(int **M,char *path, word *dic, size_t nb_words);
void reset_str(char *str);

#endif