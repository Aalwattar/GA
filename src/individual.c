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


// FIXME - NUM_GENES should be localized to ONLY this file
// FIXME - int getNumAlleles(int);
// FIXME - int getGeneType(int);
// FIXME - find an invalid fitness value to set in newly generated individuals

struct individual{
	int * gene;
	int fitness;
};

// Private method that allocates memory for an Individual.
// 	it does NOT set default values to its members
Individual newIndividual(){
	Individual ind;

	ind = (Individual) malloc(sizeof(struct individual));
	ind->gene = malloc(sizeof(int) * getNumGenes());

	return ind;
};

Individual newRandIndividual(){
	Individual ind;
    int g;
    
    ind = newIndividual();
    for(g=0; g<getNumGenes(); g++)
        ind->gene[g] = (getNumAlleles(getGeneType(g))) * randomNumber();
    // ind->fitness = 0;

    return ind;
}

Individual duplicateIndividual(Individual original){
	Individual copy;
    int g;
    
    copy = newIndividual();
    for(g=0; g<getNumGenes(); g++)
        copy->gene[g] = original->gene[g];
    // copy->fitness = original->fitness;

    return copy;
}

void freeIndividual(Individual i){
    free(i->gene);
    free(i);
}



// FIXME - broken method!!!
// FIXME - only one of the population / individuals should have access the the mutation rate - NOT BOTH
void mutate(Individual ind){
    int g;
    
    for(g=0; g<getNumGenes(); g++)
        if(randomNumber() < getMutationRate())
            ind->gene[g] = (getNumAlleles(getGeneType(g))) * randomNumber();
}                              

void crossover(Individual ind1, Individual ind2){
    int gene1, gene2;
    int temp;
    int g;
    
    // choose two different "cutting points" FIXME - the English here is atrocious ....
    gene1 = getNumGenes() * randomNumber();
    gene2 = getNumGenes() * randomNumber();
    
    while(gene1 == gene2)
        gene2 = getNumGenes() * randomNumber();

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
    
    for (g = 0; g < getNumGenes(); g++)
        fprintf(stdout, "%d", ind->gene[g]);
}
