/*******************************************************************************
 * Filename : replacement.c
 * Purpose  : A library of replacement policies
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May 16, 2013
 * Modified : May 8, 2014
 ******************************************************************************/

#include "replacement.h"
#include "population.h"

#include <stdio.h>

void replaceWorst(Population original, Population replacements, int num_replaced){
    int i;
    
    /*
    sortByReversedFitness(original);
    sortByFitness(replacements);
    
    for(i = 0 ; i < num_replaced; i++){
    	freeMember(original, i);
    	original->member[i] = cloneIndividual(replacements->member[i]);
    }
    */
}

// FUTURE - replace parents instead of replace worst???
