/*******************************************************************************
 * FileName : individual.h
 * Purpose  : a header for individual.c
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May  7, 2013
 * Modified : May 13, 2014
 ******************************************************************************/

#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H

#include "util.h"


typedef struct individual * Individual;

/******************************************************************************
 ***************         Construction and Initialization        ***************
 *****************************************************************************/

/******************************************************************************
 * Name : initIndividualClass
 *
 * Purpose : Initialized the static "class" variables
 *
 * Arguments : mutRate = The probably of an individual gene randomly mutating
 * 			   numGenes = The number of genes in a chromosome
 * 			   numAllelesFunc = a function that when given the position of a
 * 			   		gene in the chromosome, returns the number of alleles
 * 			   		that gene possesses
 * 			   fitnessFunc = a function that when given a chromosome, will
 * 			   		return the fitness score of that individual
 *
 * NOTE : This algorithm MUST be called before all other methods in this file
 *****************************************************************************/
void initIndividualClass(double mutRate, int numGenes,
		int (*numAllelesFunc)(int gene), int (*fitnessFunc)(int * chromosome));


/******************************************************************************
 * Name : newRandIndividual
 *
 * Purpose : Creates an individual with a randomly generated genotype
 *****************************************************************************/
Individual newRandIndividual(void);


/******************************************************************************
 * Name : cloneIndividual
 * 
 * Purpose : Creates a deep copy of the individual passed in as an argument
 *****************************************************************************/
Individual cloneIndividual(Individual original);


/******************************************************************************
 * Name : freeIndividual
 * 
 * Purpose : Frees all dynamically allocated data from within an Individual
 *****************************************************************************/
void freeIndividual(Individual i);



/******************************************************************************
 ***************************         Fitness        ***************************
 *****************************************************************************/

/******************************************************************************
 * Name : getFitness
 *
 * Purpose : Return the fitness of an individual
 *****************************************************************************/
int getFitness(Individual);

/******************************************************************************
 * Name : compareIndividuals
 *
 * Purpose : Used by qsort to sort individuals in descending order
 *****************************************************************************/
int compareIndividuals(const void *, const void *);

/******************************************************************************
 * Name : compareIndividualsReversed
 *
 * Purpose : Used by qsort to sort individuals in ascending order
 *****************************************************************************/
int compareIndividualsReversed(const void *, const void *);



/******************************************************************************
 ************************         Manipulation        *************************
 *****************************************************************************/

/******************************************************************************
 * Name : mutate
 * 
 * Purpose : Perform point mutation on an Individual.
 *
 * NOTE : This method should be used on every Individual in a population
 *****************************************************************************/
void mutate(Individual ind);

/******************************************************************************
 * Name : crossover
 * 
 * Purpose : Perform two point crossover on the two individuals provided.
 *****************************************************************************/
void crossover(Individual ind1, Individual ind2);



/******************************************************************************
 **************************         Display        ****************************
 *****************************************************************************/

/******************************************************************************
 * Name : printIndividual
 *
 * Purpose : Print the genotype and fitness of an individual to stdout
 *****************************************************************************/
void printIndividual(Individual);



#endif	/* INDIVIDUAL_H */





