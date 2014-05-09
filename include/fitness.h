/*******************************************************************************
 * Filename : fitness.h
 * Purpose  : The header file for fitness.c
 *
 * Author   : Jennifer Winer
 *
 * Created  : May 16, 2013
 * Modified : May 8, 2014
 ******************************************************************************/


#ifndef FITNESS_H
#define FITNESS_H

int getNumGenes(void);
int getNumAlleles(int);

int evaluateFitness(int *);

#endif /* FITNESS_H */
