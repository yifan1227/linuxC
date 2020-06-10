#include <stdio.h>
#include <string.h>

void main()
{
    char str[1000];
    char c;
    int num = 0;
    
    memset(str, 0, sizeof(str));
    //scanf("%s", str);
    fgets(str, 1000, stdin);
    c = getchar();
    
    int len = strlen(str);
    char *p = str;
    
    if(c >= 'A' && c <= 'Z'){
        while(*p != '\0'){
            if(*p == c || *p == c + 32)
                num++;
            p++;
        }
    }
    else if(c >= 'a' && c <= 'z'){
        while(*p != '\0'){
            if(*p == c || *p == c - 32)
                num++;
            p++;
        }
    }
    else{
        while(*p != '\0'){
            if(*p == c)
                num++;
            p++;
        }
    }
    printf("%d", num);
    return;
}