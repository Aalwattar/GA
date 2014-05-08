/*******************************************************************************
 * Filename : population.c
 * Purpose  : A library for the creation and manipulation of a population
 *              of individuals
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 7, 2013
 * Modified : May 8, 2014
 ******************************************************************************/

#include "population.h"
// FIXME - do I need to include individual.h??
#include "selection.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

// FIXME - These should be passed in from main, not hard coded values
// they should NOT have an initial value
static double CROSSOVER_RATE = 0.85;
static int POP_SIZE = 50;

struct population{
    Individual * member;
};

void initPopulationClass(double crossRate, double mutRate, int popSize, int numGenes, int (*numAllelesFunc)(int), int (*fitnessFunc)(int *)){
	if(crossRate < 0 || crossRate > 1){
		fprintf(stderr, "Invalid crossover rate: %.3lf.\n", crossRate);
		fprintf(stderr, "The crossover rate must be a decimal number between 0 and 1\n");
		exit(1);
	}

	if(popSize < 2){
		fprintf(stderr, "Invalid population size: %d\n", popSize);
		fprintf(stderr, "The number of genes must be greater than zero.\n");
		exit(1);
	}

	CROSSOVER_RATE = crossRate;
	POP_SIZE = popSize;
	initIndividualClass(mutRate, numGenes, numAllelesFunc, fitnessFunc);
}

// FIXME - add a proper comment
//  Private method that allocates memory for an Individual.
// 	it does NOT set default values to its members
Population newPopulation(){
	Population pop;

	pop = (Population) malloc(sizeof(struct population));
	pop->member = (Individual *) malloc(sizeof(Individual) * POP_SIZE);

	return pop;
}

Population newRandPopulation(){
    Population pop;
    int i;
    
    pop = newPopulation();
    for(i=0; i < POP_SIZE; i++)
    	pop->member[i] = newRandIndividual();
    
    return pop;
}

void freePopulation(Population pop){
    int i;
    
    for(i = 0; i < POP_SIZE; i++)
        freeIndividual(pop->member[i]);
    
    free(pop->member);
    free(pop);
}



void determineFitness(Population pop){
    int i;
    
    for (i = 0; i < POP_SIZE; i++)
        evaluateFitness(&(pop->member[i]));
    // 	FIXME - make evaluateFitness wrapper for Individual class?
}


// PRIVATE - The compare function for qsort
int compareIndividuals(const void * p1, const void * p2){
    return getFitness(p1) - getFitness(p2);
}

void sortByFitness(Population pop){
    qsort(pop->member, POP_SIZE, sizeof(Individual), compareIndividuals);
}



void evolvePopulation(Population pop){
    int i;
    
    for(i=0; i + 1 < POP_SIZE; i = i + 2)
        if(randomNumber() < CROSSOVER_RATE)
			crossover(pop->member[i], pop->member[i + 1]);
    
    // Analyzing each gene separately is an unneccessarily slow process
    // TODO - consider applying the mutation rate at the individual level. Then those chosen to "mutate" have some fixed percentage of their genes changed?
    for(i=0; i < POP_SIZE; i++)
        mutate(pop->member[i]);
}




void printPopulation(Population pop){
    int i;

    for (i = 0; i < POP_SIZE; i++)
        printIndividual(pop->member[i]);
    
    printSummaryStatistics(pop);
}

void printSummaryStatistics(Population pop){
    double mean;
    double differenceSum;
    double sd;
    
    int max = 0;
    int min = INT_MAX;
    int fitnessSum;
    int i;
    
    fitnessSum = 0;
    for(i = 0; i < POP_SIZE; i++){
        fitnessSum = fitnessSum + getFitness(pop->member[i]);
        
        if(getFitness(pop->member[i]) > max)
            max = getFitness(pop->member[i]);
        
        if(getFitness(pop->member[i]) < min)
            min = getFitness(pop->member[i]);
    }
    
    mean = (double) fitnessSum / POP_SIZE;
    
    differenceSum = 0;
    for(i = 0; i < POP_SIZE; i++)
        differenceSum = differenceSum + pow(mean - getFitness(pop->member[i]), 2);
    
    sd = sqrt(differenceSum / POP_SIZE);
    
    fprintf(stdout, "\nAverage = %.3lf\n", mean);
    fprintf(stdout, "SD      = %.3lf\n", sd);
    fprintf(stdout, "Min     = %d\n", min);
    fprintf(stdout, "Max     = %d\n", max);
    
//    // concise version
//    fprintf(stdout, "Stats : %.5lf,\t%.5lf,\t%d,\t%d\n", mean, sd, min, max);
}
