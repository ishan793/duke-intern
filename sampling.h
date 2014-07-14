// header file for functions required for generating sample points
#define max_size 1000

#ifndef SAMPLES_H
#define SAMPLES_H

void getSamples(double *array, int size,int a,double b,int sel,gsl_rng *r,const gsl_rng_type * T);
double erlang_inv(double p, int size, double sum);
void shuffle(double *array, int n);


#endif