#include "rbtree.h"
#include <stdio.h>

#define TEST2
// TODO 野指针的问题
int main()
{
    Node **root;
    root = create_rbtree();

    #ifdef TEST1
    Node *node_50 = create_node(50, NULL, BLACK);
    *root = node_50;
    Node *node_20 = create_node(20, NULL, RED);
    Node *node_10 = create_node(10, NULL, BLACK);
    Node *node_30 = create_node(30, NULL, BLACK);
    Node *node_40 = create_node(40, NULL, RED);
    Node *node_60 = create_node(60, NULL, BLACK);
    Node *node_70 = create_node(70, NULL, RED);
    insert_node(node_50, node_20, node_60, NULL);
    insert_node(node_20, node_10, node_30, node_50);
    insert_node(node_10, NULL, NULL, node_20);
    insert_node(node_30, NULL, node_40, node_20);
    insert_node(node_40, NULL, NULL, node_30);
    insert_node(node_60, NULL, node_70, node_50);
    insert_node(node_70, NULL, NULL, node_60);
    rbtree_delete(root, 50);
    rbtree_delete(root, 70);
    rbtree_delete(root, 60);
    rbtree_delete(root, 10);
    rbtree_delete(root, 20);
    inorder(*root);
    printf("\n");
    #endif

    #ifdef TEST2
    for(int i = 0; i < 9; i++)
        rbtree_insert(root, (i + 1) * 10, NULL);
    rbtree_delete(root, 50);
    inorder(*root);
    printf("\n");
    print_tree(*root, 0, 0);
    #endif

    getchar();
    rbtree_destroy(*root);
    return 0;
}
