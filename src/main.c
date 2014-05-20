/*******************************************************************************
 * Filename : main.c
 * Purpose  : Combine population functions into different GAs
 *
 * Author   : Jennifer Winer
 * 
 * Created  : May  7, 2013
 * Modified : May 13, 2014
 ******************************************************************************/

#include "population.h"
#include "fitness.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define PROBLEM_FILENAME "input/problem.txt"

static int STOP_CONDITION = 500;


void initParameters(int, char **);

Population generationalGA(Population);
Population elitestGA(Population);

// TODO - implement this
// bool populationConverged(Population * pop);


int main(int argc, char * argv[]){
	Population pop;
	Individual best_solution;
	int generation_num = 0;

    initParameters(argc, argv);

	pop = newRandPopulation();
#ifdef VERBOSE
	fprintf(stdout, "\n----------------------------------------------------------\n\n");
	fprintf(stdout, "Starting Population:\n");
	printPopulation(pop);
#endif

    while(generation_num < STOP_CONDITION){

#if  (defined VERBOSE || defined EXE)
		fprintf(stdout, "\n-----------------   GENERATION %d   -----------------\n", generation_num + 1);
		printPopulation(pop);
#endif

		//pop = elitestGA(pop);
		pop = generationalGA(pop);

        generation_num++;
    }

#ifdef VERBOSE
	fprintf(stdout, "\nFinal Population:\n");
	printPopulation(pop);
#endif

	fprintf(stdout, "\n-----------------   FINAL RESULT   -----------------\n");
	best_solution = findBest(pop);
	printIndividual(best_solution);

	freePopulation(pop);
    return EXIT_SUCCESS;
}


Population elitestGA(Population pop){
	Population selected;

	selected = tournamentSelection(pop);
	evolvePopulation(selected);

	replaceWorst(pop, selected);
	freePopulation(selected);

	return pop;
}

Population generationalGA(Population pop){
	Population selected;
        
	selected = tournamentSelection(pop);
	freePopulation(pop);

	evolvePopulation(selected);
	return selected;
}

void initParameters(int argc, char ** argv){
    char * problem_filename = PROBLEM_FILENAME;
    double crossover_rate = 0.8;
    double mutation_rate  = 0.05;
    int pop_size = 50;
    int seed = randSeed();
    int c;

    opterr = 0;
    while((c = getopt(argc, argv, "c:g:i:m:p:r:s:t:")) != -1){
        switch(c){
            case 'c':
            	crossover_rate = atof(optarg);
                break;
            case 'g':
                STOP_CONDITION = atoi(optarg);
                break;
            case 'i':
				problem_filename = optarg;
				break;
            case 'm':
                mutation_rate = atof(optarg);
                break;
            case 'p':
                pop_size = atoi(optarg);
                break;
            case 'r':
                // FIX - REPLACEMENT METHOD = generatinoal or replaceWorst with varying parameters
                break;
            case 's':
                // FIX - SELECTION METHOD = tournament selection or random
                break;
            case 't':
                seed = atoi(optarg);
                break;
                
            case ':':   
                fprintf(stderr, "Option -%c requires an operand\n", optopt);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);

            default:
                exit(1);
        }
    }

    if(optind < argc){
        printf("Non-option argument %s\n", argv[optind]);
        exit(1);
    }
    
    // FIXME - Check the return values
    seedRandGenerator(seed);
    initProblem(problem_filename);
    initPopulationClass(crossover_rate, mutation_rate, pop_size, getNumGenes(), &getNumAlleles, &evaluateFitness);

    fprintf(stdout, "Parameters:\n");
    fprintf(stdout, "\tSeed = %d\n\n", seed);
    fprintf(stdout, "\tPopulation Size       = %d\n", pop_size);
    fprintf(stdout, "\tNumber of Generations = %d\n\n", STOP_CONDITION);
    fprintf(stdout, "\tMutation Rate  = %.4lf\n", mutation_rate);
    fprintf(stdout, "\tCrossover Rate = %.4lf\n\n", crossover_rate);
}
