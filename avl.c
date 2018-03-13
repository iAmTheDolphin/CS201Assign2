//
// Created by Parker Jones on 2/28/18.
//

#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "avl.h"


typedef struct avlnode AVLNODE;

struct avlnode {
    int balance;
    int height;
    int count;
    void *value;

    void (*display)(void *v, FILE *fp);

    int (*compare)(void *v, void *w);

    void (*freer)(void *n);
};

static AVLNODE *newAVLNODE(void *v,
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

static void setAVLNODEheight(AVLNODE *n, int height) {
    if(n) n->height = height;
}

static int getAVLNODEheight(AVLNODE *n) {
    if(n) return n->height;
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
    if(!l && !r) { //if it doesnt have a left or right child
        return 0;
    }
    else if(!r) {
        return getAVLNODEheight(getBSTNODEvalue(l));
    }
    else if(!l) {
        return getAVLNODEheight(getBSTNODEvalue(r));
    }
    else {
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


//recursively fixes the AVL up the tree
static void fixupAVL (BSTNODE *b) {

    AVLNODE *n = getBSTNODEvalue(b);
    BSTNODE *parent = getBSTNODEparent(b);

    while(parent != 0) {
        AVLNODE *p = getBSTNODEvalue(parent);
        BSTNODE *left = getBSTNODEleft(parent);
        BSTNODE *right = getBSTNODEright(parent);

        p->height = maxHeight(left, right) + 1;

        p->balance = getBalance(left, right);

        //left side is unbalanced
        if(p->balance > 1) {



        }
            //right side is unbalanced
        else if(p->balance < -1) {

        }



        parent = getBSTNODEparent(parent);
    }
}


void insertAVL(AVL *a, void *value) {

    AVLNODE *n = newAVLNODE(value, a->display, a->compare, a->free);
    BSTNODE *bnode = findBST(a->tree, n);

    if (bnode) { //it already exists
        incrementAVLNODEcount(getBSTNODEvalue(bnode));
        freeAVLNODE(n);
    } else { //it doesnt already exist
        bnode = insertBST(a->tree, n);
        fixupAVL(bnode);

    }


}


