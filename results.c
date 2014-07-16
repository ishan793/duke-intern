#include <gsl/gsl_rng.h>
#include "sampling.h"
#include "results.h"
#include "size.h"
#include <string.h>
#include <stdio.h>


int total = 0;								// indicates the number of var
int uncern = 0;								// marker to tell whether uncertainty needs to be done or not
char type[10][20] = {'\0'};                     // word that indicates the variable whose value is found by sharpe.
int uncer_type[10] = {0};                         // variable to denote the uncertainty type
char *test;                                 // string to store the file as an array
int uncer_var_count = 0;                // variable to keep track of number of variables of uncer type
int expr_count;
double results[500][10];


double getValue(char const *argv){

    char sharpe_command[50] = {'\0'};       // string to form the sharpe command in case uncertainty is not reqd.
    double alfa[10];                        // array to store the confidence level of varaible(s)
    double params[10];                      // array to store the point estimate of varaible(s)
    double width[10];                       // array to store half width/lower limit of varaible(s)
    double _type[10];                       // array to store the type of varaible(s)
    double result_column[500];              // array to store the resultant values of the system
    int num_variables = 0;                  // varaible for number of variables
    int sample_size = 0;                    // variable for number of total sampels
    int i = 0;                              // loop counter
    double t = 0;                           // time value


    test = storeFile(argv);
    num_variables = getVar(test);
    
    for(i = 0; i<uncer_var_count;i++){

        alfa[i] = variables_uncer[i].conf_level;
        params[i] = variables_uncer[i].value;
        width[i] = variables_uncer[i].sec_value;
        _type[i] = variables_uncer[i].type;    
        
    }
    
    if(uncern == 0){
        strcat(sharpe_command,"./sharpe ");
        strcat(sharpe_command,argv);
        command(sharpe_command);
        return ;
    }
    
    sample_size = getResults(uncer_var_count,params,width,alfa,_type,t);
    
    makeString(test,params,12);
    command("./sharpe output.txt");

    int j = 0;
    for(i = 0; i < expr_count ; i++){
        switch (uncer_type[i]){
            case(1):
                // evaluate expected
                for(j = 0;j<sample_size;j++){
                    // j indicates the sample size counter and 'i' is the expr_count counter
                    result_column[j] = results[j][i];
                }
                printf("Expected %s %lf\n",type[i],average(result_column,sample_size));
                break;
            case(2):
                for(j = 0;j<sample_size;j++){
                    // j indicates the sample size counter and 'i' is the expr_count counter
                    result_column[j] = results[j][i];
                }
                printf("Variance of %s %lf\n",type[i],variance(result_column,sample_size));
                break;
            default :
                printf("Feature not yet implemented\n");

        }
        // re intialize the matrix
        for(j = 0;j<sample_size;j++){
            // j indicates the sample size counter and 'i' is the expr_count counter
            result_column[j] = 0;
        }
    }
    printf("Number of samples considered %d\n",sample_size);

    /*FILE *fout;
    fout = fopen("my_text.txt","w");
    for(i=0;i<sample_size;i++){
        fprintf(fout,"value %lf %lf %lf\n",result[i]);
    }
    fclose(fout);
    */
    free(test);
}

