#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "queue.h"
#include "bst.h"
#include "avl.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
{
    srandom(43);
    //simple STRING test of AVL
    AVL *p = newAVL(displaySTRING,compareSTRING,freeSTRING);
    insertAVL(p,newSTRING("a2"));
    insertAVL(p,newSTRING("a3"));
    insertAVL(p,newSTRING("a1"));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    STRING *q = newSTRING("a2");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    STRING *r = newSTRING("a3");


    STRING *l = deleteAVL(p, r);
    freeSTRING(l);


    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    setSTRING(r,"a2");
    freeSTRING(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeAVL(p));
    freeSTRING(q);
    freeSTRING(r);
    freeAVL(p);
    return 0;
}
























/*
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include "scanner.h"
#include "avl.h"
#include "gst.h"
#include "string.h"
#include "integer.h"

#include <unistd.h>

*/
/* options *//*

static int Strings = 1;
static int debugMAIN = 0;
static int avl = 1;
char *filename;




void Fatal(char *fmt, ...)
{
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}





static void printName() {
    printf("Parker Jones\n");
    exit(0);
}



static int
processOptions(int argc, char **argv)
{
    int argIndex;
    int argUsed;
    int separateArg;
    argIndex = 1;


    while (argIndex < argc && *argv[argIndex] == '-')
    {
        if(debugMAIN) printf("Checking Options\n");

        */
/* check if stdin, represented by "-" is an argument *//*

        */
/* if so, the end of options has been reached *//*

        if (argv[argIndex][1] == '\0') return argIndex;

        separateArg = 0;
        argUsed = 0;

        if (argv[argIndex][2] == '\0')
        {
            //filename = argv[argIndex+1];
            separateArg = 1;
        }
        //filename = argv[argIndex]+2;

        switch (argv[argIndex][1])
        {
            */
/*
             * when option has an argument, do this
             *
             *     examples are -m4096 or -m 4096
             *
             *     case 'm':
             *         MemorySize = atol(arg);
             *         argUsed = 1;
             *         break;
             *
             *
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             *//*


                break;
            case 'g':
                avl = 0;
                argUsed=1;
                break;
            case 'r':
                avl = 1;
                argUsed=1;
                break;
            case 'v':
                printName();
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }

        if (separateArg && argUsed){
            if(debugMAIN) printf("separate arg and argUsed\n");
            //++argIndex;
        }

        ++argIndex;
    }
    if(debugMAIN) printf("%d\n", argIndex);

    return argIndex;
}



int
main(int argc,char **argv)
{
    if(debugMAIN)printf("First Arg: %s\nSecond Arg: %s\n", argv[1], argv[2]);
    FILE *fp;
    void (*display)(void *, FILE *);
    void (*specialfree)(void *);
    int argIndex;
    void *tree;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);


    if (argIndex == argc)
        printf("No arguments\n");
    else
    {
        filename = argv[argIndex];
        if(debugMAIN) {
            printf("Remaining arguments:\n");
            for (int i = argIndex; i < argc; ++i)
                printf("    %s\n", argv[i]);
        }
    }

    if(debugMAIN) printf("Trying to open %s ", filename);
    fp = fopen(filename, "r");
    if(!fp && debugMAIN) printf("couldn't open file!\n");
    else if(debugMAIN) printf("opened file!\n");

    if(avl) {
        AVL *a = newAVL(displaySTRING, compareSTRING, freeSTRING);
        tree = a;

        if (Strings) { //it is a file of strings
            if (debugMAIN) printf("inserting strings\n");
            char *temp;
            if (stringPending(fp)) {
                temp = readString(fp);
            } else {
                temp = readToken(fp);
            }



            while (!feof(fp)) {
                if (debugMAIN) printf("adding : %s\n", temp);
                insertAVL(a, newSTRING(temp));
                if (stringPending(fp)) {
                    temp = readString(fp);
                } else {
                    temp = readToken(fp);
                }
            }
//        if(debugMAIN) printf("adding : %s\n", temp);
//        insertHEAP(h, newSTRING(temp));
            if (debugMAIN) printf("items loaded. displaying avl...\n");
        } else { //it is an integer file
            a = newAVL(displayINTEGER, compareINTEGER, freeINTEGER);
            int temp = 0;
            temp = readInt(fp);
            while (!feof(fp)) {
                if (debugMAIN) printf("adding : %d\n", temp);
                insertAVL(a, newINTEGER(temp));
                temp = readInt(fp);
            }
            display = displayINTEGER;
            specialfree = freeINTEGER;
        }
        if(debugMAIN)displayAVL(a, stdout);
        //freeAVL(a);

    }
    else {
        GST *g = newGST(displaySTRING, compareSTRING, freeSTRING);
        tree = g;

        if (debugMAIN) printf("inserting strings\n");
        char *temp;
        if (stringPending(fp)) {
            temp = readString(fp);
        } else {
            temp = readToken(fp);
        }

        if (debugMAIN)printf("cleaning strings\n");


        while (!feof(fp)) {
            if (debugMAIN) printf("adding : %s\n", temp);
            insertGST(g, newSTRING(temp));
            if (stringPending(fp)) {
                temp = readString(fp);
            } else {
                temp = readToken(fp);
            }
        }
        if (debugMAIN) {
            printf("items loaded. displaying gst...\n");
            displayGST(g, stdout);
        }

    }

    */
/*    Done loading in words from file    *//*



    */
/*    loading in commands    *//*

    filename = argv[argIndex+1];
    if(debugMAIN) printf("Trying to open %s ", filename);
    fp = fopen(filename, "r");
    if(!fp && debugMAIN) printf("couldn't open file!\n");
    else if(debugMAIN) printf("opened file!\n");

    if(fp) {

        char *command = readToken(fp);
        while(!feof(fp)) {
            if(strncmp(command, "s", 1) == 0) {
                if(avl) displayAVL(((AVL *)tree), stdout);
                else displayGST(((GST *)tree), stdout);
            }
            else if(strncmp(command, "r", 1) == 0) {
                if(avl) statisticsAVL(((AVL *)tree), stdout);
                else statisticsGST(((GST *)tree), stdout);
            }
            else if (strncmp(command, "i", 1) == 0) {
                if (stringPending(fp)) {
                    command = readString(fp);
                } else {
                    command = readToken(fp);
                }
                if(avl) insertAVL(((AVL *)tree), newSTRING(command));
                else insertGST(((GST *)tree), newSTRING(command));
            }
            else if (strncmp(command, "d", 1) == 0) {
                if (stringPending(fp)) {
                    command = readString(fp);
                } else {
                    command = readToken(fp);
                }
                if(avl) deleteAVL(((AVL *)tree), newSTRING(command));
                else deleteGST(((GST *)tree), newSTRING(command));
            }
            else if(strncmp(command, "f", 1) == 0) {
                if (stringPending(fp)) {
                    command = readString(fp);
                } else {
                    command = readToken(fp);
                }
                int num = 0;
                if(avl) num = findAVLcount(((AVL *)tree), newSTRING(command));
                else num = findGSTcount(((GST *)tree), newSTRING(command));
                printf("%d\n", num);
            }

            if (stringPending(fp)) {
                command = readString(fp);
            } else {
                command = readToken(fp);
            }
        }
    }


    if(avl) freeAVL(((AVL *)tree));
    else freeGST((GST *) tree);
    //printf("\n");


    return 0;
}*/
