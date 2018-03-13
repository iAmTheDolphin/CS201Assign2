//
// Created by Parker Jones on 2/28/18.
//

#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "avl.h"


static int debug = 1;


typedef struct avlnode AVLNODE;

struct avlnode {
    int balance;
    int height;
    int count;
    void *value;
    BST *tree;

    void (*display)(void *v, FILE *fp);

    int (*compare)(void *v, void *w);

    void (*freer)(void *n);
};

static AVLNODE *newAVLNODE(void *v,
                           BST *tree,
                           void (*display)(void *v, FILE *fp),
                           int (*compare)(void *v, void *w),
                           void (*freer)(void *n)) {
    AVLNODE *node = malloc(sizeof(AVLNODE));
    assert(node != 0);
    node->value = v;
    node->display = display;
    node->compare = compare;
    node->freer = freer;
    node->count = 1;
    node->balance = 0;
    node->height = 1;
    node->tree = tree;

    return node;
}


static void *getAVLNODEvalue(AVLNODE *n) {
    return n->value;
}

static int getAVLNODEcount(AVLNODE *n) {
    return n->count;
}

static void incrementAVLNODEcount(AVLNODE *n) {
    n->count++;
}

static void decrementAVLNODEcount(AVLNODE *n) {
    n->count--;
}

static void decrementAVLNODEbalance(AVLNODE *n) {
    n->balance--;
}

static void incrementAVLNODEbalance(AVLNODE *n) {
    n->balance++;
}

static int getAVLNODEbalance(AVLNODE *n) {
    return n->balance;
}

static void displayAVLNODE(void *n, FILE *fp) {
    ((AVLNODE *) n)->display(getAVLNODEvalue(n), fp);
    //printf("[%d]", getAVLNODEcount(n));
}

static int compareAVLNODE(void *v, void *w) {
    return ((AVLNODE *) v)->compare(getAVLNODEvalue(v), getAVLNODEvalue(w));
}

static void freeAVLNODE(void *n) {
    ((AVLNODE *) n)->freer(((AVLNODE *) n)->value);
    free(n);
}

static BST *getAVLNODEtree(AVLNODE *n) {
    if(n) return n->tree;
};

static void setAVLNODEheight(AVLNODE *n, int height) {
    if (n) n->height = height;
}

static int getAVLNODEheight(AVLNODE *n) {
    if (n) return n->height;
    else return 0;
}

static void setAVLNODEbalance(AVLNODE *n, int balance) {
    n->balance = balance;
}

static void swapAVLNODE(BSTNODE *bv, BSTNODE *bw) {
    //value and count need to switch not height and balance

    AVLNODE *v = getBSTNODEvalue(bv);
    AVLNODE *w = getBSTNODEvalue(bw);

    void *value = v->value;
    int count = v->count;

    v->value = w->value;
    v->count = w->count;

    w->value = value;
    w->count = count;
}


struct avl {
    void (*display)(void *, FILE *);

    int (*compare)(void *, void *);

    void (*free)(void *);

    BST *tree;
};

AVL *newAVL(
        void (*display)(void *, FILE *),           //display
        int (*compare)(void *, void *),            //comparator
        void (*free)(void *)) {

    AVL *a = malloc(sizeof(AVL));
    a->display = display;
    a->compare = compare;
    a->free = free;
    a->tree = newBST(displayAVLNODE, compareAVLNODE, swapAVLNODE, freeAVLNODE);
    return a;
}


static int isRightChild(BSTNODE *leaf) {
    if (getBSTNODEright(getBSTNODEparent(leaf)) == leaf) {
        return 1;
    }
    return 0;
}

static int isLeftChild(BSTNODE *leaf) {
    if (getBSTNODEleft(getBSTNODEparent(leaf)) == leaf) {
        return 1;
    }
    return 0;
}


static int maxHeight(BSTNODE *l, BSTNODE *r) {
    if (!l && !r) { //if it doesnt have a left or right child
        return 0;
    } else if (!r) {
        return getAVLNODEheight(getBSTNODEvalue(l));
    } else if (!l) {
        return getAVLNODEheight(getBSTNODEvalue(r));
    } else {
        int left = getAVLNODEheight(getBSTNODEvalue(l));
        int right = getAVLNODEheight(getBSTNODEvalue(r));
        return (left > right) ? left : right;
    }
}

static int getBalance(BSTNODE *lb, BSTNODE *rb) {
    AVLNODE *l = getBSTNODEvalue(lb);
    AVLNODE *r = getBSTNODEvalue(rb);

    return getAVLNODEheight(l) - getAVLNODEheight(r);
}