// returns the number of samples taken, modifies the val array to store the values.
double getResults(int num_var, double *param_hat, double *var, double *alfa, double *_type, double t){
	// _size - refers to the number of samples that need to be generated.
	// num_var - number of variables present.
	// param_hat - expected value of parameter(array).
	// alfa - confidence value(array).				
	// var - half width for erlang, c_lower for beta(array).
	// _type - variable to tell whether values at ith index are of beta(0) or erlang(1)
	
	int _size = 0;
	int _expr_count = 0;
	double sample_points[num_var][max_size]; 	// array to store the samples generated
	int i,j; 					// for loop param
	int total[num_var]; 		// array to store the number of observation for each variable
	double args[3];				// array to pass to fixePoint
	int a;						// successfull trials for beta/ size for erlang
	double b;					// failed trials for beta/ sum for erlang
	double func_arg[num_var];	// array to store the variables which are used to evaluate the required expression.
	
	// random number generator types
	const gsl_rng_type * T;
  	gsl_rng * r;
	T = gsl_rng_default;
	
	// calculate number of observation for each variable
	for (i = 0; i < num_var; ++i){
		
		args[0] = param_hat[i];
		args[1] = var[i];
		args[2] = alfa[i];

		if(_type[i] == 0) total[i] = fixedPoint(100,&args[0],&beta_1sided);
		else if(_type[i] == 1) total[i] = fixedPoint(100,&args[0],&erlang_2sided);

		if(total[i] > _size) _size = total[i];

	}
	
	//printf("%d\n",_size);
	
	// generating sample points

	for(i = 0; i < num_var; i++){
		// if ith distribution is of beta type
		if(_type[i] == 0){
			a = param_hat[i]*total[i];
			b = total[i] - a;	
		} 
		// if ith type distribution is of erlang type
		else if(_type[i] == 1){
			a = total[i];
			b = a/param_hat[i];
		}
		//printf("%dth distribution\n",i+1);
		getSamples(&sample_points[i][0],_size,a,b,_type[i],r,T);
		//gsl_rng_free (r);	
	}
	/*printf("samples generated\n");
	for (i = 0; i < num_var; i++)
	{
		printf("i : %d\n",i);
		for(j = 0 ; j < _size ; j++){
			
			printf("1st distribution %lf\n",sample_points[i][j]);
		}
	}*/
	for (i = 0; i < _size; i++)
	{
		for(j = 0 ; j < num_var ; j++){
			func_arg[j] = sample_points[j][i];
		}
		relExpr(func_arg,t,results[i]);
	}
    
	return _size;
}

int relExpr(double *rate, double t, double *res){
   
    int i;                          // for loop counter
    char my_string[50] = {"\0"};    // string for storing words and checking them
    int _count = 0;                 // index for my_string array
    int on = 0;                     // flag to tell whether required word encountered or not
    int wcount = 0;                 // count to keep track of words after encountering required word
    int var_count = 0;              // count to keep track of the number of variables
    int my_count = 0;
    
	makeString(test,rate,t);        // function called to make output text file which sharpe acts upon   
	
    // direct the sharpe outputs to a text file
    freopen("sharpe_output.txt","w",stdout);
	command("./sharpe output.txt");
    freopen("/dev/tty","a",stdout);
    // read the text file and extract the value.
    FILE *fin;
    fin = fopen("sharpe_output.txt","r");
    if(fin == NULL){ 
        printf("Error opening files from relExpr function\n");
        return 0;
    }
    char c = fgetc(fin);
    while(c != EOF){
        
        // read the given input file
        if (c != ' ' && c != '\t' && c != '\n') {
            if (c > 33 && c < 127){ 
                my_string[_count] = c; 
                _count ++;   
            }
        }
        // when a word is completed, check for appropriate markers.
        else if (c == ' ' || c == '\t' || c == '\n'){   
            // if word is encountered, switch on, switch off upon new line
            if(strcmp(type[my_count],my_string) == 0 ){
                if(strcmp(type[my_count],"") != 0){
                    on = 1;
                    //printf("String = %s, count = %d\n",my_string,my_count);
                }
            }
            if(on == 1){
                wcount++;
                switch(wcount){
                    case 4:
                        //printf("value is %s\n",my_string);
                        /*changes made here, rectify*/
                        //printf("count = %d, value = %s\n",my_count,my_string);
                        res[my_count] = atof(my_string);
                        my_count++;
                        //return atof(my_string);
                        
                        break;
                    default:
                        //printf("wcount = %d and word is %s\n",wcount,my_string);
                        break;
                }
                if(c == '\n'){
                    wcount = 0;
                    on = 0;
                }
            }
            // initialize the string back, to make it ready to accomodate next word
            for(i = 0;i < _count+1; i++){
                my_string[i] = '\0';
            }
            _count = 0;          
        }
        else { printf("there is something else\n");}
    c = fgetc(fin);
    }
    
    return my_count;
}
// function to read the input file and store it an array
char *storeFile(char const *argv){
    FILE *fin;
    int i = 0;
    char *input_array;
    fin = fopen(argv, "r");
    if(fin == NULL){
        printf("Error reading file\n");
        EXIT_FAILURE;
    }
 
    fseek(fin, 0L, SEEK_END);
    size_t fileSize = ftell(fin);
    fseek(fin, 0L, SEEK_SET);
    
    input_array = (char *)malloc(fileSize*sizeof(char));
    char c = fgetc(fin);

    while( c != EOF){
        input_array[i] = c;
        i++;
        c = fgetc(fin);
    }

    input_array[i] = c;
    fclose(fin);
    return input_array;
}

