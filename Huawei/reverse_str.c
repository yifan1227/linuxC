#include <stdio.h>
#include <string.h>
int main(){   
    char str[1000];
    char *p;
    gets(str);

    while(p = strrchr(str,' ')){
        printf("%s ",p+1);
        *p='\0';
    }
    printf("%s",str);
    return 0;
}
