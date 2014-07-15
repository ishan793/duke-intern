#include <stdio.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_rng.h>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include "results.h"


int main(int argc, char *argv[]) {
    
    if (argc < 2){
        printf("Error in input\n");
        return -1;
    }
    /*
    int result = 0;
    int i = 1;
    double limit;


    while (i == 1){
    	limit = gsl_cdf_chisq_Qinv(0.05,15);
    	printf("%lf\n",limit);
    	scanf("%d",&i);
    }*/
    getValue(argv[1]);
    

    return 0;
}
