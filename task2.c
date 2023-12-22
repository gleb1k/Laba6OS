#define ERROR 1
#define SUCCESS 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

double taylorSeries(int i, int n, int N) {
    double x = 2 * M_PI * i / N;
    double result = pow(-1, n-1) * pow(x,2*n+1) / tgamma(2*n+2);
    printf("i=%d, n=%d, PID=%d, calculated value=%f\n", i, n, getpid(), result);
    return result;
    //tgamma(n+1) it's factorial of n
}

int main(int arc, char** argv) {
    if (arc != 4) {
	fprintf(stderr, "Wrong parameter count\n");
	return ERROR;
    }

    int K = atoi(argv[1]);
    int N = atoi(argv[2]);
    int n = atoi(argv[3]);

    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Can't open file\n");
	return ERROR;
    }

    for (int i = 0; i < K; i++) {
    	double functionResult = 0;
    	int pipefd[2];
    	
    	if (pipe(pipefd) == -1) {
	    fprintf(stderr, "Can't create pipe\n");
	    return ERROR;
        } 
	
    	for (int j = 1; j <= n; j++) {
            pid_t pid = fork();
            
            if (pid == -1) {
                perror("Can'e exec fork()");
                return ERROR;
 	    } else if (pid == 0) { 
 	    	close(pipefd[0]);
 	    	// child process
 	    	double localResult = taylorSeries(i,j,N);
 	    	write(pipefd[1], &localResult, sizeof(localResult));
 	    	close(pipefd[1]);
                exit(EXIT_SUCCESS); //exit from child process
            } else if (pid > 0) {
            	// parent process
            	int status;
            	wait(&status);
            	close(pipefd[1]);
		double childResult;
		read(pipefd[0], &childResult, sizeof(childResult)); 
		close(pipefd[0]);
		functionResult += childResult; 	
	    }
        }
        
	printf("pid=%d,y[%d]=%f\n", getpid(), i, functionResult);
	fprintf(file, "y[%d]=%f\n", i, functionResult);
        
    }

    fclose(file);

    return 0;
}
