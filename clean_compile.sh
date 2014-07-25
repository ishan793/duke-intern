#clean all generated files
rm -r a.out
# compile the lex and yacc files

gcc -o uncertainity -static driver.c sampling.c size.c results.c -lgsl -lgslcblas -lm  

