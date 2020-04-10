#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

#define rb_father(r)        ((r)->father)
#define rb_color(r)         ((r)->color)
#define is_red(r)           (((r) != NULL) && ((r)->color == RED))
#define is_black(r)         ((((r) != NULL) && ((r)->color == BLACK)) || ((r) == NULL))
#define set_red(r)          do {if(r){(r)->color = RED;}} while(0)
#define set_black(r)        do {if(r){(r)->color = BLACK;}} while(0)
#define set_father(r, p)    do {(r)->parent = (p);} while(0)
#define set_color(r, c)     do {if(r){(r)->color = (c);}} while(0)

/* compare function 
    a > b return 1;    
    a = b return 0;
    a < b return -1;
*/
int (*compare)(void *a, void *b);

static void swap(void **a, void ** b)
{
    void *tmp;
    tmp = *b;
    *b = *a;
    *a = tmp;
}

/* Replace node a with b */
static void replace_node(Node *a, Node *b)
{
    a->key = b->key;
    a->value = b->value;
}

Node **create_rbtree(void)
{
    Node **root = malloc(sizeof(Node **));
    *root = NULL;
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

void print_tree(RBTree tree, Type key, int direction)
{
    if (tree != NULL){
        if(direction == 0){
            printf("%2d (B) is root\n", tree->key);
        }
        else{
            printf("%d (%s) is %d's %6s child\n", tree->key,
                    is_black(tree)?"B" : "R", key,
                    direction == 1? "right" : "left");
        }
        print_tree(tree->lchild, tree->key, -1);
        print_tree(tree->rchild, tree->key, 1);
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

Node *create_node(Type key, void *value, int color)
{
    Node *node = (Node *)malloc(sizeof(*node));
    node->key = key;
    node->value = value;
    node->color = color;
    node->lchild = NULL;
    node->rchild = NULL;
    node->father = NULL;
    return node;
}

void insert_node(Node *node, Node *lchild, Node *rchild, Node *father)
{
    node->lchild = lchild;
    node->rchild = rchild;
    node->father = father;
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
        else if(node->key > x->key)
            x = x->rchild;
        else
            x->value = node->value;
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
    Node *node = create_node(key, value, RED);
    if(node == NULL)
        return -1;
    rbtree_insert_node(root, node);
}

static void delete_fixup(Node **root, Node *node)
{
    Node *brother, *father;

    while(node != *root){
        father = rb_father(node);
        // Node is left child
        if(node == father->lchild){
            brother = father->rchild;    
            // 兄黑
            if(is_black(brother)){
                // 兄子全黑
                if(is_black(brother->lchild) && is_black(brother->rchild)){
                    // 父黑，涂红兄，向上递归父
                    if(is_black(father)){
                        set_red(brother);
                        node = father;
                        continue;
                    } 
                    // 父红，交换父兄颜色
                    else{
                        set_red(brother);
                        set_black(father);
                        break;
                    }
                }
                // 兄右子为红，以父为支点左旋，兄染父色，父染黑
                if(is_red(brother->rchild)){
                    set_color(brother, is_black(father)? BLACK : RED);
                    set_black(father);
                    set_black(brother->rchild);
                    left_rotate(root, father);
                    break;
                }
                // 兄左子为红，右子为黑
                if(is_red(brother->lchild) && is_black(brother->rchild)){
                    set_black(brother->lchild);
                    set_red(brother);
                    right_rotate(root, brother);
                    continue;
                }
            }
            //兄红
            else{
                set_black(brother);
                set_red(father);
                left_rotate(root, father);
                continue;
            }
        }
        // Node is right child, symmetrically
        else{
            brother = father->lchild;
            if(is_black(brother)){
                // 兄子全黑
                if(is_black(brother->lchild) && is_black(brother->rchild)){
                    // 父黑，涂红兄，向上递归父
                    if(is_black(father)){
                        set_red(brother);
                        node = father;
                        continue;
                    } 
                    // 父红，交换父兄颜色
                    else{
                        set_red(brother);
                        set_black(father);
                        break;
                    }
                }
                // 兄左子为红，以父为支点右旋，兄染父色，父染黑
                if(is_red(brother->lchild)){
                    set_color(brother, is_black(father)? BLACK : RED);
                    set_black(father);
                    set_black(brother->lchild);
                    right_rotate(root, father);
                    break;
                }
                // 兄右子为红，左子为黑
                if(is_red(brother->rchild) && is_black(brother->lchild)){
                    set_black(brother->rchild);
                    set_red(brother);
                    left_rotate(root, brother);
                    continue;
                }
            }
            //兄红
            else{
                set_black(brother);
                set_red(father);
                right_rotate(root, father);
                continue;
            }
        }
    }
}

static void rbtree_delete_node(Node **root, Node *node)
{
    /* Node to replace the node to be deleted */
    Node *replace;
    int color;

    // Both left and right children exist, pick successor as replace
    if((node->lchild != NULL) && (node->rchild != NULL)){
        Node *replace = successor(node);
        // Node is not root
        if(rb_father(node)){
            replace_node(node, replace);
        } else
            replace_node(*root, replace);
        rbtree_delete_node(root, replace);
        return;
    }
    // Node has no child
    else if((node->lchild == NULL) && (node->rchild == NULL)){
        // Node is root and has no child
        if(!rb_father(node)){
            *root = NULL;
            free(node);
            return;
        }
        // Node is black
        if(is_black(node))
            delete_fixup(root, node);
        if(node == rb_father(node)->lchild)
            rb_father(node)->lchild = NULL;
        else
            rb_father(node)->rchild = NULL;
        free(node);
        return;
    }
    // Node has only 1 child, node must be black
    else{
        replace = node->lchild? node->lchild : node->rchild;
        if(rb_father(node)){
            if(node == rb_father(node)->lchild)
                rb_father(node)->lchild = replace;
            else
                rb_father(node)->rchild = replace;
            replace->father = rb_father(node);
        }
        else
            *root = replace;
        set_black(replace);
        free(node);
        return;
    }
}

void rbtree_delete(Node **root, Type key)
{
    if(*root == NULL)
        return;
    Node *node;
    if((node = rbtree_search(*root, key)) != NULL){
        rbtree_delete_node(root, node);
    }
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