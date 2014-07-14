#ifndef results_h
#define results_h

typedef struct{
	char *name;
	double value;
	double conf_level;
	double sec_value;
	int type;
	} var_info;

var_info variables[100]; 

double getFile(char const *argv);
char *storeFile(char const *argv);
int makeString(char *input_array, double *mod_value,double time_value);
int getVar(char *input_array);
int command (char *_command);
double relExpr(double *rate, double t);
double getResults(double *val, int num_var, double *param_hat, double *var, double *alfa, double *type, double t);

#endif
