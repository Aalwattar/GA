/*******************************************************************************
 * Filename : fitness.c
 * Purpose  : A library of fitness functions that frame the current problem
 * 				into a representation (genotype) and operations to evaluate
 * 				the feasibility of each representation
 *
 *				This file must be completed by the user
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 16, 2013
 * Modified : May 17, 2014
 ******************************************************************************/

#include "fitness.h"
#include <stdio.h>

static int NUM_GENES = -1;
static int * NUM_ALLELES = NULL;


// Initialize the instance of this problem from an input file
void initProblem(char * filename){
	/**
	 * TODO - Initialize the following variable in here:
	 *	1) NUM_GENES	= The number of genes in a chromosome
	 *
	 *  2) NUM_ALLELES	= a mapping between the position of the
	 *			gene on the chromosome, and the number of possible
	 *			values it may take.
	 *
	 *			eg. NUM_ALLELES[3] = 7 means that the fourth gene
	 *				on the chromosome has 7 possible solutions
	 *
	 *			NOTE: All indices start at 0
	 */
}

// free any dynamically allocated memory (eg. as used in NUM_ALLELES)
void freeProblem(void){

}


int getNumGenes(void){
	return NUM_GENES;
}


int getNumAlleles(int position){
	return NUM_ALLELES[position];
}


int evaluateFitness(int * chromosome){
	// implement this method using the static variables
	// 		initialized above.
	return 0;
}
