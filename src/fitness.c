/*******************************************************************************
 * Filename : fitness.c
 * Purpose  : A library of fitness functions that frame the current problem
 * 				into a representation (genotype) and operations to evaluate
 * 				the feasibility of each representation
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 16, 2013
 * Modified : May 8, 2014
 ******************************************************************************/

#include "individual.h"

#define BUFF_SIZE 500
#define NUM_GENES 10


void initProblem(char * filename){

}

// FIXME - explain that these are example implementations only
int getNumGenes(void){
	return NUM_GENES;
}

int getNumAlleles(int position){
	static int numAlleles[] = {4, 7, 2, 6, 9, 1, 2, 6, 3, 1};

	return numAlleles[position];
}

int evaluateFitness(int * gene){
	int sum, i;

	sum = 0;
	for(i=0; i<NUM_GENES; i++){
		sum = sum + gene[i];
	}

	return sum;
}