static void rightRotate(BSTNODE *n) {
    if(debug) printf("ROTATING RIGHT\n");
    BSTNODE *left = getBSTNODEleft(n);
    BSTNODE *parent = getBSTNODEparent(n);
    BSTNODE *leftRight = getBSTNODEright(left);

    if (parent && isLeftChild(n)) {
        if(debug) printf("Setting parents left child to L\n");
        setBSTNODEleft(parent, left);
        setBSTNODEparent(left, parent);
    } else if (parent) {
        if(debug) printf("Setting parents right child to L\n");
        setBSTNODEright(parent, left);
        setBSTNODEparent(left, parent);
    }
    else {
        if (debug) printf("There was no parent. Setting new Root\n");
        setBSTNODEparent(left, 0);
        setBSTroot(getAVLNODEtree(getBSTNODEvalue(n)),left);
    }

    setBSTNODEparent(n, left);
    setBSTNODEright(left, n);

    setBSTNODEparent(leftRight, n);
    setBSTNODEleft(n, leftRight);


    AVLNODE *nAVL = getBSTNODEvalue(n);
    AVLNODE *leftAVL = getBSTNODEvalue(left);
    AVLNODE *parentAVL = getBSTNODEvalue(parent);


    //reset the heights
    nAVL->height = maxHeight(getBSTNODEleft(n), getBSTNODEright(n)) + 1;
    if (debug) printf("Node N height is now %d\n", nAVL->height);
    if (leftAVL) {
        leftAVL->height = maxHeight(getBSTNODEleft(left), getBSTNODEright(left)) + 1;
        if (debug) printf("Node L height is now %d\n", leftAVL->height);
    }
    if (parentAVL) {
        parentAVL->height = maxHeight(getBSTNODEleft(parent), getBSTNODEright(parent)) + 1;
        if (debug) printf("Node P height is now %d\n", parentAVL->height);
    }

    //reset the balances
    nAVL->balance = getBalance(getBSTNODEleft(n), getBSTNODEright(n));
    if (debug) printf("Node N balance is now %d\n", nAVL->balance);
    if (leftAVL) {
        leftAVL->balance = getBalance(getBSTNODEleft(left), getBSTNODEright(left));
        if (debug) printf("Node L balance is now %d\n", leftAVL->balance);
    }
    if(parentAVL) {
        parentAVL->balance = getBalance(getBSTNODEleft(parent), getBSTNODEright(parent));
        if (debug) printf("Node P balance is now %d\n", parentAVL->balance);
    }
}


//recursively fixes the AVL up the tree
static void fixupAVL(BSTNODE *b) {

    AVLNODE *n = getBSTNODEvalue(b);
    BSTNODE *parent = getBSTNODEparent(b);

    while (parent != 0) {

        if(debug) {
            printf("Parent is now ");
            displayAVLNODE(getBSTNODEvalue(parent), stdout);
            printf("\n");
        }

        if(debug) printf("fixing parent\n");

        AVLNODE *p = getBSTNODEvalue(parent);
        BSTNODE *left = getBSTNODEleft(parent);
        BSTNODE *right = getBSTNODEright(parent);

        p->height = maxHeight(left, right) + 1;
        if(debug) printf("Parent height now %d\n", p->height);

        p->balance = getBalance(left, right);
        if(debug) printf("Parent balance now %d\n", p->balance);

        //left side is unbalanced
        if (p->balance > 1) {

            if (compareAVLNODE(n, getBSTNODEvalue(left)) > 0) { //LEFT RIGHT CASE
                if (debug) printf("LEFT RIGHT CASE\n");
                break;
            } else {  //LEFT LEFT CASE
                if (debug) printf("LEFT LEFT CASE\n");
                rightRotate(parent);
                break;
            }

        }
            //right side is unbalanced
        else if (p->balance < -1) {

            if (compareAVLNODE(n, getBSTNODEvalue(right)) < 0) { //RIGHT LEFT CASE
                if (debug) printf("RIGHT LEFT CASE\n");
                break;
            } else {
                if (debug) printf("RIGHT RIGHT CASE\n");
                break;

            }
        }
        else {
            if(debug) printf("No rotations required.\n");

        }


        parent = getBSTNODEparent(parent);
    }
}


void insertAVL(AVL *a, void *value) {

    AVLNODE *n = newAVLNODE(value, a->tree, a->display, a->compare, a->free);
    BSTNODE *bnode = findBST(a->tree, n);

    if (bnode) { //it already exists
        if(debug) printf("NODE already exists. incrementing count\n");
        incrementAVLNODEcount(getBSTNODEvalue(bnode));
        freeAVLNODE(n);
    } else { //it doesnt already exist
        bnode = insertBST(a->tree, n);
        if(debug) printf("NODE doesn't exist. inserted and fixing up\n");
        fixupAVL(bnode);

    }
}


void displayAVL(AVL *a,FILE *fp) {
    displayBSTdecorated(a->tree, fp);
}


