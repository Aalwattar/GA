/*******************************************************************************
 * Filename : util.c
 * Purpose  : library for utility functions such as random number generation
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 7, 2013
 * Modified : May 8, 2014
 ******************************************************************************/

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


/******************************************************************************
 *****************          RANDOM NUMBER GENERATION          *****************
 *****************************************************************************/

void seedRandGenerator(int seed) {
    srand(seed);
}

int randSeed(void) {
    return time(NULL);
}

double randomNumber(void) {
    return rand() / (double) RAND_MAX;
}
