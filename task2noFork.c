#define ERROR 1
#define SUCCESS 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

double taylorSeries(int i, int n, int N) {
    double x = 2 * M_PI * i / N;
    double result = pow(-1, n-1) * pow(x,2*n-1) / tgamma(2*n);
    printf("i=%d, n=%d, PID=%d, calculated value=%f\n", i, n, getpid(), result);
    return result;
    //tgamma(n) it's factorial of n-1
}

int main(int arc, char** argv) {
    if (arc != 4) {
	fprintf(stderr, "Wrong parameter count\n");
	return ERROR;
    }

    int K = atoi(argv[1]);
    int N = atoi(argv[2]);
    int n = atoi(argv[3]);

    FILE *file = fopen("outputNoFork.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Can't open file\n");
	return ERROR;
    }

    for (int i = 0; i < K; i++) {
    	double functionResult = 0;

    	for (int j = 1; j <= n; j++) {
	    functionResult += taylorSeries(i,j,N);
	    printf("pid=%d,y[%d]=%f\n", getpid(), i, functionResult);
	}
        
	printf("pid=%d,y[%d]=%f\n", getpid(), i, functionResult);
	fprintf(file, "y[%d]=%f\n", i, functionResult);
        
    }
    
    printf("pid=%d, for Pi", getpid());
    
    double piResult = 0;
    for (int j = 1; j <= 100; j++) {
	piResult += taylorSeries(M_PI,j,N);
    }
    printf("sin(pi)=%f\n", piResult);

    fclose(file);

    return 0;
}
