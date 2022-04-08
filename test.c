#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int c;
    system ("/bin/stty raw");
    while((c=getchar())!='~')
    {
        printf("%c",c);
    }
    system ("/bin/stty cooked");
    printf("\n");
    return 0;
}