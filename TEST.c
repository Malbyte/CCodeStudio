#include <stdio.h>
#include <stdlib.h>
//try to create a matrix and return it
int *Matrix(int V){
	int * ptr = calloc(V, sizeof(int));
	return ptr;
}
int main(int argc, char *argv[] ){
	int IN;
	if(argc > 1){
		IN = atoi(argv[1]);
		printf("%d\n\n", IN);
	}
	else{
		IN = 2;
	}
	int *ptr = Matrix(IN);
	for (int i = 1; i <= 100; i++){
		printf("%d  ", ptr[i-1]);
		if(i % 5 == 0 && i != 0){
			printf("\n");
		}
	}
	printf("\n");
}



