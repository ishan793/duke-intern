/*
This file contains the source code to find the number of observations required to 
obtain the stated conf interval.
*/
#include <gsl/gsl_cdf.h>
#include <stdlib.h>
#include "size.h"


int fixedPoint(int initial_estimate, double *argv,double (*func)(double,double,double,double)){
	// func is the function using which iteration is performed
	// note : func should be of the form :func(x,....) to allow x = func(x,....)
	//argsv[0] - lambda_hat ;argsv[1] - half_width/lambda_lower/lambda_upper;argsv[2] - alfa	
	
	int x_n_1 = initial_estimate;
	int x_n = (int)func(x_n_1,argv[0],argv[1],argv[2]);
	float diff = 1;
	// in case rogue values of alfa are given, return -1
	if(argv[2] >1 || argv[2] <0)	return -1;
	
	// find 'r' value by iterative mehtod
	while(diff > ERROR){
		//printf("x(n) : %d and x(n-1) : %d\n",x_n,x_n_1);
		x_n_1 = x_n;
		x_n = (int)func((double)x_n_1,argv[0],argv[1],argv[2]);
		diff = x_n - x_n_1;
		if(diff <0.0) diff = -1*diff;

	}
	
	return x_n;
	
}

double erlang_2sided(double r, double lambda_hat, double half_width, double alfa){
	if(alfa>1 || alfa<0)	return -1;
	//printf("in erlang_2sided\n");
	double res = gsl_cdf_chisq_Qinv (alfa/2,2*r) - gsl_cdf_chisq_Qinv ((1 - (alfa/2)),2*r);
	
	res = lambda_hat*res/(4*half_width);
	//printf("exiting erlang_2sided\n");
	return res;
}

double beta_1sided(double r,double c_hat,double c_l,double alfa){
	double arg1,den,res;

	arg1 = r*(1 - c_hat) + 1;
	arg1 = 2*arg1;
	den = 2*(1 - c_l);

	res = gsl_cdf_chisq_Qinv(alfa,arg1)/den;
	return res;
}