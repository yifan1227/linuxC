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

Node **create_rbtree(void);

/* Traverse the tree */
void preorder(RBTree tree);

void inorder(RBTree tree);

void postorder(RBTree tree);

void print_tree(RBTree tree, Type key, int direction);

Node *create_node(Type key, void *value, int color);

/* Insert node to specific position, for test */
void insert_node(Node *node, Node *lchild, Node *rchild, Node *father);

int rbtree_insert(Node **root, Type key, void *value);

void rbtree_delete(Node **root, Type key);

void rbtree_destroy(RBTree tree);

#endif