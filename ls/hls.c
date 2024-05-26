#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hls.h"

int main(int argc, char *argv[]) {
    int op_l = 0, op_A = 0;
    const char *dir = ".";
    int i;
    int start = 1;

    /*Parse command line arguments*/
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            const char *p = argv[i] + 1;
            while (*p) {
                if (*p == 'l') op_l = 1;
                else if (*p == 'A') op_A = 1;
                else {
                    fprintf(stderr, "hls: invalid option -- '%c'\n", *p);
                    exit(EXIT_FAILURE);
                }
                p++;
            }
        } else {
            dir = argv[i];
        }
    }

    /* Handle multiple directories or files*/
    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (start) start = 0;
            else printf("\n");
            printf("%s:\n", argv[i]);
            list_directory(argv[i], op_l, op_A);
        }
    }

    if (start) {
        list_directory(dir, op_l, op_A);
    }

    return 0;
}