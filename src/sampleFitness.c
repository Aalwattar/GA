/*******************************************************************************
 * Filename : sampleFitness.c
 * Purpose  : A quick example of an implementation of fitness.c
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 16, 2013
 * Modified : May 17, 2014
 ******************************************************************************/

#include "fitness.h"

#include <stdio.h>
#include <stdlib.h>

static int NUM_GENES = -1;
static int * NUM_ALLELES = NULL;


// Initialize the instance of this problem from an input file
void initProblem(char * filename){
	/**
	 * The input file must be formatted as follows:
	 *	--------------------
	 *	num_genes
	 *	num_alleles[0]
	 *  num_alleles[1]
	 * 	.....
	 *	--------------------
	 *
	 *  eg.
	 *	--------------------
	 *	5
	 *	34
	 *	9
	 *	12
	 *	56
	 *	4
	 *	--------------------
	 */

	FILE * fp;
	char buffer[128];
	int i;

	fp = fopen(filename, "r");
	if(fp == NULL){
		fprintf(stderr, "Invalid problem file: %s", filename);
		exit(1);
	}

	// FIXME - change to fgets
	fscanf(fp, "%s", buffer);
	NUM_GENES = atoi(buffer);
	if(NUM_GENES < 1){
		fprintf(stderr, "Invalid number of genes: %s", buffer);
		exit(1);
	}

	NUM_ALLELES = (int *) malloc(sizeof(int) * NUM_GENES);

	for(i=0; i<NUM_GENES; i++){
		fscanf(fp, "%s", buffer);
		NUM_ALLELES[i] = atoi(buffer);

		if(NUM_ALLELES[i] < 1){
			fprintf(stderr, "Gene [%d] had an invalid number of alleles: %s", i+1, buffer);
			exit(1);
		}
	}

	fclose(fp);
}

void freeProblem(void){
	free(NUM_ALLELES);
}


int getNumGenes(void){
	return NUM_GENES;
}


int getNumAlleles(int position){
	return NUM_ALLELES[position];
}


int evaluateFitness(int * chromosome){
	int sum, i;

	sum = 0;
	for(i=0; i<NUM_GENES; i++){
		sum = sum + chromosome[i];
	}
	return sum;
}
