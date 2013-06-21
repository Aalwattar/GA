/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 31, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : population.c
 * 
 * Purpose  : A library for the creation and manipulation of a population
 *              of individuals
 ******************************************************************************/

#include "config.h"
#include "population.h"
#include "fitness.h"
#include "selection.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

// FIX
static double CROSSOVER_RATE = DEFAULT_CROSSOVER_RATE;
static double MUTATION_RATE  = DEFAULT_MUTATION_RATE;

/*
 * FIXEME move this to wherever  you want
 */
#define RANDOM_RATIO 0.75

Population * genRandPopulation(int pop_size){
    Population * pop;
    int i;
    int sum=0;
    
    pop = malloc(sizeof(Population));
    pop->member = malloc(sizeof(Individual) * pop_size);

    for(i=0; i < pop_size; i++){
    	if(randomNumber() <=RANDOM_RATIO)
    	{sum++;
        initRandIndividual(&(pop->member[i]));
    	}else
    	{
    		initSeededIndividual(&(pop->member[i]));
    	}
    
    }

    fprintf(stderr,"\nNo of random Ind [%d] out of [%d]\n",sum,i);
    pop->size = pop_size;
    
    return pop;
}

void freePopulation(Population * pop){
    int i;
    
    for(i = 0; i < pop->size; i++)
        freeIndividual(&(pop->member[i]));
    
    free(pop->member);
    free(pop);
}



void determineFitness(Population * pop){
    int i;
    
    for (i = 0; i < pop->size; i++)
        evaluateFitness(&(pop->member[i]));
}

/*
 * START:  Added By Ahmed Al-Wattar June 20th, 2013
 */

#define EQUEL_RATIO 0
int calcDistancePopulation(Population * pop) {
	int i, k;
    // CHANGE NAMES TO BE MORE DESCRIPTIVE - SEE PAPER
	int sum = 0;    // hamming distance sum 
	int sumHam=0; // hamming distance of hamming distances  sum
	int sumEqual=0;
	int indDistance;
	static int indDistance_old;
    int *uniqueAr;
    double normalSumHam;
    double normalSum;
    int unique=1;
    int sumUnique =0;
    double normalUnique;
    
    
	fprintf(stderr,
			"WARNING [CalcDistancePopulation] this is a test\n"
					"function do not used it unless you really know what u r  doing \n");
    uniqueAr=calloc(pop->size,sizeof(int));
    
    for (i = 0; i < pop->size - 1; i++) {
        unique=1;
		for (k = i + 1; k < pop->size; ++k) {

			indDistance = calcIndividualHamDistance(&(pop->member[i]),
					&(pop->member[k]));

			sum += indDistance;
			if (i!=0 && k!=1){
				sumHam+=!(indDistance==indDistance_old);
            }
            
           /*FIXME speedup there is no need to do all the time
            */
             if (abs(pop->member[i].fitness - pop->member[k].fitness)<=EQUEL_RATIO)
             {
                 unique=0;
                 uniqueAr[k]=1;
                 uniqueAr[i]=1;
                 
                 
             }
            
			//sumEqual+= (abs(pop->member[i].fitness - pop->member[k].fitness)<=EQUEL_RATIO);
            
//			fprintf(stderr, " Indi Dist [%d & %d]-> [%d] h[%d]\n", i,k,
//					indDistance,!(indDistance==indDistance_old));
			indDistance_old=indDistance;

		}
        
        
	}
    for (i = 0; i < pop->size; i++) {
        if(uniqueAr[i] == 0)
            sumUnique++;
    }
    
    normalSumHam =  sumHam / pow(pop->size, 2) * getNumGenes() / 2.0;
    normalSum =  sum / pow(pop->size, 2) * getNumGenes() / 2.0;
    normalUnique = (double) sumUnique / pop->size;
    
	fprintf(stdout," \n\nHammig distance sum [%f] h[%f] --- Unique -->[%f]\n\n  ", normalSum, normalSumHam,normalUnique);
	return sum;
}

void initStatArray(int stat[][10], int imax, int jmax) {
	int i, j;
	for (i = 0; i < imax; ++i) {
		for (j = 0; j < jmax; ++j) {
			stat[i][j] = 0;

		}

	}
}

