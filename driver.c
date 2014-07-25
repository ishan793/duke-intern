#include <stdio.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_rng.h>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include "results.h"

int compare_doubles (const double * a, const double * b);

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
    }
    
    int i = 0;

    double test_array[] = {0.1,0.05,0.85,0.07,0.23,0.0154};
    for (i = 0; i < 6; ++i)
    {
        printf("%lf\t",test_array[i]);
    }
    printf("\n");
    
    gsl_heapsort (test_array, 6, sizeof(double), compare_doubles);
    for (i = 0; i < 6; ++i)
    {
        printf("%lf\t",test_array[i]);
    }*/
    printf("\n");
    getValue(argv[1]);

    return 0;
}
