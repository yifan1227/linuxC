#include <stdio.h>
#include <stdlib.h>

#define NODES_STATUS 49
#define NODE_TREE    50
int main()
{
    char choice;
    while(1){
        printf("Choose an operation:\n");
        printf("1.Print the nodes status\n");
        printf("2.Generate the node tree\n");
        while((choice = getchar()) != '\n')
        {
            switch (choice)
            {
            case NODES_STATUS:
                printf("hahaha123\n");
                break;
            
            default:
                break;
            }
        }
        printf("****************************************************\n");
        printf("press 'ENTER' to exit");
        while((choice = getchar() != '\n'))
        {
        }
        system("clear");
    }
}