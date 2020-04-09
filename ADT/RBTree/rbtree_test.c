#include "rbtree.h"
#include <stdio.h>

int main()
{
    Node **root;
    root = create_rbtree();
    rbtree_insert(root, 50, NULL);
    Node *node_20 = create_node(20, NULL, RED, NULL, NULL, *root);
    (*root)->lchild = node_20;
    Node *node_10 = create_node(10, NULL, BLACK, NULL, NULL, node_20);
    Node *node_30 = create_node(30, NULL, BLACK, NULL, NULL, node_20);
    node_20->lchild = node_10;
    node_20->rchild = node_30;
    Node *node_40 = create_node(40, NULL, RED, NULL, NULL, node_30);
    node_30->rchild = node_40;
    Node *node_60 = create_node(60, NULL, BLACK, NULL, NULL, *root);
    (*root)->rchild = node_60;
    Node *node_70 = create_node(70, NULL, RED, NULL, NULL, node_60);
    node_60->rchild = node_70;
}
