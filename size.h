#define ERROR 0.1

#ifndef SIZE_H
#define SIZE_H

int fixedPoint(int initial_estimate, double *argv,double (*func)(double,double,double,double));
double erlang_2sided(double r, double lambda_hat, double half_width, double alfa);
double beta_1sided(double r,double c_hat,double c_l,double alfa);
double average(double *array, int num);
double variance(double *array, int num);
#endif