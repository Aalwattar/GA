/*******************************************************************************
 * Filename : fitness.h
 * Purpose  : The header file for fitness.c
 *
 * Author   : Jennifer Winer
 *
 * Created  : May 16, 2013
 * Modified : May 13, 2014
 ******************************************************************************/


#ifndef FITNESS_H
#define FITNESS_H

/******************************************************************************
 * Name : initProblem
 *
 * Purpose : Initializes the Genetic algorithm with the problem description
 * 				located in the file provided
 *
 * Arguments : filename = the name of the file containing description of
 * 				the problem
 *
 * NOTE : This algorithm MUST be called before all other methods in this file
 *****************************************************************************/
void initProblem(char * filename);

/******************************************************************************
 * Name : freeProblem
 *
 * Purpose : frees any dynamically allocated memory from inside fitness.c
 *****************************************************************************/
void freeProblem(void);



/******************************************************************************
 * Name : getNumGenes
 *
 * Purpose : Return the number of genes in a chromosome
 *****************************************************************************/
int getNumGenes(void);

/******************************************************************************
 * Name : getNumAlleles
 *
 * Purpose : Return number of alleles a gene may take
 *****************************************************************************/
int getNumAlleles(int position);

/******************************************************************************
 * Name : evaluateFitness
 *
 * Purpose : calculate the fitness value of a chromosome
 *****************************************************************************/
int evaluateFitness(int * chromosome);

#endif /* FITNESS_H */
