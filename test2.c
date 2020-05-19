#include <stdio.h>
#include <string.h>

void print(char *str, int len)
{
    if(len < 8){
        for(int i = 0; i < len; i++){
            putchar(*str);
            str++;
        }
        for(int i = 0; i < 8 - len; i++){
            putchar('0');
        }
    }
    else{
        for(int i = 0; i < 8; i++){
            putchar(*str);
            str++;
        }
    }
    putchar('\n');
}

int main()
{
    char str[101];
    char *cur;
    
    for(int i = 0; i < 2; i++){
        gets(str);
        cur = str;
        int left = strlen(str);
        while(left > 0){
            if(left >= 8)
                print(cur, 8);
            else{
                print(cur, left);
            }
            left -= 8;
            cur += 8;
        }
    }
}