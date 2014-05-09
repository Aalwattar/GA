/*******************************************************************************
 * Filename : replacement.h
 * Purpose  : a header for replacement.c
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 16, 2013
 * Modified : May 8, 2014
 ******************************************************************************/


#ifndef REPLACEMENT_H
#define	REPLACEMENT_H

#include "population.h"

/******************************************************************************
 * NAME : replaceWorst
 * 
 * PURPOSE : Replaces the worst individuals in one population with 
 *              the best individuals from another
 *
 * ARGUMENTS : Population = the destination population
 *             Population = the source population
 *             int = the number of individuals that you wish to replace
 * 
 * PRECONDITIONS : Both populations must have already had the fitnesses of
 *                      their members evaluated (by a previous call to the
 *                      function evaluateFitness)
 * 
 * NOTE : This algorithm sorts each of the populations according to fitness
 *****************************************************************************/
void replaceWorst(Population, Population, int);

#endif	/* REPLACEMENT_H */

