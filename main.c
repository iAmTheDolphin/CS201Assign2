#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "avl.h"

int
main(void)
{
    AVL *a = newAVL(displayINTEGER, compareINTEGER, freeINTEGER);

    insertAVL(a, newINTEGER(100));
    displayAVL(a, stdout);
    printf("\n");

    insertAVL(a, newINTEGER(99));
    displayAVL(a, stdout);
    printf("\n");
    insertAVL(a, newINTEGER(98));
    displayAVL(a, stdout);
    printf("\n");
    insertAVL(a, newINTEGER(97));
    displayAVL(a, stdout);
    printf("\n");
    insertAVL(a, newINTEGER(96));
    displayAVL(a, stdout);
    printf("\n");
    insertAVL(a, newINTEGER(95));
    displayAVL(a, stdout);
    printf("\n");


    return 0;
}
