#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int comp(const void *a, const void *b){
    return *(int *)b - *(int *)a;
}

void helper(char *name)
{
    int arr[26];
    char *cur = name;
    int beauty = 26;
    int sum = 0;
    
    memset(arr, 0, 26 * 4);
    for(cur = name; *cur != '\0'; cur++){
        if(isupper(*cur))
            *cur = tolower(*cur);
        arr[*cur - 'a']++;        
    }
    qsort(arr, 26, sizeof(int), comp);
    for(int i = 0; i < 26; i++){
        if(arr[i] != 0){
            sum += arr[i] * beauty;
            beauty--;
        }
        else{
            break;
        }
    }
    printf("%d\n", sum);
}

int main()
{
    char name[100000];
    
    int num;
    
    scanf("%d", &num);
    
    for(int i = 0; i < num; i++){
        scanf("%s", name);
        helper(name);
    }
}