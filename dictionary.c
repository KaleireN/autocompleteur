#include "dictionary.h"
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

#define false 0
#define true 1


/*
    modify word to convert all uper case in lower case
*/
void to_lower_case(char *word)
{
    size_t i = 0;
    while(word[i] != '\0')
    {
        if('A' < word[i] && word[i] < 'Z')
        {
            word[i] -= 'A' - 'a';
        }
        i++;
    }
}

/*
    return 0 if str1 == str2
    return 1 if str1 < str2
    return 2 if str1 > str2
*/
int compare_str(char *str1, char *str2)
{
    size_t i = 0;
    while(str1[i] == str2[i])
    {
        if(str1[i] == '\0')
            return 0;
        i++;
    }
    if(str1[i] < str2[i])
        return 1;
    return 2;
}


/*
    return the index of word in dic
    return -1 if word does not exist
*/
ssize_t find_word(word *dic, char *word, size_t nb_words)
{
    size_t begin = 0;
    size_t end = nb_words;

    while(begin <= end)
    {
        size_t mid = (end-begin)/2 + begin;
        if(mid >= nb_words)
            return -1;
        int state = compare_str(word, dic[mid].word);
        switch (state)
        {
        case 0:
            return mid;
            break;
        
        case 1:
            end = mid - 1;
            break;
        
        case 2:
            begin = mid + 1;
            break;
        }
    }
    return -1;
}


/*
    check if str1 is the begining of str2
    return 0 if str1 == str2
    return 1 if str1 < str2
    return 2 if str1 > str2
*/
int compare_patern(char *str1, char *str2)
{
    size_t i = 0;
    while(str1[i] != '\0')
    {
        if(str1[i] != str2[i])
        {
            if(str1[i] < str2[i])
                return 1;
            return 2;
        }
        i++;
    }
    return 0;
}


/*
    return the interval of indexes where words begin by patern
    return NULL if no word begin with patern 
*/
interval *find_patern(word *dic, char *patern, size_t nb_words)
{
    interval *inter = NULL;
    size_t i = 0;
    // used to stock res of compare_patern(char *str1, char *str2)
    int res;

    while(i < nb_words && (res = compare_patern(patern, dic[i].word)) == 2)
    {
        if(res == 1)
            return NULL;
        i++;
    }

    inter = malloc(sizeof(interval));
    inter->start = i;

    while(i < nb_words && (res = compare_patern(patern, dic[i].word)) == 0)
    {
        i++;
    }
    inter->end = i;

    return inter;
}


/*
    return the number of lines a file with given path
*/
size_t get_number_of_line(char *path)
{
    FILE *file;
    if((file = fopen(path,"r")) == 0)
    {
        errx(1, "ERROR: failed to load file: %s", path);
    }

    char *line = NULL;
    ssize_t read = 0;
    size_t len = 0;
    size_t i = 0;
    while((read = getline(&line, &len, file)) != -1)
    {
        i++;
    }

    free(line);
    fclose(file);
    return i;
}


/*
    build a list with every words of the given file with nb_words
    hint: use get_number_of_line(char *path) to get the number of word
*/
word* build_dictionary(char* path, size_t nb_words)
{
    FILE *file;
    if((file = fopen(path,"r")) == 0)
    {
        errx(1, "ERROR: failed to load file: %s", path);
    }
    
    word* dic = malloc(sizeof(word)*(nb_words));
    size_t i = 0;
    char *line = NULL;
    ssize_t read = 0;
    size_t len = 0;
    while((read = getline(&line, &len, file)) != -1)
    {
        dic[i].word = malloc(read+1);
        strcpy(dic[i].word, line);
        dic[i].word[read-1] = '\0';
        i++;
    }

    free(line);
    fclose(file);

    return dic;
}


int is_part_of_word(char c)
{
    if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')
        || c == '\'' || c == '-'){
            return true;
        }
    return false;
}


/*
    stock next word of given file in buf
    return false if there is no next word and true otherwise
*/
int get_word(FILE *file, char *buf)
{
    char c;

    // go to first char of next word
    do{
        c = fgetc(file);
        // end of file
        if(c == EOF)
        {
            return false;
        }
    }while(! is_part_of_word(c));

    // stock next word in buf
    size_t i = 0;
    while(is_part_of_word(c))
    {
        // too many char to be a word
        if(i == 50)
        {
            buf[0] = '\0';
            return true;
        }
        buf[i] = c;
        i++;
        c = fgetc(file);
    }
    buf[i] = '\0';
    return true;
}

/*
    copy str1 in str2
*/
void cp_str(char *str1, char *str2)
{
    size_t i = 0;
    while(str1[i] != '\0')
    {
        str2[i] = str1[i];
        i++;
    }
}


/*
    generate the initial probability of a word coming after another
    in form of a matrix where 
    M[first word][following word] = number of time following appear after first word
    in the given file
    and M[word][nb_words] = number of time a word appeared so that:
    (double) M[word1][word2] / M[word1][nb_words] gives the proba of word2 following word1

    hint: use find_word(word *dic, char *word, size_t nb_words) to get the index of a word
*/
void train(int **M,char *path, word *dic, size_t nb_words)
{
    char prev[50], act[50];
    ssize_t iprev, iact;

    FILE *file;
    if((file = fopen(path,"r")) == 0)
    {
        errx(1, "ERROR: failed to load file: %s", path);
    }

    get_word(file, prev);
    to_lower_case(prev);
    iprev = find_word(dic, prev, nb_words);
    while(get_word(file, act))
    {
        // the word was not valid
        if(act[0] == '\0')
        {
            continue;
        }
        to_lower_case(act);

        // check if word exist in dictionary
        if((iact = find_word(dic, act, nb_words)) != -1){
            if(iprev == -1)
            {
                iprev = iact;
                continue;
            }
            M[iprev][iact] += 1;
            M[iact][nb_words] += 1;

            iprev = iact;
        }
        else{
            // TODO
        }

        reset_str(act);
    }

    fclose(file);
}