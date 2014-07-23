#ifndef results_h
#define results_h

typedef struct{
	char *name;
	double value;
	double conf_level;
	double sec_value;
	int type;
	} var_info;

typedef struct{
	char *name;
	double value;
	} var_info_normal;

var_info variables_uncer[50]; 
var_info_normal variables_normal[50]; 

double getFile(char const *argv);
char *storeFile(char const *argv);
int makeString(char *input_array, double *mod_value,double time_value);
int getVar(char *input_array);
int command (char *_command);
int relExpr(double *rate, double t, double *res);
double getResults(int num_var, double *param_hat, double *var, double *alfa, double *type, double t);
int compare_doubles (const double * a, const double * b);

#endif
