/*******************************************************************************
 * Filename : individual.h
 * Purpose  : a header for individual.c
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 7, 2013
 * Modified : May 8, 2014
 ******************************************************************************/

#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H

#include "util.h"


typedef struct individual * Individual;

/******************************************************************************
 ******************         INDIVIDUAL MANIPULATION         *******************
 *****************************************************************************/

// FIXME - comment here
// this function sets up the static "class" variables such as the fitness
// 		function and the mutation rate
void initIndividualClass(double mutRate, int numGenes, int (*numAllelesFunc)(int), int (*fitnessFunc)(int *));


/******************************************************************************
 * NAME : newRandIndividual
 *
 * PURPOSE : Creates an individual with a randomly generated genotype
 *****************************************************************************/
Individual newRandIndividual(void);


/******************************************************************************
 * NAME : cloneIndividual
 * 
 * PURPOSE : Creates a deep copy of the individual passed in as an argument 
 *****************************************************************************/
Individual cloneIndividual(Individual);


/******************************************************************************
 * NAME : freeIndividual
 * 
 * PURPOSE : Frees all dynamically allocated data from within an Individual
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through newRandIndividual() or
 *      cloneIndividual().
 *****************************************************************************/
void freeIndividual(Individual);


// FIXME - needs a comment
int getFitness(Individual);

// FIXME - needs commenting
int compareIndividuals(const void *, const void *);
int compareIndividualsReversed(const void *, const void *);



/******************************************************************************
 * NAME : mutate
 * 
 * PURPOSE : Perform mutation on an Individual. The probability of mutating one
 *      individual gene (MUTATION_RATE) is pre-determined by the user.
 * 
 * RECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through newRandIndividual() or
 *      cloneIndividual().
 *****************************************************************************/
void mutate(Individual);

/******************************************************************************
 * NAME : crossover
 * 
 * PURPOSE : Perform crossover on the two individuals provided. The probability
 *       of mutating one individual gene (CROSSOVER_RATE) is pre-determined by
 *       the user.
 * 
 * RECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through newRandIndividual() or
 *      cloneIndividual().
 *****************************************************************************/
void crossover(Individual, Individual);


/******************************************************************************
 * NAME : printIndividual
 *
 * PURPOSE : Display the genotype of an individual
 *****************************************************************************/
void printIndividual(Individual);

// FIXME - needs a comment
void calculateFitness(Individual);

#endif	/* INDIVIDUAL_H */

