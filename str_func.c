#include <stdio.h>
#include <string.h>

char *my_strtok(char *str, char const *sep)
{
    static char *last;     // The core is static

    if(str == NULL){
        str = last;
        if(str == NULL)
            return NULL;
    } else{
        str += strspn(str, sep);
    }

    char *temp;
    temp = strpbrk(str, sep);
    if(temp != NULL){
        *temp = 0;
        last = temp + 1;
    } else{
        last = NULL;
    }
    return str;
}

int main()
{
    int len;
    char buffer[] = "1234,45345,123,4582";
    char *seg;
    char delim = ',';

    for(seg = my_strtok(buffer, &delim); seg != NULL; seg = my_strtok(NULL, &delim))
    {
        printf("Next token is %s\n", seg);
    }
}