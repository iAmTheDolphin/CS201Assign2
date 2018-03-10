//
// Created by Parker Jones on 2/28/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "gst.h"
#include "bst.h"

typedef  struct gstnode GSTNODE;

struct gstnode {
    int count;
    void *value;
    void (*display)(void *v,FILE *fp);
    int (* compare) (void *v, void *w);
    void (*freer)(void *n);
};

static GSTNODE *newGSTNODE(void *v ,
                          void (*display)(void *v,FILE *fp),
                          int (* compare) (void *v, void *w),
                          void (*freer)(void *n)) {
    GSTNODE *node = malloc(sizeof(GSTNODE));
    assert(node != 0);
    node->value = v;
    node->display = display;
    node->compare = compare;
    node->freer = freer;
    node->count = 1;
    return node;
}


static void *getGSTNODEvalue(GSTNODE *n) {
    return n->value;
}

static int getGSTNODEcount(GSTNODE *n) {
    return n->count;
}

static void incrementGSTNODEcount(GSTNODE *n) {
    n->count++;
}

static void displayGSTNODE(void *n, FILE *fp) {
    ((GSTNODE *) n)->display(getGSTNODEvalue(n), fp);
    printf("(%d)", getGSTNODEcount(n));
}

static int compareGSTNODE(void *v, void *w) {
    return ((GSTNODE *) v)->compare(getGSTNODEvalue(v), getGSTNODEvalue(w));
}

static void freeGSTNODE(void *n) {
    ((GSTNODE *)n)->freer(((GSTNODE *)n)->value);
    free(n);
}


struct gst {
    int size;
    BST *tree;
    void (*display)(void *v,FILE *fp);
    int (* compare) (void *v, void *w);
    void (*freer)(void *n);

};


GST *newGST(
        void (*display)(void *v,FILE *fp),           //display
        int (*comparator)(void *v,void *w),            //comparator
        void (*freer)(void *n)) {

    GST *g = malloc(sizeof(GST));
    assert(g != 0);

    g->size = 0;
    g->display = display;
    g->compare = comparator;
    g->freer = freer;
    g->tree = newBST(displayGSTNODE, compareGSTNODE, 0, freeGSTNODE);
    return g;
}


void insertGST(GST *g,void *v) {

    // needs to make a new BSTNODE

    //needs to call findBST
    //if it exists, then it needs to increment the count
    //if it doesnt, call insertBST

    GSTNODE *n = newGSTNODE(v, g->display, g->compare, g->freer);
    BSTNODE *bnode = findBST(g->tree, n);
    if(bnode) {
        incrementGSTNODEcount(getBSTNODEvalue(bnode));
        free(n);
    }
    else {
        insertBST(g->tree, n);
    }

}

void displayGST(GST *g,FILE *fp) {
    displayBSTdebug(g->tree, fp);
}


int findGSTcount(GST *g,void *v) {

    //calls findGST then gets the count out of it

}


void *findGST(GST *g,void *v) {

    //needs to call findBST

    //to do that we need to recreate the node

}

void *deleteGST(GST *g,void *v) {


}


int sizeGST(GST *g) {
    //returns the size of the gst
    return g->size;
}