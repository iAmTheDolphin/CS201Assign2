#include <stdio.h>
#include "gst.h"
#include "integer.h"

int main() {

    GST *g = newGST(displayINTEGER, compareINTEGERmin, freeINTEGER);

    insertGST(g, newINTEGER(5));
    insertGST(g, newINTEGER(2));
    insertGST(g, newINTEGER(1));
    insertGST(g, newINTEGER(6));
    insertGST(g, newINTEGER(10));
    insertGST(g, newINTEGER(1));
    insertGST(g, newINTEGER(5));

    displayGST(g, stdout);

}