void calcPercentagePopulation(Population * pop) {
	int i, j;
	/*
	 * FIXME this is ugly but we just need for one simple test
	 */
	fprintf(stderr,
			"WARNING [CalcPercentagePopulation] this is a test\n"
					"function do not used it unless you really know what u r doing \n");
	int stat[500][10];

	initStatArray(stat, 500, 10);
	for (i = 0; i < pop->size; i++) {
		calcIndividualPercentage(&(pop->member[i]), stat);
	}

	for (i = 0; i < getNumGenes(); ++i) {
		fprintf(stdout,"Gene[%d]---> \t",i);
		for (j = 0; j < getNumArch(getTaskType(i)); j++) {

			fprintf(stdout, "\t[%%%d]",(int)((100 * stat[i][j]) / (float) pop->size));

		}
		fprintf(stdout,"\n");
	}
}

/*
 * END change by Ahmed Al-Wattar
 */

// The compare function for qsort
int compare(const void * p1, const void * p2){
    return ((Individual *)p1)->fitness - ((Individual *)p2)->fitness;
}

void sortByFitness(Population * pop){
    qsort(pop->member, pop->size, sizeof(Individual), compare);
}



void evolvePopulation(Population * pop, int crossover_type, int mutation_type){
    int i;
    
    for(i=0; i + 1 < pop->size; i = i + 2)
        if(randomNumber() < CROSSOVER_RATE){
            if(crossover_type == 1)
                onePointCrossover(&(pop->member[i]), &(pop->member[i + 1]));
            else
                twoPointCrossover(&(pop->member[i]), &(pop->member[i + 1]));
        }
                
    if(mutation_type == 1)
        for(i=0; i < pop->size; i++)
            mutateRotationally(&(pop->member[i]));
    else
        for(i=0; i < pop->size; i++)
            mutateRandomly(&(pop->member[i]));
}



void setCrossoverRate(double rate){
    if(0 <= rate && rate <= 1){
        CROSSOVER_RATE = rate;
        return;
    }
    
    fprintf(stderr, "Invalid crossover rate %.3lf.\n", rate);
    fprintf(stderr, "The crossover rate must be a decimal number between 0 and 1\n");
    
    exit(1);
}

double getCrossoverRate(void){
    return CROSSOVER_RATE;
}


void setMutationRate(double rate){
    if(0 <= rate && rate <= 1){
        MUTATION_RATE = rate;
        return;
    }
    
    fprintf(stderr, "Invalid mutation rate %.3lf.\n", rate);
    fprintf(stderr, "The mutation rate must be a decimal number between 0 and 1\n");
    
    exit(1);
}

double getMutationRate(void){
    return MUTATION_RATE;
}


void printPopulation(Population * pop){
    #if (defined VERBOSE || defined DEBUG)
        int i;

        for (i = 0; i < pop->size; i++)
            printIndividual(&(pop->member[i]));
    #endif
    
    printSummaryStatistics(pop);
    calcDistancePopulation(pop);
    
    
}

void printSummaryStatistics(Population * pop){
    double mean;
    double differenceSum;
    double sd;
    
    int max = 0;
    int min = INT_MAX;
    int fitnessSum;
    int i;
    
    fitnessSum = 0;
    for(i = 0; i < pop->size; i++){
        
        fitnessSum = fitnessSum + pop->member[i].fitness;
        
        if(pop->member[i].fitness > max)
            max = pop->member[i].fitness;
        
        if(pop->member[i].fitness < min)
            min = pop->member[i].fitness;
    }
    
    mean = (double) fitnessSum / pop->size;
    
    differenceSum = 0;
    for(i = 0; i < pop->size; i++)
        differenceSum = differenceSum + pow(mean - pop->member[i].fitness, 2);
    
    sd = sqrt(differenceSum / (pop->size-1));
    
    fprintf(stdout, "\nAverage = %.3lf\n", mean);
    fprintf(stdout, "SD      = %.3lf\n", sd);
    fprintf(stdout, "Min     = %d\n", min);
    fprintf(stdout, "Max     = %d\n", max);
    
//    // concise version
//    fprintf(stdout, "Stats : %.5lf,\t%.5lf,\t%d,\t%d\n", mean, sd, min, max);
}
