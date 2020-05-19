#include <stdio.h>
#include <string.h>

int main()
{
    int num;
    int number;
    int arr[1000];
    
    memset(arr, 0, 1000 * sizeof(int));
    while(scanf("%d", &num) != EOF){
        for(int i = 0; i < num; i++){
            scanf("%d", &number);
            if(arr[number] != 1)
                arr[number] = 1;
        }
        
    }
    for(int i = 0; i < 1000; i++){
            if(arr[i] == 1){
                printf("%d\n", i);
            }
        }
}