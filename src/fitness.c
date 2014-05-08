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


// FIXME - explain that these are example implementations only
int getNumGenes(void){
	return 1;
}

int getNumAlleles(int position){
	if(position == 0)
		return 1;

	return 0;
}
