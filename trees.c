
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


/* options */
static int debugMAIN = 0;
static int avl = 1;
char *filename;


void Fatal(char *fmt, ...) {
    va_list ap;

    fprintf(stderr, "An error occured: ");
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
processOptions(int argc, char **argv) {
    int argIndex;
    int argUsed;
    int separateArg;
    argIndex = 1;


    while (argIndex < argc && *argv[argIndex] == '-') {
        if (debugMAIN) printf("Checking Options\n");


/* check if stdin, represented by "-" is an argument */


/* if so, the end of options has been reached */

        if (argv[argIndex][1] == '\0') return argIndex;

        separateArg = 0;
        argUsed = 0;

        if (argv[argIndex][2] == '\0') {
            //filename = argv[argIndex+1];
            separateArg = 1;
        }
        //filename = argv[argIndex]+2;

        switch (argv[argIndex][1]) {

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
             */


            break;
            case 'g':
                avl = 0;
                argUsed = 1;
                break;
            case 'r':
                avl = 1;
                argUsed = 1;
                break;
            case 'v':
                printName();
                break;
            default:
                Fatal("option %s not understood\n", argv[argIndex]);
        }

        if (separateArg && argUsed) {
            if (debugMAIN) printf("separate arg and argUsed\n");
            //++argIndex;
        }

        ++argIndex;
    }
    if (debugMAIN) printf("%d\n", argIndex);

    return argIndex;
}


int
main(int argc, char **argv) {

    if (debugMAIN)printf("First Arg: %s\nSecond Arg: %s\n", argv[1], argv[2]);
    FILE *fp;
    int argIndex;
    void *tree;

    if (argc == 1) Fatal("%d arguments!\n", argc - 1);

    argIndex = processOptions(argc, argv);


    if (argIndex == argc)
        printf("No arguments\n");
    else {
        filename = argv[argIndex];
        if (debugMAIN) {
            printf("Remaining arguments:\n");
            for (int i = argIndex; i < argc; ++i)
                printf("    %s\n", argv[i]);
        }
    }

    if (debugMAIN) printf("Trying to open %s ", filename);
    fp = fopen(filename, "r");
    if (!fp && debugMAIN) printf("couldn't open file!\n");
    else if (debugMAIN) printf("opened file!\n");

    if (avl) {
        AVL *a = newAVL(displaySTRING, compareSTRING, freeSTRING);
        tree = a;

        if (debugMAIN) printf("inserting strings\n");
        char *temp;
        if (stringPending(fp)) {
            temp = readString(fp);
        } else {
            temp = readToken(fp);
        }


        while (!feof(fp)) {
            if (debugMAIN) printf("adding : %s\n", temp);
            if (temp[0] != '\0') insertAVL(a, newSTRING(temp));
            if (stringPending(fp)) {
                temp = readString(fp);
            } else {
                temp = readToken(fp);
            }
        }

//        if(debugMAIN) printf("adding : %s\n", temp);
//        insertHEAP(h, newSTRING(temp));
        if (debugMAIN) printf("items loaded. displaying avl...\n");

        if (debugMAIN)displayAVL(a, stdout);

        //freeAVL(a);

    } else {
        GST *g = newGST(displaySTRING, compareSTRING, freeSTRING);
        tree = g;

        if (debugMAIN) printf("inserting strings\n");
        char *temp;
        if (stringPending(fp)) {
            temp = readString(fp);
        } else {
            temp = readToken(fp);
        }

        while (!feof(fp)) {
            if (debugMAIN) printf("adding : %s\n", temp);
            if (temp[0] != '\0') insertGST(g, newSTRING(temp));
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


/*    Done loading in words from file    */




/*    loading in commands    */

    filename = argv[argIndex + 1];
    if (debugMAIN) printf("Trying to open %s ", filename);
    fp = fopen(filename, "r");
    if (!fp && debugMAIN) printf("couldn't open file!\n");
    else if (debugMAIN) printf("opened file!\n");

    if (fp) {

        char *command = readToken(fp);
        while (!feof(fp)) {
            if (strncmp(command, "s", 1) == 0) {
                if (debugMAIN) printf("_trees : MAIN -  command:s displaying tree.\n");
                if (avl) displayAVL(((AVL *) tree), stdout);
                else displayGST(((GST *) tree), stdout);
            } else if (strncmp(command, "r", 1) == 0) {
                if (debugMAIN) printf("_trees : MAIN -  command:r displaying stats.\n");
                if (avl) statisticsAVL(((AVL *) tree), stdout);
                else statisticsGST(((GST *) tree), stdout);
            } else if (strncmp(command, "i", 1) == 0) {
                if (stringPending(fp)) {
                    command = readString(fp);
                } else {
                    command = readToken(fp);
                }
                if (command[0] != '\0') {
                    if (debugMAIN) printf("_trees : MAIN -  command:i inserting[%s].\n", command);
                    if (avl) insertAVL(((AVL *) tree), newSTRING(command));
                    else insertGST(((GST *) tree), newSTRING(command));
                }
            } else if (strncmp(command, "d", 1) == 0) {
                if (stringPending(fp)) {
                    command = readString(fp);
                } else {
                    command = readToken(fp);
                }
                if (command[0] != '\0') {
                    if (debugMAIN) printf("_trees : MAIN -  command:d deleting[%s].\n", command);
                    void *str = newSTRING(command);
                    void *temp = 0;
                    if (avl) {
                        temp = deleteAVL(((AVL *) tree), str);
                        if(!temp) {
                            printf("Value ");
                            displaySTRING(str, stdout);
                            printf(" not found.\n");
                        }
                    }
                    else {
                        temp = deleteGST(((GST *) tree), str);
                    }

                }
            } else if (strncmp(command, "f", 1) == 0) {
                if (stringPending(fp)) {
                    command = readString(fp);
                } else {
                    command = readToken(fp);
                }
                if (command[0] != '\0') {
                    if (debugMAIN) printf("_trees : MAIN -  command:f finding count of [%s].\n", command);
                    int num = 0;
                    if (avl) num = findAVLcount(((AVL *) tree), newSTRING(command));
                    else num = findGSTcount(((GST *) tree), newSTRING(command));
                    printf("Frequency of %s: %d\n",command, num);
                }
            }

            if (stringPending(fp)) {
                command = readString(fp);
            } else {
                command = readToken(fp);
            }
        }
    }


    if (avl) freeAVL(((AVL *) tree));
    else freeGST((GST *) tree);
    //printf("\n");


    return 0;
}
