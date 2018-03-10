#include <stdio.h>
#include "gst.h"
#include "integer.h"

int main() {

    GST *g = newGST(displayINTEGER, compareINTEGERmin, freeINTEGER);

    insertGST(g, newINTEGER(1));

    displayGST(g, stdout);

}