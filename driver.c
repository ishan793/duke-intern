#include <stdio.h>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include "results.h"


int main(int argc, char *argv[]) {
    
    if (argc < 2){
        printf("Error in input\n");
        return -1;
    }
    
    getValue(argv[1]);
    
    return 0;
}
