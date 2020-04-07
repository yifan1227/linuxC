#ifndef RBTREE_H
#define RBTREE_H

#define RED     0
#define BLACK   1

typedef int Type;

typedef struct RBTreeNode{
    unsigned char color;
    Type key;
    void *value;
    struct RBTreeNode *lchild;
    struct RBTreeNode *rchild;
    struct RBTreeNode *father;
}Node, *RBTree;

#endif