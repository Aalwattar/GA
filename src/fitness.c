/*******************************************************************************
 * Filename : fitness.c
 * Purpose  : A library of fitness functions that frame the current problem
 * 				into a genotype
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 16, 2013
 * Modified : June 6, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : fitness.c
 * 
 * Purpose  : A library for integration with Ziad's Napoleon
 * 
 *            Napoleon will act as our fitness function, taking a mapping of 
 *              architectures to each task as input, and returning the runtime
 *              and power required for that schedule as a fitness function
 ******************************************************************************/

#include "fitness.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#define BUFF_SIZE 500

