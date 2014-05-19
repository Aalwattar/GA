/*******************************************************************************
 * FileName : population.h
 * Purpose  : a header for population.c
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May  7, 2013
 * Modified : May 13, 2014
 ******************************************************************************/

#ifndef POPULATION_H
#define	POPULATION_H

#include "individual.h"

typedef struct population * Population;


/******************************************************************************
 ***************         Construction and Initialization        ***************
 *****************************************************************************/

/******************************************************************************
 * Name : initPopulationClass
 *
 * Purpose : Initialize the static "class" variables
 *
 * Arguments : crossRate = the probability of crossover occurring between each
 * 					pair of individuals
 * 			   popSize = The number of individuals in a population
 *
 * 			   All other Arguments are passed directly to initIndividualClass.
 * 			   Please see individual.h for more information
 *
 * NOTE : This algorithm MUST be called before all other methods in this file
 *****************************************************************************/
void initPopulationClass(double crossRate, double mutRate, int popSize,
							int numGenes, int (*numAllelesFunc)(int),
							int (*fitnessFunc)(int *));

/******************************************************************************
 * Name : newRandPopulation
 *
 * Purpose : Creates a population of individuals. Each individual will
 *              have randomly generated genes, but is still a valid solution
 *****************************************************************************/
Population newRandPopulation();

/******************************************************************************
 * Name : freePopulation
 *
 * Purpose : Frees all dynamically allocated data from within a Population
 *****************************************************************************/
void freePopulation(Population pop);



/******************************************************************************
 ***************************         Fitness        ***************************
 *****************************************************************************/

/******************************************************************************
 * Name : sortByFitness
 *
 * Purpose : Orders the individuals in a population in ascending order by
 *              their fitness
 *
 * Arguments : pop = The population you wish to sort
 * 			   popSize = the number of individuals in the population
 *****************************************************************************/
void sortByFitness(Population pop, int popSize);

/******************************************************************************
 * Name: sortByReverseFitness
 *
 * Purpose: Orders the individuals in a population in descending order by
 * 				their fitness
 *
 * Arguments : pop = The population you wish to sort
 * 			   popSize = the number of individuals in the population
 *****************************************************************************/
void sortByReversedFitness(Population pop, int popSize);

/******************************************************************************
 * Name : findBest
 *
 * Purpose : Finds the individual with the best fitness
 *****************************************************************************/
Individual findBest(Population);



/******************************************************************************
 ************************         Manipulation        *************************
 *****************************************************************************/

/******************************************************************************
 * Name : evolvePopulation
 *
 * Purpose : Perform recombination and mutation on a Population
 *****************************************************************************/
void evolvePopulation(Population pop);

/******************************************************************************
 * Name : tourNamentSelection
 *
 * Purpose : Select individuals from a population based on the the hard
 *              tourNament selection algorithm
 *****************************************************************************/
Population tournamentSelection(Population pop);

/******************************************************************************
 * Name : randomSelection
 *
 * Purpose : Randomly selects individuals from a population.
 *****************************************************************************/
Population randomSelection(Population pop);

/******************************************************************************
 * Name : replaceWorst
 *
 * Purpose : Replaces the worst individuals in one population with
 *              the best individuals from another
 *
 * Arguments : pop1 = the destination population
 *             pop2 = the source population
 *****************************************************************************/
void replaceWorst(Population pop1, Population pop2);



/******************************************************************************
 **************************         Display        ****************************
 *****************************************************************************/

/******************************************************************************
 * Name : printPopulation
 *
 * Purpose : Print every individual's genotype and fitness
 *****************************************************************************/
void printPopulation(Population pop);

/******************************************************************************
 * Name : printSummaryStatistics
 *
 * Purpose : Print statistical information about a population to stdout
 *
 * NOTE : This function was developed by Ahmed Al-Watter.
 *****************************************************************************/
void printSummaryStatistics(Population pop);



#endif	/* POPULATION_H */

