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
 * NAME : newRandPopulation
 * PURPOSE : Creates a population of individuals. Each individual will
 *              contain randomly generated genes, but is still a valid solution
 * 
 * RETURNS : A population of randomly generated individuals
 * 
 * NOTE : the population that is returned should eventually be freed through
 *              the usage of freePopulation();
 *****************************************************************************/
Population newRandPopulation();

/******************************************************************************
 * NAME : freePopulation
 * PURPOSE : Frees all dynamically allocated data from within a Population
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by newRandPopulation();
 *****************************************************************************/
void freePopulation(Population);


/******************************************************************************
 * NAME : evolvePopulation
 * PURPOSE : Perform recombination and mutation on the provided Population
 *****************************************************************************/
void evolvePopulation(Population);


/******************************************************************************
 * NAME : determineFitness
 * PURPOSE : Calculate the fitness of all members of a population
 *****************************************************************************/
void determineFitness(Population);



/******************************************************************************
 * NAME : sortByFitness
 * PURPOSE : Orders the individuals in a population in ascending order by
 *              their fitness
 *****************************************************************************/
void sortByFitness(Population);

/******************************************************************************
 * NAME: sortByReverseFitness
 * PURPOSE: Orders the individuals in a population in descending order by
 * 				their fitness
 *****************************************************************************/
void sortByReversedFitness(Population);

/******************************************************************************
 * NAME : printPopulation
 * PURPOSE : Print every individual's genotype with their fitness values
 *****************************************************************************/
void printPopulation(Population);

/******************************************************************************
 * NAME : printSummaryStatistics
 * PURPOSE : Print statistical information about a population to stdout
 *****************************************************************************/
void printSummaryStatistics(Population);


#endif	/* POPULATION_H */

