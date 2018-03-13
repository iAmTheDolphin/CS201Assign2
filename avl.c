//
// Created by Parker Jones on 2/28/18.
//

#include "bst.h"
#include "avl.h"

struct avl {
    void (*display)(void *,FILE *);
    int (*compare)(void *,void *);
    void (*free)(void *);
    int balance;
    int count;
    void *value;

};

AVL *newAVL(
        void (*display)(void *,FILE *),           //display
        int (*compare)(void *,void *),            //comparator
        void (*free)(void *)) {



}