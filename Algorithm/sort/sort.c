#include <stdio.h>
#include <stdlib.h>

void print_array(char *string, int *array, int size)
{
    printf("%s", string);
    for(int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");
}

void bubble_sort(int *array, int size)
{
    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size - i - 1; j++){
            if(array[j] > array[j + 1]){
                int tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
       }
    }
}

void quick_sort(int *array, int left, int right)
{
    if(left > right)
        return;
    
    int l = left, r = right;
    int base = array[l];

    while(l < r)
    {
        while(l < r && array[r] >= base)
            r--;
        
        if(l < r)
            array[l++] = array[r];

        while(l < r && array[l] < base)
            l++;
        
        if(l < r)
            array[r--] = array[l];
    }
    array[l] = base;

    quick_sort(array, left, l - 1);
    quick_sort(array, l + 1, right);
}

int main()
{
    int array[20];
    int size = 20;

    for(int i = 0; i < size; i++)
        array[i] = rand() % 100;

    print_array("Before sort: ", array, size);
    
    //bubble_sort(array, size);
    quick_sort(array, 0, size - 1);
    print_array("After sort: ", array, size);
}