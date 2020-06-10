#include <stdio.h>
#include <string.h>

int main()
{
    unsigned int mask[4];
    unsigned int ip1[4];
    unsigned int ip2[4];
    int res;
  
    while(scanf("%u.%u.%u.%u", &mask[0], &mask[1], &mask[2], &mask[3]) != EOF){
        scanf("%u.%u.%u.%u", &ip1[0], &ip1[1], &ip1[2], &ip1[3]);
        scanf("%u.%u.%u.%u", &ip2[0], &ip2[1], &ip2[2], &ip2[3]);
        res = 0;
        for(int i = 0; i < 4; i++){
            if(mask[i] > 0xff || ip1[i] > 0xff || ip2[i] > 0xff){
                printf("1\n");
                res = -1;
            }
        }
        if(res == 0){
             for(int i = 0; i < 4; i++){
            if(mask[i] == 0)
                break;
            if(mask[i] != 0 && (mask[i] & ip1[i]) != (mask[i] & ip2[i])){
                printf("2\n");
                res = -1;
                break;
            }
        }
        }
       
        if(res == 0)
            printf("0\n");
      
    }
}