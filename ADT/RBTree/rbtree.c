#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

#define rb_father(r)       ((r)->father)
#define rb_color(r)        ((r)->color)
#define is_red(r)       ((r)->color == RED)
#define is_black(r)     ((r)->color == BLACK)
#define set_red(r)      do{(r)->color = RED;} while(0)
#define set_black(r)    do{(r)->color = BLACK;} while(0)
#define set_father(r, p)    do{(r)->parent = (p);} while(0)
#define set_color(r, c)     do{(r)->color = (c);} while(0)

void swap(void **a, void ** b)
{
    void *tmp;
    tmp = *b;
    *b = *a;
    *a = tmp;
}

Node **create_rbtree(void)
{
    Node **root = malloc(sizeof(Node **));
    root = NULL;
    return root;
}

void preorder(RBTree tree)
{
    if(tree != NULL){
        printf("%d ", tree->key);
        preorder(tree->lchild);
        preorder(tree->rchild);
    }
}

void inorder(RBTree tree)
{
    if(tree != NULL){
        inorder(tree->lchild);
        printf("%d ", tree->key);
        inorder(tree->rchild);
    }
}

void postorder(RBTree tree)
{
    if(tree != NULL){
        postorder(tree->lchild);
        postorder(tree->rchild);
        printf("%d ", tree->key);
    }
}

RBTree rbtree_search(RBTree x, Type key)
{
    if(x == NULL || x->key == key)
        return x;
    if(key < x->key)
        return rbtree_search(x->lchild, key);
    else
        return rbtree_search(x->rchild, key);
}

static Node *minimum(RBTree tree)
{
    if(tree == NULL)
        return NULL;
    
    while(tree->lchild != NULL)
        tree = tree->lchild;
    return tree;
}

static Node *maximum(RBTree tree)
{
    if(tree == NULL)
        return NULL;
    
    while(tree->rchild != NULL)
        tree = tree->rchild;
    return tree;
}

/* 后继节点 */
static Node *successor(RBTree x)
{
    if(x->rchild != NULL)
        return minimum(x->rchild);
    /* 
       If x has no right child, there 2 possiblities:
       1. x is a left child, then its father is the successor
       2. x is a right child, then find its lowest father who has left child
    */
    Node *y = x->father;
    while((y != NULL) && (x == y->rchild)){
        x = y;
        y = y->father;
    }
    return y;
}

/* 前驱结点 */
static Node *predecessor(RBTree x)
{
    if(x->lchild != NULL)
        return maximum(x->lchild);
    
    Node *y = x->father;
    while((y != NULL) && (x == y->lchild)){
        x = y;
        y = y->father;
    }
    return y;
}

/* left rotate     
        px                  px
       /                   /
      x                   y
     / \       -->       / \
   lx   y               x  ry
       / \             / \
      ly ry           lx ly
*/
static void left_rotate(Node **root, Node *x)
{
    // y is x's right child
    Node *y = x->rchild;
    // Set y's left child as x's right child
    x->rchild = y->lchild;
    // If y's left child exists, set x as its father
    if(y->lchild != NULL)
        y->lchild->father = x;
    // Set x's father as y's father
    y->father = x->father;
    // If x's father is NULL, set y as root
    if(x->father == NULL){
        *root = y;
    }
    // Replace x's position with y
    else{
        if(x == x->father->lchild)
            x->father->lchild = y;
        else
            x->father->rchild = y;
    }
    // Set x as y's left child
    y->lchild = x;
    x->father = y;
}

/* right rotate     
        py                  py
       /                   /
      y                   x
     / \       -->       / \
    x  ry              lx   y
   / \                     / \
  lx rx                   rx ry
*/
static void right_rotate(Node **root, Node *y)
{
    // x is y's left child
    Node *x = y->lchild;
    // Set x's right child as y's left child
    y->lchild = x->rchild;
    // If x's right child exists, set y as its father
    if(x->rchild != NULL)
        x->rchild->father = y;
    // Set y's father as x's father
    x->father = y->father;
    // If y's father is NULL, set x as root
    if(y->father == NULL){
        *root = x;
    }
    // Replace y's position with x
    else{
        if(y == y->father->rchild)
            y->father->rchild = x;
        else
            x->father->lchild = x;
    }
    // Set y as x's left child
    x->rchild = y;
    y->father = x;
}

static void insert_fixup(Node **root, Node *node)
{
    Node *father, *gfather;

    while((father = rb_father(node)) && is_red(father)){
        gfather = rb_father(father);
        if(father == gfather->lchild){
            // Case 1: uncle is red
            Node *uncle = gfather->rchild;
            if(uncle && is_red(uncle)){
                set_black(uncle);
                set_black(father);
                set_red(gfather);
                node = gfather;
                continue;
            }
            // Case 2: uncle is black and node is right child
            if(node == father->rchild)
            {
                left_rotate(root, father);
                /*
                Node *tmp;
                tmp = father;
                father = node;
                node = tmp;
                */
                swap(&father, &node);
            }
            // Case 3: uncle is black and node is left child
            set_black(father);
            set_red(gfather);
            right_rotate(root, gfather);
        }
        else{
            // Case 1: uncle is red
            Node *uncle = gfather->lchild;
            if(uncle && is_red(uncle)){
                set_black(uncle);
                set_black(father);
                set_red(gfather);
                node = gfather;
                continue;
            }
            // Case 2: uncle is black and node is left child
            if(node == father->lchild){
                right_rotate(root, father);
                swap(&father, &node);
            }
            // Case 3: uncle is black and node is right child
            set_black(father);
            set_red(gfather);
            left_rotate(root, gfather);
        }
    }
    set_black(*root);
}

static Node *create_node(Type key, void *value)
{
    Node *node = (Node *)malloc(sizeof(*node));
    node->key = key;
    node->value = value;
    node->lchild = NULL;
    node->rchild = NULL;
    node->father = NULL;
    return node;
}

static void rbtree_insert_node(Node **root, Node *node)
{
    Node *y = NULL;
    Node *x = *root;
    // 1. Insert node to the tree 
    while(x != NULL)
    {
        y = x;
        if(node->key < x->key)
            x = x->lchild;
        else
            x = x->rchild;
    }
    rb_father(node) = y;
    if(y != NULL){
        if(node->key < y->key)
            y->lchild = node;
        else
            y->rchild = node; 
    }
    else{
        *root = node;
    }
    // 2. Set the color of node
    set_red(node);
    // 3. Fix up the RBTree
    insert_fixup(root, node);
}

/* Insert a node with key and value to RBTree */
int rbtree_insert(Node **root, Type key, void *value)
{
    Node *node = create_node(key, value);
    if(node == NULL)
        return -1;
    rbtree_insert_node(root, node);
}

static void delete_fixup(Node **root, Node *node, Node *father)
{

}

static void rbtree_delete_node(Node **root, Node *node)
{
    Node *child, *father;
    int color;

    // Both left and right children exist
    if((node->lchild != NULL) && (node->rchild != NULL)){
        Node *replace = node;
    }
}

void rbtree_delete(Node **root, Type key)
{
    Node *node;
    if((node = rbtree_search(*root, key)) != NULL)
        rbtree_delete_node(root, node);
}

void rbtree_destroy(RBTree tree)
{
    if(tree == NULL)
        return;
    if(tree->lchild != NULL)
        rbtree_destroy(tree->lchild);
    if(tree->rchild != NULL)
        rbtree_destroy(tree->rchild);
    free(tree);
}