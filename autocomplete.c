#include "autocomplete.h"
#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>


#define false 0
#define true 1


void reset_str(char *str)
{
    size_t i = 0;
    while(str[i] != '\0')
    {
        str[i] = '\0';
        i++;
    }
}

int find_next_word(char *prev, char *act, 
                   word *dic, size_t nb_words, int **matrix)
{
    interval *inter;

    if(act[0] != '\0')
        inter = find_patern(dic, act, nb_words);
    else
    {
        inter = malloc(sizeof(interval));
        inter->start = 0;
        inter->end = nb_words;
    }
    if(inter == NULL)
        return -1;
    

    int max = inter->start;
    size_t i = find_word(dic, prev, nb_words);

    for(size_t y = inter->start; y < inter->end; y++)
    {
        if(matrix[i][max] < matrix[i][y])
            max = y;
    }

    free(inter);
    return max;
}



void autocomplete(word *dic, size_t nb_words, int **matrix)
{
    char c;
    char prev[50];
    char act[50];
    memset(prev, 0, 50);
    memset(act, 0, 50);
    size_t len = 0;
    size_t lennext = 0;
    int next;
    printf("start\n");
    system ("/bin/stty raw");
    while((c = getchar()) != '~')
    {
        if(is_part_of_word(c))
        {
            // word is too long
            if(len == 48)
            {
                reset_str(act);
                len = 0;
                continue;
            }
            act[len] = c;
            len++;
            // no previous word
            if(prev[0] == '\0')
                continue;

            next = find_next_word(prev, act, dic, nb_words, matrix);
            if(next == -1)
                continue;

            if(lennext >= len)
            {
                for(size_t i = 0; i < lennext-len; i++)
                {
                    printf(" ");
                }
                for(size_t i = 0; i < lennext; i++)
                {
                    printf("\b \b");
                }
            }
            else
            {
                if(len <= lennext)
                {
                    for(size_t i = 0; i < len; i++)
                    {
                        printf("\b \b");
                    }
                }
                else
                {
                    for(size_t i = 0; i < lennext+1; i++)
                    {
                        printf("\b \b");
                    }
                }
            }

            printf("%s", dic[next].word);
            lennext = strlen(dic[next].word);
            if(len < lennext)
            {
                for(size_t i = 0; i < lennext-len; i++)
                {
                    printf("\b");
                }
            }
            
        }
        else
        {
            if(act[0] != '\0')
            {
                if(prev[0] != '\0')
                {
                    if(len < lennext)
                    {
                        for(size_t i = 0; i < len+1; i++)
                        {
                            printf("\b");
                        }
                    }
                    else
                    {
                        for(size_t i = 0; i < lennext+1; i++)
                        {
                            printf("\b");
                        }
                    }
                }
                
                if(find_word(dic, act, nb_words) != -1)
                {
                    if(prev[0] != '\0')
                    {
                        if(lennext > len)
                        {
                            for(size_t i = 0; i < lennext+1; i++)
                            {
                                printf(" ");
                            }
                            for(size_t i = 0; i < lennext+1; i++)
                            {
                                printf("\b");
                            }
                        }
                        printf("%s%c", act, c);
                    }
                    strcpy(prev, act);
                }
                else
                {
                    if(prev[0] != '\0')
                        printf("%s%c", dic[next].word, c);
                    strcpy(prev, dic[next].word);
                }
                reset_str(act);
            }
            
            len = 0;
            lennext = 1;
        }
    }
    system ("/bin/stty cooked");
    printf("\nend\n");
}


