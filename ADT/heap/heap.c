#include "heap.h"
#include <stdio.h>

#define HEAP_SIZE   100

#define parent(i)       (((i) - 1) / 2)
#define left(i)         (2 * (i) + 1)
#define right(i)        (2 * (i) + 2)
#define swap(a, b)      do{     \
                            HEAP_TYPE t;    \
                            t = a;      \
                            a = b;      \
                            b = t;      \
                        } while(0)

HEAP_TYPE heap[HEAP_SIZE];
static int size = 0;

void print_heap(void)
{
    for(int i = 0; i < size; i++)
        printf("%d ", heap[i]);
    putchar('\n');
}

void insert(HEAP_TYPE value)
{
    int idx;

    heap[size++] = value;
    idx = size - 1;
    while(idx != 0){
        int parent_idx = parent(idx);
        if(heap[idx] > heap[parent_idx]){
            swap(heap[idx], heap[parent_idx]);
            idx = parent_idx;
        }
        else
            break;
    }
    printf("Heap after insertion: ");
    print_heap();
}

void delete()
{
    if(size < 1)
        return;
    else if(size > 1){
        swap(heap[size - 1], heap[0]);
        size--;
        for(int idx = 0; idx < size; )
        {
            int left_idx = left(idx);
            int right_idx = right(idx);
            int idx_swap;
            if(left_idx > size - 1)
                break;
            else if(right_idx > size - 1){
                idx_swap = left_idx;
            }
            else
                idx_swap = heap[left_idx] > heap[right_idx]? 
                            left_idx : right_idx;
            if(heap[idx] > heap[idx_swap])
                break;
            swap(heap[idx], heap[idx_swap]);
            idx = idx_swap;
        }
        printf("Heap after delete: ");
        print_heap();
    }
    else
        size--;
}

int main(void)
{
    insert(3);
    insert(1);
    insert(10);
    insert(2);
    insert(15);
    insert(11);
    insert(20);
    delete();
    delete();
}