#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
float calcDeltaTime(clock_t lastCPUcycle, clock_t curCPUcycle){
	/*float dif = curCPUcycle - lastCPUcycle;
	dif = (float)(dif/CLOCKS_PER_SEC);*/
	float dif = (float)((float)(curCPUcycle / CLOCKS_PER_SEC) - (float)(lastCPUcycle / CLOCKS_PER_SEC));
	return dif;
}

/*1, 0, 0, 0
 *0, 1, 0, 0
 *0, 0, 1, 0
 *0, 0, 0, 1
 *
*/
