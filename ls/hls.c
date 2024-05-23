#include<dirent.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, const char *argv[]) {
    if (argc == 1) {
      _ls(".", 0,0);
    }
    else if(argc == 2) {
        if (argv[1][0] == '_') {
            int op_a = 0;
            int op_l = 0;
            else if(*p == 'l') op_l =1;
            else {
                 perror("Unable to read directory");
                 exit(EXIT_FAILURE);
            }
            p++;
        }
    _ls('.', op_a, op_l);
    } else {/**does the directory exist*/
_ls(argv[1], 0, 0);
    } else if (argc ==3) {
        if(argv[1][0] != '-') {
            perror("Unable to read directory");
            exit(EXIT_FAILURE);
        }
         int op_a = 0, op_l = 0;
         char *p = (char *)(argv[1]+[1]);
         while (*p) {
            if (*p == 'a') op_a = 1;
            else if (*P == 'l') op_l =1;
            else {
              perror("Unable to read directory");
              exit(EXIT_FAILURE);
            }
            p++;
        }
        _ls (argv[2], op_a, op_l);
         }
    return (0);
}