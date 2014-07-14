/* This file contains functions which help in producing sample points.
*/
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_rng.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "sampling.h"

void getSamples(double *array, int num_samples,int a,double b,int sel,gsl_rng *r,const gsl_rng_type * T){
	// a - successful trials for beta/ size for erlang
	// b - failed trials for beta/ sum for erlang
	clock_t now;
	
	double step_size;	
	int i=0;
	double rando = 0;

	r = gsl_rng_alloc (T);
	gsl_rng_set(r,(unsigned int) clock());	  

	// generate points in equally spaced probab axis
	step_size = 1.0/num_samples;
	
	for(i=0;i<num_samples;i++){
		
		rando = gsl_rng_uniform(r);
		rando = step_size*(i+rando);
		
		if(sel == 0){
			//printf("in bet\n");
			// set distribution parameters 
			array[i] = gsl_cdf_beta_Pinv(rando,a,(int)b);
		}
		else if(sel == 1){
			// set distribution parameters
			//printf("random number is %lf, sum is %lf and size is %d\n",rando,b,size);
			array[i] = erlang_inv(rando,a,b);
		}
		//printf("in sampling %lf\n",array[i]);
	}
	
	shuffle(array,num_samples);	
}


double erlang_inv(double p,int size,double sum){
	//gsl_cdf_gamma_P (double x, double a, double b)
	//b = 1/lambda,a = size,x = sum
	double _guess = size/sum;
	double _lower = 0.0;
	double _upper = 10*_guess;
	double _pguess = 0.0;

	double b = 1/_guess;
	int a = size;
	//double x = sum;
	double current = 0.0;
	double differene = 5.0;

	//printf("guess is : %lf",_guess);
	while(differene > 0.00001){
		differene = (_guess - _pguess);
		if (differene < 0)
		{
			differene = differene*(-1);
		}
		b = 1.0/_guess;
		current = gsl_cdf_gamma_P (sum,size,b);	
		_pguess = _guess;
		//printf("previous guess : %lf, current guess : %lf\n",_pguess,_guess);
		if(current > p){
			_upper = _guess;
			_guess = (_lower + _upper)/2;
		}
		else if(current < p){
			_lower = _guess;
			_guess = (_upper + _lower)/2;
		}
		else{
			return _guess;
		}
	}
	return _guess;
}

void shuffle(double *array, int n){
	int i=0;
	int j;
	double temp;
	srand(time( NULL));
	for(i=0;i<n;i++){
		j = rand()%(i+1);
		temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}
	