// function to make a sharpe input file with the given variable values
int makeString(char *input_array, double *mod_value,double time_value)
{   
    int char_count = 0;             // index of the input array
    int i;                          // for loop counter
    char my_string[50] = {"\0"};    // string for storing words and checking them
    int _count = 0;                 // index for my_string array
    int on = 0;                     // flag to tell whether bind/end encountered or not
    int _flag = 0;                  // flag to ensure prints for bind statements are done only once

    FILE *fout;
    fout = fopen("output.txt","w");
    if( fout == NULL ){
        printf("Error with files\n");
    }
    
    char c = input_array[char_count];
    
    while(c != EOF){
        
        c = input_array[char_count++];
        // read the given input file
        if (c != ' ' && c != '\t' && c != '\n') {
            if (c > 33 && c < 127){
                my_string[_count] = c; 
                _count ++;
                //printf("_count = %d\n",_count );   
            }
        }
        // when a word is completed, check for appropriate markers.
        else if (c == ' ' || c == '\t' || c == '\n'){   
            // if bind is encountered, switch on, if end then switch off
            //printf("%s\n",my_string);
            if(strcmp(my_string,"bind") == 0)       on = 1; 
            else if(strcmp(my_string,"end") == 0)   on = 0;
            
            if(on == 0) {
                if(strcmp(my_string,"expected") == 0){
                    //fprintf(fout,"bind\nt %lf\nend\n",time_value);
                }
                else if(strcmp(my_string,"variance") == 0){
                    //fprintf(fout,"bind\nt %lf\nend\n",time_value);    
                }
                else if(strcmp(my_string,"confidence") == 0){
                    //fprintf(fout,"bind\nt %lf\nend\n",time_value);    
                }
                else fprintf(fout, "%s%c", my_string,c);
            }
            else if(on == 1) {
                if(_flag == 0){
                    /* Enter print statements*/
                    fprintf(fout, "bind\n");
                    for( i = 0; i < uncer_var_count ; i++) {
                        fprintf(fout,"%s %lf\n",variables_uncer[i].name,mod_value[i]);
                    }
                    for ( i=0; i < total - uncer_var_count ; i++){
                        fprintf(fout,"%s %lf\n",variables_normal[i].name,variables_normal[i].value);    
                    }
                    _flag = 1;
                }
                
            }
            for(i = 0;i < _count+1; i++){
                my_string[i] = '\0';
            }
            _count = 0;          
        }
        else { printf("there is something else\n");}
        
        
        
        //printf("%c",c); 
    }
    if(c == EOF)    fprintf(fout,"%s",my_string);
    //fprintf(fout,"%c",c);
    fflush(fout);
    fclose(fout);
    

    return 0;

}

