/*******************************************************************************
 * Filename : Individual.c
 * Purpose  : All methods for the creation and manipulation of a possible
 *              solution to the problem (an individual in the population)
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May  7, 2013
 * Modified : May 13, 2014
 ******************************************************************************/

#include "individual.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// TODO - is this variable unncessary?
	// This information will be hard-coded into the problem statement file (fitness.c)
	// BUT, by having a copy of it in this class, you save on the overhead of CONSTANTLY calling the function getNumGenes()
static int    NUM_GENES = 0;
static double MUTATION_RATE  = -1;

static int (*evaluateFitness)(int * genotype);
static int (*getNumAlleles)(int);


struct individual{
	int * chromosome;
	int fitness;
};

/******************************************************************************
 ***************         Construction and Initialization        ***************
 *****************************************************************************/

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

//  Private method that allocates memory for an Individual.
// 	it does NOT set default values to its members
Individual newIndividual(){
	Individual ind;

	ind = (Individual) malloc(sizeof(struct individual));
	ind->chromosome = malloc(sizeof(int) * NUM_GENES);

	return ind;
}

Individual newRandIndividual(){
	Individual ind;
    int g;
    
    ind = newIndividual();
    for(g=0; g<NUM_GENES; g++){
        ind->chromosome[g] = (getNumAlleles(g)) * randomNumber();
    }
    ind->fitness = evaluateFitness(ind->chromosome);

    return ind;
}

Individual cloneIndividual(Individual original){
	Individual copy;
    int g;
    
    copy = newIndividual();
    for(g=0; g<NUM_GENES; g++){
        copy->chromosome[g] = original->chromosome[g];
    }
	copy->fitness = original->fitness;

    return copy;
}

void freeIndividual(Individual i){
    free(i->chromosome);
    free(i);
}



/******************************************************************************
 ***************************         Fitness        ***************************
 *****************************************************************************/

int getFitness(Individual i){
	return i->fitness;
}

void calculateFitness(Individual ind){
	ind->fitness = evaluateFitness(ind->chromosome);
}


// the compare function for qsort
int compareIndividuals(const void * p1, const void * p2){
    return (*(Individual *)p2)->fitness - (*(Individual *)p1)->fitness;
}

// the compare function for qsort
int compareIndividualsReversed(const void * p1, const void * p2){
    return (*(Individual *)p1)->fitness - (*(Individual *)p2)->fitness;
}



/******************************************************************************
 ************************         Manipulation        *************************
 *****************************************************************************/

void mutate(Individual ind){
    int g;
    bool recalcFitness;
    
    // TODO - checking the probability of EVERY gene of EVERY individual is
    	// likely a huge bottleneck. Is there a heuristic I can use that will
    	// reduce the efficiency of this process?

    	// eg. use the mutation rate to determine which INDIVIDUALS will be
    	// 		mutated. Of that subset, mutate as below OR mutate a fixed
    	//		# or % of genes.

    recalcFitness = false;
    for(g=0; g<NUM_GENES; g++){
        if(randomNumber() < MUTATION_RATE){
            ind->chromosome[g] = (getNumAlleles(g)) * randomNumber();
            recalcFitness = true;
        }
    }

    if(recalcFitness){
    	calculateFitness(ind);
    }
}                              

void crossover(Individual ind1, Individual ind2){
    int gene1, gene2;
    int temp;
    int g;
    
    // choose two different points to cut
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

    // Swap all genes between the two positions chosen above (inclusive)
    for(g=gene1; g <= gene2; g++){
        temp = ind1->chromosome[g];
        ind1->chromosome[g] = ind2->chromosome[g];
        ind2->chromosome[g] = temp;
    }

    calculateFitness(ind1);
    calculateFitness(ind2);
}



/******************************************************************************
 **************************         Display        ****************************
 *****************************************************************************/

void printIndividual(Individual ind){
    int g;
    
    fprintf(stdout, "\t");
    for (g = 0; g < NUM_GENES; g++)
        fprintf(stdout, "%d", ind->chromosome[g]);

    fprintf(stdout, "\tfitness = %d", ind->fitness);
	printf("\n");
}


