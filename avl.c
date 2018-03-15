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
    if(n)((AVLNODE *) n)->display(getAVLNODEvalue(n), fp);
    //printf("[%d]", getAVLNODEcount(n));
}

static int compareAVLNODE(void *v, void *w) {
    if(w && v)return ((AVLNODE *) v)->compare(getAVLNODEvalue(v), getAVLNODEvalue(w));
    else if(w) return -1;
    else if(v) return 1;
    else return 0;
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

    int duplicates;
};



AVL *newAVL(
        void (*display)(void *, FILE *),           //display
        int (*compare)(void *, void *),            //comparator
        void (*free)(void *)) {

    AVL *a = malloc(sizeof(AVL));
    a->display = display;
    a->compare = compare;
    a->free = free;
    a->duplicates = 0;
    a->tree = newBST(displayAVLNODE, compareAVLNODE, swapAVLNODE, freeAVLNODE);
    return a;
}


static int isRightChild(BSTNODE *leaf) {

    if(compareAVLNODE(getBSTNODEvalue(leaf), getBSTNODEvalue(getBSTNODEright(getBSTNODEparent(leaf)))) == 0)
        return 1;
    else return 0;
}

static int isLeftChild(BSTNODE *leaf) {
    if(compareAVLNODE(getBSTNODEvalue(leaf), getBSTNODEvalue(getBSTNODEleft(getBSTNODEparent(leaf)))) == 0)
        return 1;
    else return 0;
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

static void leftRotate(BSTNODE *n) {
    if(debug) printf("ROTATING LEFT\n");
    BSTNODE *right = getBSTNODEright(n);
    BSTNODE *parent = getBSTNODEparent(n);
    BSTNODE *rightLeft = getBSTNODEleft(right);

    if (parent && isLeftChild(n)) {
        if(debug) printf("Setting parents left child to R\n");
        setBSTNODEleft(parent, right);
        setBSTNODEparent(right, parent);
    } else if (parent) {
        if(debug) printf("Setting parents right child to R\n");
        setBSTNODEright(parent, right);
        setBSTNODEparent(right, parent);
    }
    else {
        if (debug) printf("There was no parent. Setting new Root\n");
        setBSTNODEparent(right, 0);
        setBSTroot(getAVLNODEtree(getBSTNODEvalue(n)),right);
    }

    setBSTNODEparent(n, right);
    setBSTNODEleft(right, n);

    setBSTNODEparent(rightLeft, n);
    setBSTNODEright(n, rightLeft);


    AVLNODE *nAVL = getBSTNODEvalue(n);
    AVLNODE *rightAVL = getBSTNODEvalue(right);
    AVLNODE *parentAVL = getBSTNODEvalue(parent);


    //reset the heights
    nAVL->height = maxHeight(getBSTNODEleft(n), getBSTNODEright(n)) + 1;
    if (debug) printf("Node N height is now %d\n", nAVL->height);
    if (rightAVL) {
        rightAVL->height = maxHeight(getBSTNODEleft(right), getBSTNODEright(right)) + 1;
        if (debug) printf("Node R height is now %d\n", rightAVL->height);
    }
    if (parentAVL) {
        parentAVL->height = maxHeight(getBSTNODEleft(parent), getBSTNODEright(parent)) + 1;
        if (debug) printf("Node P height is now %d\n", parentAVL->height);
    }

    //reset the balances
    nAVL->balance = getBalance(getBSTNODEleft(n), getBSTNODEright(n));
    if (debug) printf("Node N balance is now %d\n", nAVL->balance);
    if (rightAVL) {
        rightAVL->balance = getBalance(getBSTNODEleft(right), getBSTNODEright(right));
        if (debug) printf("Node R balance is now %d\n", rightAVL->balance);
    }
    if(parentAVL) {
        parentAVL->balance = getBalance(getBSTNODEleft(parent), getBSTNODEright(parent));
        if (debug) printf("Node P balance is now %d\n", parentAVL->balance);
    }
}


//uses a loop and fixes the tree from the leaf up to the root
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
                leftRotate(getBSTNODEleft(parent));
                rightRotate(parent);
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
                rightRotate(getBSTNODEright(parent));
                leftRotate(parent);
                break;
            } else {
                if (debug) printf("RIGHT RIGHT CASE\n");
                leftRotate(parent);
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
        a->duplicates++;
        freeAVLNODE(n);
    } else { //it doesnt already exist
        bnode = insertBST(a->tree, n);
        if(debug) printf("NODE doesn't exist. inserted and fixing up\n");
        fixupAVL(bnode);
    }
}

int findAVLcount(AVL *a,void *v) {
    AVLNODE *n = newAVLNODE(v,a->tree , a->display, a->compare, a->free);
    BSTNODE *b = findBST(a->tree, n);
    free(n);
    return getAVLNODEcount(getBSTNODEvalue(b));
}

void *findAVL(AVL *a,void *v) {
    AVLNODE *n = newAVLNODE(v,a->tree , a->display, a->compare, a->free);
    BSTNODE *b = findBST(a->tree, n);
    free(n);
    return getAVLNODEvalue(getBSTNODEvalue(b));
}



int duplicatesAVL(AVL *a) {
    if(a) return a->duplicates;
    else return 0;
}




void displayAVL(AVL *a,FILE *fp) {
    displayBSTdecorated(a->tree, fp);
}


static int favorite(BSTNODE *parent, BSTNODE *child) {
    if((getAVLNODEbalance(getBSTNODEvalue(parent)) > 0 && isLeftChild(child))
       || (getAVLNODEbalance(getBSTNODEvalue(parent)) < 0 && isRightChild(child))) return 1;
    else return 0;
}


static void deleteFixup(BSTNODE *b) {

    BSTNODE *parent = getBSTNODEparent(b);

    while(parent != 0) {



        AVLNODE *p = getBSTNODEvalue(parent);

        if(p->balance == 0) { //the parent is balanced
            p->balance = getBalance(getBSTNODEleft(parent), getBSTNODEright(parent));
            break;
        }
        else { //the parent is not balanced



        }

    }

}


void *deleteAVL(AVL *a,void *v) {

    AVLNODE *n = newAVLNODE(v, a->tree, a->display, a->compare, a->free);
    BSTNODE *b = findBST(a->tree, n);
    void *temp;
    free(n);
    n = getBSTNODEvalue(b);
    temp = getAVLNODEvalue(n);

    if(n->count > 1) {
        if (debug) printf("count is >1. decrementing\n");
        decrementAVLNODEcount(n);
    }
    else {
        if (debug) printf("swapping to leaf\n");
        b = swapToLeafBST(a->tree, b);
        if(debug) {
            displayAVL(a, stdout);
            printf("deleting leaf. \n");
        }
        pruneLeafBST(a->tree, b);
        //freeBSTNODE(b, 0);
        if(debug) {
            displayAVL(a, stdout);
            printf("-------- \n");
        }
        //setBSTsize(a->tree, sizeBST(a->tree) - 1);
        fixupAVL(b);

    }
    return temp;
}

