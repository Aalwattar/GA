/*******************************************************************************
 * Filename : population.h
 * Purpose  : a header for population.c
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 7, 2013
 * Modified : May 8, 2014
 ******************************************************************************/

#ifndef POPULATION_H
#define	POPULATION_H

#include "individual.h"

typedef struct population * Population;

/******************************************************************************
 ******************         POPULATION MANIPULATION         *******************
 *****************************************************************************/

// FIXME - comment here
// this function sets up the static "class" variables such as the fitness
// 		function and the mutation rate
void initPopulationClass(double crossRate, double mutRate, int popSize, int numGenes, int (*numAllelesFunc)(int), int (*fitnessFunc)(int *));


/******************************************************************************
 * NAME : genRandPopulation
 * 
 * PURPOSE : Creates a population of individuals. Each individual will
 *              contain randomly generated genes, but is still a valid solution
 * ARGUMENTS : int = the number of individuals in the population
 * 
 * RETURNS : A population of randomly generated individuals
 * 
 * NOTE : the population that is returned should eventually be freed through
 *              the usage of freePopulation();
 *****************************************************************************/
Population newRandPopulation();

/******************************************************************************
 * NAME : freePopulation
 * 
 * PURPOSE : Frees all dynamically allocated data from within a Population
 * ARGUMENTS : Population * = the population that you wish to free
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by genRandPopulation();
 *****************************************************************************/
void freePopulation(Population);



/******************************************************************************
 * NAME : evolvePopulation
 * 
 * PURPOSE : Perform recombination and mutation on the provided Population
 * ARGUMENTS : Population * = the population that you wish to transform
 *             int = the size of that population;
 *****************************************************************************/
void evolvePopulation(Population);


/******************************************************************************
 * NAME : determineFitness
 * 
 * PURPOSE : Calculate the fitness of all members of a population
 * ARGUMENTS : Population * = the population that you wish evaluate
 *****************************************************************************/
void determineFitness(Population);



/******************************************************************************
 * NAME : evaluateRanks
 * 
 * PURPOSE : Orders the individuals in a population in ascending order
 *              based on fitness
 * ARGUMENTS : Population * = the Population that you wish to evaluate
 *****************************************************************************/
void sortByFitness(Population);



/******************************************************************************
 * NAME : printPopulation
 * 
 * PURPOSE : Print all chromosomes with their fitness values
 * ARGUMENTS : Population * = the population that you wish visualize
 *****************************************************************************/
void printPopulation(Population);

/******************************************************************************
 * NAME : printSummaryStatistics
 * 
 * PURPOSE : Statistical information about a population. 
 * ARGUMENTS : Population * = the population that you wish visualize
 *****************************************************************************/
void printSummaryStatistics(Population);


#endif	/* POPULATION_H */

