/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : July 10, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : fitness.h
 * 
 * Purpose  : a header for fitness.c
 ******************************************************************************/


#ifndef FITNESS_H
#define	FITNESS_H

#include <stdbool.h>
#include "individual.h"

/******************************************************************************
 *****************           ARCHITECTURE FILE I/O            *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : getNumArch
 * 
 * PURPOSE : return the number of architectures that exist for that task
 * ARGUMENTS : int = which task that we are implementing
 * 
 * RETURNS : false if the filename could not be found or opened or the file did 
 *              not follow the specified format
 *           true otherwise (successful completion)
 * 
 * NOTE : please see the README file for more information about the format
 *              and contents of the architecture information file
 *****************************************************************************/
int getNumArch(int);



/******************************************************************************
 *****************        FITNESS FUNCTION (NAPOLEON)         *****************
 *****************************************************************************/        


// FIX - all comments bellow this line are wrong
/******************************************************************************
 * NAME : initNapoleon
 * 
 * PURPOSE : Create and initialize all of Napoleon's static data structures
 * ARGUMENTS : char * = the name of the file that contains the DFG that you
 *                        wish to schedule (please see Ahmed Al-Wattar for 
 *                        more information
 * 
 * RETURNS : true if Napoleon was successfully initialized
 *           false if anything has gone wrong (error message printed to stderr)
 *****************************************************************************/
bool initScheduler(char *, char *, char *);

// FIX
/******************************************************************************
 * NAME : freeNapoleon
 * 
 * PURPOSE : free all of Napoleon's static data structures
 * PRECONDITION : This function should only be called if initNapoleon() 
 *                  returned true.
 *****************************************************************************/
void freeScheduler(void);

/******************************************************************************
 * NAME : evaluateFitness
 * 
 * PURPOSE : Evaluate the fitness of one possible solution (a chromosome)
 * ARGUMENTS : Individual * = the possible solution to be evaluated
 *****************************************************************************/
void evaluateOfflineFitness(Individual *);

/******************************************************************************
 * NAME : evaluateFitness
 * 
 * PURPOSE : Evaluate the fitness of one possible solution (a chromosome)
 * ARGUMENTS : Individual * = the possible solution to be evaluated
 *****************************************************************************/
void evaluateOnlineFitness(Individual *);



/******************************************************************************
 * NAME : getNumGenes
 * 
 * PURPOSE : getter for the number of genes in a chromosome (AKA the number of
 *              nodes in the DFG that we are trying to schedule)
 * PRECONDITIONS : This function should only be called after initNapoleon()
 *                      returns true.
 * 
 * RETURNS : the number of genes in a chromosome
 *****************************************************************************/
int getNumGenes(void);

/******************************************************************************
 * NAME : getTaskType
 * 
 * PURPOSE : getter for the type of each gene in a chromosome (AKA the type of
 *              task of each node in the DFG)
 * ARGUMENTS : int = the position of the gene (task) in a chromosome
 * 
 * PRECONDITIONS : This function should only be called after initNapoleon()
 *                      returns true.
 * 
 * RETURNS : the type of task (or Operation) of the chosen task
 *****************************************************************************/
int getTaskType(int);

/******************************************************************************
 * NAME : setRuntimeWeight
 * 
 * PURPOSE : The fitness function is derived of X% schedule runtime, and 
 *              (1 - X)% power. This function sets the value of X
 * ARGUMENTS : double * = The weight of the runtime when calculating a
 *                        chromosome's fitness
 * 
 * PRECONDITIONS: The argument must be a number between 0 and 1 inclusive
 *****************************************************************************/
void setRuntimeWeight(double);

/******************************************************************************
 * NAME : getRuntimeWeight
 * 
 * PURPOSE : return the weight of runtime from the schedule that the chromosome
 *              produces when calculating a chromosome's fitness
 * 
 * RETURNS : runtime's weight
 *****************************************************************************/
double getRuntimeWeight(void);


void setHardwareSetup(int index);


#endif	/* FITNESS_H */

