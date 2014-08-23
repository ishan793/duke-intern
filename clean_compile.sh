#clean all generated files
rm -f uncertainty
# compile the lex and yacc files
gcc -o uncertainty driver.c sampling.c size.c results.c -lgsl -lgslcblas -lm  

