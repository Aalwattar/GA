/*******************************************************************************
 * Filename : Individual.c
 * Purpose  : All methods for the creation and manipulation of a possible
 *              solution to the problem (an individual in the population)
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 7, 2013
 * Modified : May 8, 2014
 ******************************************************************************/

#include "individual.h"

#include <stdlib.h>
#include <stdio.h>

// FIXME - These should be passed in from main, not hard coded values
// they should NOT have an initial value
static double MUTATION_RATE  = 0.005;
// TODO - is this variable unncessary? This information will be hard-coded into the problem statement file (fitness.c)
static int    NUM_GENES = 0;

static int (*evaluateFitness)(int * genotype);
static int (*getNumAlleles)(int);




struct individual{
	int * gene;
	int fitness;
};


void initIndividualClass(double mutRate, int numGenes, int (*numAllelesFunc)(int), int (*fitnessFunc)(int *)){
	if(mutRate < 0 || mutRate > 1){
		fprintf(stderr, "Invalid mutation rate: %.3lf.\n", mutRate);
		fprintf(stderr, "The mutation rate must be a decimal number between 0 and 1\n");
		exit(1);
	}

	if(numGenes <= 0){
		fprintf(stderr, "Invalid number of genes: %d\n", numGenes);
		fprintf(stderr, "The number of genes must be greater than zero.\n");
		exit(1);
	}

	MUTATION_RATE = mutRate;
	NUM_GENES = numGenes;
	getNumAlleles = numAllelesFunc;
	evaluateFitness = fitnessFunc;
}


// FIXME - add a propper comment
//  Private method that allocates memory for an Individual.
// 	it does NOT set default values to its members

// FIXME - find an invalid fitness value to set in newly generated individuals
Individual newIndividual(){
	Individual ind;

	ind = (Individual) malloc(sizeof(struct individual));
	ind->gene = malloc(sizeof(int) * NUM_GENES);

	return ind;
}

Individual newRandIndividual(){
	Individual ind;
    int g;
    
    ind = newIndividual();
    for(g=0; g<NUM_GENES; g++)
        ind->gene[g] = (getNumAlleles(g)) * randomNumber();
    // ind->fitness = 0;

    return ind;
}

Individual cloneIndividual(Individual original){
	Individual copy;
    int g;
    
    copy = newIndividual();
    for(g=0; g<NUM_GENES; g++)
        copy->gene[g] = original->gene[g];
    	copy->fitness = original->fitness;

    return copy;
}

void freeIndividual(Individual i){
    free(i->gene);
    free(i);
}

int getFitness(Individual ind){
	return ind->fitness;
}

// the compare function for qsort
int compareIndividuals(const void * p1, const void * p2){
    return (*(Individual *)p2)->fitness - (*(Individual *)p1)->fitness;
}

// the compare function for qsort
int compareIndividualsReversed(const void * p1, const void * p2){
    return (*(Individual *)p1)->fitness - (*(Individual *)p2)->fitness;
}

void mutate(Individual ind){
    int g;
    
    for(g=0; g<NUM_GENES; g++)
        if(randomNumber() < MUTATION_RATE)
            ind->gene[g] = (getNumAlleles(g)) * randomNumber();
}                              


void crossover(Individual ind1, Individual ind2){
    int gene1, gene2;
    int temp;
    int g;
    
    // choose two different "cutting points" FIXME - the English here is atrocious ....
    gene1 = NUM_GENES * randomNumber();
    gene2 = NUM_GENES * randomNumber();
    
    while(gene1 == gene2)
        gene2 = NUM_GENES * randomNumber();

    // ensure that gene1 appears before gene2
    if(gene1 > gene2){
        temp = gene1; 
        gene1 = gene2;
        gene2 = temp;
    }

    // perform crossover for all genes between our two cutting points (inclusive)
    for(g=gene1; g <= gene2; g++){
        temp = ind1->gene[g];
        ind1->gene[g] = ind2->gene[g];
        ind2->gene[g] = temp;
    }
}

void printIndividual(Individual ind){
    int g;
    
    fprintf(stdout, "\t");
    for (g = 0; g < NUM_GENES; g++)
        fprintf(stdout, "%d", ind->gene[g]);
    fprintf(stdout, "\tfitness = %d", ind->fitness);
	printf("\n");
}

void calculateFitness(Individual ind){
	ind->fitness = evaluateFitness(ind->gene);
}
