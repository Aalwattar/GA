/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : June 26, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : main.c
 * 
 * Purpose  : Contains main and executes the GA
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/time.h>

#include "config.h"
#include "fitness.h"
#include "selection.h"
#include "replacement.h"

// FIX
static int STOP_CONDITION = 150;
static int POP_SIZE = 0;

// FIX 
int crossover_type = 2;
int mutation_type = 1;
int replacement_type = 1;
int selection_type = 1;


void initParameters(int, char **);
void freeParameters(void);
void setPopSize(int);

// FIX - implement this
bool populationConverged(Population * pop);


int main(int argc, char * argv[]){
    Population * pop, * selected;
    Individual * best_solution;
    int generation_num = 0;
    
    initParameters(argc, argv);
    pop = genSeededPopulation(POP_SIZE);
//    printGeneComposition(pop);

    determineFitness(pop);
    sortByFitness(pop);

    while(generation_num < STOP_CONDITION){
        #if  (defined VERBOSE || defined EXE)
            fprintf(stdout, "\n-----------------   GENERATION %d   -----------------\n", generation_num + 1);
            printPopulation(pop);
        #endif
        
        // FIX
        if(selection_type == 1)
            selected = tournamentSelection(pop);
        else 
            selected = randomSelection(pop);
            
        // FIX
        evolvePopulation(selected, crossover_type, mutation_type);
        determineFitness(selected);
        
        // FIX
        if(replacement_type == 1)
            pop = replaceAll(pop, selected);
        else  
            pop = retainBest(pop, selected);

        generation_num++;
    }
    
    fprintf(stdout, "\nFINAL RESULT:\n");
    printPopulation(pop);
    best_solution = findBest(pop);
    fprintf(stdout, "\nBEST SOLUTION:\n");
    printIndividual(best_solution);
    
    freePopulation(pop);
    freeParameters();
    return EXIT_SUCCESS;
} 