// function to extract the variable values from the input file
int getVar(char *input_array)
{   
    int char_count = 0;             // index of the input array
    int i;                          // for loop counter
    char my_string[50] = {"\0"};    // string for storing words and checking them
    int _count = 0;                 // index for my_string array
    int on = 0;                     // flag to tell whether bind/end encountered or not
    int _flag = 0;                  // flag to ensure prints for bind statements are done only once
    int wcount = 0;                 // count to keep track of words after encountering bind
    int var_count = 0;              // count to keep track of the number of variables
    int uncer_flag = 0;             // variable to denote type of analysis
    int is_uncer = 0;               // variable to check the type of current sharpe var

    char c = input_array[char_count];
    while(c != EOF){
        c = input_array[char_count++];
        // read the given input file
        if (c != ' ' && c != '\t' && c != '\n') {
            if (c > 33 && c < 127){
                
                my_string[_count] = c; 
                _count ++;   
            }
        }
        // when a word is completed, check for appropriate markers.
        else if (c == ' ' || c == '\t' || c == '\n'){   
            // if bind is encountered, switch on, if end then switch off
            if(strcmp(my_string,"bind") == 0)       on = 1; 
            else if(strcmp(my_string,"end") == 0)   on = 0;
            // look for markers related to uncertainty analysis
            else if(strcmp(my_string,"expected") == 0) {
                uncer_type[expr_count] = 1;
                uncer_flag = 2;
            }
            else if(strcmp(my_string,"variance") == 0) {
                uncer_type[expr_count] = 2;
                uncer_flag = 2;
            }
            else if(strcmp(my_string,"confidence") == 0) {
                uncer_type[expr_count] = 3;
                uncer_flag = 2;
            }

            if(uncer_flag == 2) {
                wcount ++;
                switch(wcount){
                    case 3 :
                        //printf("3: %s\n",my_string);
                        strcpy(type[expr_count],my_string);
                        strcat(type[expr_count++],":");
                        break;
                    default :
                        //printf("%s\t%d\n",my_string,wcount);
                        break;
                }
                if(c == '\n'){
                    wcount = 0;
                    uncer_flag = 0;
                }
            }


            if(on == 1) {
                //printf("%s and %d\n",my_string,wcount);   
                wcount ++; 
                
                switch(wcount){
                    case 1 : 
                        if(strcmp(my_string,"conf") == 0){
                            uncern = 1;
                            is_uncer = 1;
                            uncer_var_count++;
                            var_count++;
                        }
                        else {
                            is_uncer = 0;
                            if (strcmp(my_string,"bind") != 0) {
                                if (strcmp(my_string,"") != 0) {
                                    //printf("name = %s and count = %d\n",my_string,(var_count - uncer_var_count));
                                    var_count++;
                                    variables_normal[var_count - uncer_var_count - 1].name = strdup(my_string);        
                                } 
                            }
                        }
                        break;
                    case 2 :
                        if(is_uncer == 1){
                            if(strcmp(my_string,"bind") != 0) {
                                variables_uncer[uncer_var_count-1].name = strdup(my_string);
                            }
                        }
                        else {
                            
                            variables_normal[var_count - uncer_var_count - 1].value = atof(my_string);    
                        }
                        break ;
                    case 3 :
                        if(is_uncer == 1){
                            variables_uncer[uncer_var_count-1].value = atof(my_string);
                        }
                        break;
                    case 4:
                        if(uncern == 1){
                            variables_uncer[uncer_var_count-1].conf_level = atof(my_string);
                        }
                        break;
                    case 5:
                        if(uncern == 1){
                            variables_uncer[uncer_var_count-1].sec_value = atof(my_string);
                        }
                        break;
                    case 6:
                        if(uncern == 1){
                            if(strcmp(my_string,"erlang") == 0){ 
                                variables_uncer[uncer_var_count-1].type = 1;
                            }
                            else if(strcmp(my_string,"beta") == 0){
                                variables_uncer[uncer_var_count-1].type = 0;    
                            }
                        }
                        break;
                    default :
                        printf("word : %s and wcount : %d\n",my_string,wcount);
                        break;
                }
                if(c == '\n'){
                    wcount = 0;
                }
            }
            // initialize the string back, to make it ready to accomodate next word
            for(i = 0;i < _count+1; i++){
                my_string[i] = '\0';
            }
            _count = 0;          
        }
        else { printf("there is something else\n");}
    }
    
    if(c == EOF)    //printf("%s",my_string);
    total = var_count;
    return total;

}

int command (char *_command)
{
    int i;
    char command[20];
   
    if (system(NULL)) ;
    else exit (EXIT_FAILURE);
    /*    Make the argument of system a variable to be passed by the driver func  */
    system (_command);
    return 0;
}

