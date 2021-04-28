#include <stdio.h>
#include <stdlib.h>
#include "MWrapper.h"

float *_MATCREATE(int V, int Mask){
	int I = V;
	V = V * V;
	float * ptr = calloc(V, sizeof(float));
	if(Mask == 1){
		//setup identity matrix for chosen size matrix
		int Loop = 0;
		for(int i = 0; i < I; i++){
			ptr[Loop] = 1;
		       	Loop = Loop + I + 1;
		}
	}
	return ptr;
}


/*1, 0, 0, 0
 *0, 1, 0, 0
 *0, 0, 1, 0
 *0, 0, 0, 1
 *
*/