// FIX - too long
// FIX - print out every parameter selected
void initParameters(int argc, char ** argv){
    char * arch_filename = DEFAULT_ARCH_FILENAME;
    char * dfg_filename = DEFAULT_DFG_FILENAME;
    char * prr_filename = DEFAULT_PRR_FILENAME;
    int seed = randSeed();
    int c;

    opterr = 0;

    while((c = getopt(argc, argv, "a:b:c:d:f:g:h:m:n:o:p:r:s:t:Vvw:")) != -1){
        switch(c){
            case 'a':
                arch_filename = optarg;
                break;
            case 'b':
                if(strncasecmp("one-point", optarg, strlen(optarg))  == 0 || 
                    strncasecmp("one_point", optarg, strlen(optarg)) == 0 ||
                    strncasecmp("one point", optarg, strlen(optarg)) == 0 )
                    crossover_type = 1;
                else if(strncasecmp("two-point", optarg, strlen(optarg)) != 0 &&
                        strncasecmp("two_point", optarg, strlen(optarg)) != 0 &&
                        strncasecmp("two point", optarg, strlen(optarg)) != 0 ){
                    fprintf(stderr, "Unknown crossover option : %s\n", optarg);
                    exit(1);
                }
                break;
            case 'c':
                setCrossoverRate(atof(optarg));
                break;
            case 'd':
                dfg_filename = optarg;
                break;
            case 'f':
                setFitnessFunction(optarg);
                break;
            case 'g':
                STOP_CONDITION = atoi(optarg);
                break;
            case 'h':
                setHardwareSetup(atoi(optarg));
                break;
            case 'm':
                setMutationRate(atof(optarg));
                break;
            case 'n':
               if(strncasecmp("random", optarg, strlen(optarg)) == 0)
                    mutation_type = 2;
                else if(strncasecmp("rotationally", optarg, strlen(optarg)) != 0){ 
                    fprintf(stderr, "Unknown mutation option : %s\n", optarg);
                    exit(1);
                }
                break;
            case 'p':
                setPopSize(atoi(optarg));
                break;
            case 'r':
                if(strncasecmp("keep-best", optarg, strlen(optarg))  == 0 ||
                    strncasecmp("keep_best", optarg, strlen(optarg)) == 0 ||
                    strncasecmp("keep best", optarg, strlen(optarg)) == 0)
                    replacement_type = 2;
                else if(strncasecmp("all", optarg, strlen(optarg)) != 0){
                    fprintf(stderr, "Unknown replacement option : %s\n", optarg);
                    exit(1);
                }
                break;

            case 's':
                if(strncasecmp("random", optarg, strlen(optarg)) == 0)
                    selection_type = 2;
                else if(strncasecmp("tournament", optarg, strlen(optarg)) != 0){ 
                    fprintf(stderr, "Unknown selection option : %s\n", optarg);
                    exit(1);
                }
                break;
                
            case 't':
                seed = atoi(optarg);
                break;
                
            case 'V':
            case 'v':
                // FIX - make it print out the build as well! (#if (defined EXE) printf....)
                fprintf(stdout, "Offline Scheduler version 1.5.1  (GA + Napoleon)\n");
                fprintf(stdout, "Please see https://github.com/Aalwattar/GA for more information\n");
                exit(0);
                
            case 'w':
                setRuntimeWeight(atof(optarg));
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
    
    // FIX - Check the return value
    seedRandGenerator(seed);
    
    // FIX
    initScheduler(arch_filename, dfg_filename, prr_filename);

    
    if(POP_SIZE == 0){
        POP_SIZE = getNumGenes();
        fprintf(stdout, "There are %d genes\n", getNumGenes());
    }
    
    fprintf(stdout, "Parameters:\n");
    fprintf(stdout, "\tSeed = %d\n\n", seed);
    fprintf(stdout, "\tPopulation Size       = %d\n", POP_SIZE);
    fprintf(stdout, "\tNumber of Generations = %d\n\n", STOP_CONDITION);
    fprintf(stdout, "\tMutation Rate  = %.4lf\n", getMutationRate());
    fprintf(stdout, "\tCrossover Rate = %.4lf\n", getCrossoverRate());
    fprintf(stdout, "\tRuntime Weight = %.4lf\n", getRuntimeWeight());
    fprintf(stdout, "\tPower Weight   = %.4lf\n\n", 1.0 - getRuntimeWeight());
    
    if(crossover_type == 1)
        fprintf(stdout, "\tCrossover Algorithm   = One point Crossover\n");
    else
        fprintf(stdout, "\tCrossover Algorithm   = Two point Crossover\n");
    
    if(mutation_type == 1)
        fprintf(stdout, "\tMutation Algorithm    = Rotational Mutation\n");
    else
        fprintf(stdout, "\tMutation Algorithm    = Random Mutation\n");
    
    if(selection_type == 1)
        fprintf(stdout, "\tSelection Algorithm   = Tournament Selection\n");
    else
        fprintf(stdout, "\tSelection Algorithm   = Random Selection\n");
    
    if(replacement_type == 1)
        fprintf(stdout, "\tReplacement Algorithm = Replace All\n\n");
    else
        fprintf(stdout, "\tReplacement Algorithm = Keep Best\n\n");
    
    fprintf(stdout, "\tSeeding Ratio = %.4lf\n\n", (1.0 - PERCENT_POP_RANDOM));
}


void freeParameters(void){
    freeScheduler();
}

void setPopSize(int size){
    if(2 <= size && size <= 10000){
        POP_SIZE = size;
        return;
    }
    
    fprintf(stderr, "Invalid population size %d.\n", size);
    fprintf(stderr, "The population size must be an integer between 0 and 1\n");
    
    exit(1);
}

// COMMENTING TEMPLATE
/******************************************************************************
 * NAME : initRandGenerator
 * PURPOSE : 
 * ARGUMENTS : 
 * PRECONDITIONS : 
 * RETURNS : 
 * NOTE : 
 *****************************************************************************/
