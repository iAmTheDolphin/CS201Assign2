#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "avl.h"
#include "gst.h"

int
main(void)
{
/*

    GST *g = newGST(displayINTEGER, compareINTEGER, freeINTEGER);

    insertGST(g, newINTEGER(5));
    insertGST(g, newINTEGER(2));
    insertGST(g, newINTEGER(3));
    insertGST(g, newINTEGER(7));
    insertGST(g, newINTEGER(6));
    displayGST(g, stdout);
    deleteGST(g, newINTEGER(2));
    printf("\n");
    displayGST(g, stdout);

*/

    AVL *a = newAVL(displayINTEGER, compareINTEGER, freeINTEGER);

    insertAVL(a, newINTEGER(10));
    displayAVL(a, stdout);
    printf("\n");

    insertAVL(a, newINTEGER(5));
    displayAVL(a, stdout);
    printf("\n");
    insertAVL(a, newINTEGER(11));
    displayAVL(a, stdout);
    printf("\n");
    insertAVL(a, newINTEGER(4));
    displayAVL(a, stdout);
    printf("\n");
    insertAVL(a, newINTEGER(6));
    displayAVL(a, stdout);
    printf("\n");
    insertAVL(a, newINTEGER(7));
    displayAVL(a, stdout);

    printf("- - deleting AVL - -\n");
    deleteAVL(a, newINTEGER(6));
    displayAVL(a, stdout);
    printf("\n");



    return 0;
}
