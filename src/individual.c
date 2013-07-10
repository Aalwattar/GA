/*******************************************************************************
 * Author   : Jennifer Winer & Ahmed Al-Wattar
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : June 24, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : Individual.c
 * 
 * Purpose  : All methods for the creation and manipulation of a possible
 *              solution to the problem (an individual in the population)
 ******************************************************************************/

#include "config.h"
#include "individual.h"
#include "fitness.h"
#include "population.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>


void initRandIndividual(Individual * ind){
    int i;
    
    ind->encoding = malloc(sizeof(int) * getNumGenes());
        
    for(i=0; i<getNumGenes(); i++)
        ind->encoding[i] = getNumArch(getTaskType(i)) * randomNumber();
}

// FIX - get rid of MAX_NUM_GENES
void initSeededIndividual(Individual * ind){
    static int alleleTracker[MAX_NUM_GENES];
	int i;

	ind->encoding = malloc(sizeof(int) * getNumGenes());

    // FIX - the logic right here should be able to be shortened from 4 lines to three. or two.
	for (i=0; i < getNumGenes(); i++){
		if (alleleTracker[i] == getNumArch(getTaskType(i)) - 1){
			alleleTracker[i] = 0;
		}

		ind->encoding[i] = (alleleTracker[i])++;
	}
}

void freeIndividual(Individual * i){
    free(i->encoding);
}

void duplicateIndividual(Individual * copy, Individual * original){
    int i;
    
    copy->encoding = malloc(sizeof(int) * getNumGenes());
    
    for(i=0; i<getNumGenes(); i++)
        copy->encoding[i] = original->encoding[i];
    
    copy->energy =      original->energy;
    copy->exec_time =   original->exec_time;
    copy->fitness =     original->fitness;
    copy->num_reuse =   original->num_reuse;
    copy->prefetch =    original->prefetch;

}

void mutateRotationally(Individual * ind){
    int i;
    
    for(i=0; i<getNumGenes(); i++)
        if(randomNumber() < getMutationRate()){
            ind->encoding[i] = (ind->encoding[i] + 1) % getNumArch(getTaskType(i));
        }
}              

void mutateRandomly(Individual * ind){
    int new_gene;
    int i;
    
    for(i=0; i<getNumGenes(); i++)
        if(randomNumber() < getMutationRate()){
            new_gene = getNumArch(getTaskType(i)) * randomNumber();
            
            while(new_gene == ind->encoding[i])
                new_gene = getNumArch(getTaskType(i)) * randomNumber();
                
            ind->encoding[i] = new_gene;
        }
}  

void onePointCrossover(Individual * p1, Individual * p2){
    int cross_point;
    int temp;
    int i;
    
    cross_point = getNumGenes() * randomNumber();
    
    while(cross_point == 0 || cross_point == getNumGenes())
        cross_point = getNumGenes() * randomNumber();
    
    for(i=cross_point; i < getNumGenes(); i++){
        temp = p1->encoding[i];
        p1->encoding[i] = p2->encoding[i];
        p2->encoding[i] = temp;
    }
}

void twoPointCrossover(Individual * p1, Individual * p2){
    int cross1, cross2;
    int temp;
    int i;
    
    cross1 = getNumGenes() * randomNumber();
    cross2 = getNumGenes() * randomNumber();
    
    while(cross1 == cross2)
        cross2 = getNumGenes() * randomNumber();

    if(cross1 > cross2){
        temp = cross1; 
        cross1 = cross2;
        cross2 = temp;
    }
    
    for(i=cross1; i <= cross2; i++){
        temp = p1->encoding[i];
        p1->encoding[i] = p2->encoding[i];
        p2->encoding[i] = temp;
    }
}

void printIndividual(Individual * ind){
    int i;
    
    // print the chromosome
    for (i = 0; i < getNumGenes(); i++)
        fprintf(stdout, "%d", ind->encoding[i] + 1);
    
    // Napoleon information
    fprintf(stdout, "\tfitness = %d\truntime = %d\tprefetch = %d\tpower = %d\treuse = %d\n", 
                ind->fitness, ind->exec_time, ind->prefetch, ind->energy, ind->num_reuse);
}