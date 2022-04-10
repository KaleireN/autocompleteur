#include "correction.h"
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

size_t str_len(char *s)
{
    size_t k = 0;
    while (*s != '\0')
    {
        k++;
        s++;
    }
    return k;
}

void printmat(size_t *m,size_t r, size_t c)
{
    for(size_t i = 0; i < r; i++)
    {
        printf("[");
        for(size_t j = 0; j < c; j++)
        {
            if(j!=0)
                printf(", ");
            printf("%lu",m[i * c + j]);
        }
        printf("]\n");
    }
}

size_t levenshtein(char *str1, char *str2)
{
    size_t len1 = str_len(str1)+1;
    size_t len2 = str_len(str2)+1;
    size_t *mat = malloc(len1 * len2 * sizeof(size_t));
    if (mat == NULL)
        errx(1,"can't allocate matrix");
    memset(mat,0,len1*len2*sizeof(size_t));
    size_t cost = 0;
    size_t i = 0;
    size_t j = 0;
    size_t min = 0;
    for(i = 0; i < len1; i++)
        mat[i * len2] = i;
    for(j = 0; j < len2; j++)
        mat[j] = j;
    for(i = 1; i < len1; i++)
    {
        //printf("[");
        for (j = 1; j < len2; j++)
        {
            if (*(str1+i-1) == *(str2+j-1))
                cost = 0;
            else
                cost = 1;
            min = mat[(i-1) * len2 + j] + 1;
            if(mat[i * len2 + j - 1] + 1 < min)
                min = mat[i * len2 + j - 1] + 1;
            if(mat[(i-1) * len2 + j - 1] + cost < min)
                min = mat[(i-1) * len2 + j - 1] + cost;
            mat[i * len2 + j] = min;
            //printf("%lu, ",min);
        }
        //printf("]\n");
    }
    printmat(mat,len1,len2);
    size_t res = mat[(len1-1) * len2 + len2 - 1];
    free(mat);
    return res;    
}