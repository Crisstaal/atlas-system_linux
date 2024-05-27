#include <stdio.h>


/**
* print_error- print error
* return: error
*/
void print_error(const char *msg)
{
    fprintf(stderr,"hls: %s\n", msg);
}
