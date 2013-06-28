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
#include "config.h"
#include "fitness.h"
#include "selection.h"
#include "replacement.h"

#include "rcsSimulator.h"
#include "offlineScheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>

// FIX
static int STOP_CONDITION = 150;
static int POP_SIZE = 0;
static t_task * task; 

// FIX 
int crossover_type = 1;
int mutation_type = 1;
int replacement_type = 1;
int selection_type = 1;


void initParameters(int, char **);
void freeParameters(void);
void setPopSize(int);

// FUTURE - implement this
bool populationConverged(Population * pop);

///*   SCHEDULER TESTING MAIN  */
//int main(int argc, char * argv[]){
//    time_t start,end;               // timing stuff
//
//    Individual ind;
//    char chromosome[] = "212151115412111111122151112151211211111113151151213121112111111155111151151111111115112121112211151511111113113112311131121112212111111111151111111122";
//    int i;
//    
//    initParameters(argc, argv);
//    
//    ind.encoding = malloc(sizeof(int) * getNumGenes());
//    for(i=0; i<getNumGenes(); i++)
//        ind.encoding[i] = chromosome[i] - '1';
//    
//    fprintf(stdout, "Starting Napoleon\n");
//    start = time(NULL);
//    evaluateFitnessNapoleon(&ind);
//    end = time(NULL);
//    printIndividual(&ind);
//    printf("Napoleon used %f seconds.\n\n", difftime(end, start));
//    
//    fprintf(stdout, "Starting Napoleon\n");
//    start = time(NULL);
//    evaluateFitness(&ind);
//    end = time(NULL);
//    printIndividual(&ind);
//    printf("RCScheduler used %f seconds.\n\n", difftime(end, start));
//    
//    freeParameters();
//    return EXIT_SUCCESS;
//}

/* REGULAR MAIN */
// a generational keep-best approach
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
    // FIX
    free(task);
    freeParameters();
    return EXIT_SUCCESS;
} 

// FIX - too long
// FIX - print out every parameter selected
void initParameters(int argc, char ** argv){
    char * arch_filename = DEFAULT_ARCH_FILENAME;
    char * aif_filename = DEFAULT_AIF_FILENAME;
    int seed = randSeed();
    int c;

    opterr = 0;

    while((c = getopt(argc, argv, "a:b:c:d:g:m:n:p:r:s:t:Vvw:")) != -1){
        switch(c){
            case 'a':
                arch_filename = optarg;
                break;
            case 'b':
                if(strncasecmp("two-point", optarg, strlen(optarg))  == 0 || 
                    strncasecmp("two_point", optarg, strlen(optarg)) == 0 ||
                    strncasecmp("two point", optarg, strlen(optarg)) == 0 )
                    crossover_type = 2;
                else if(strncasecmp("one-point", optarg, strlen(optarg)) != 0 &&
                        strncasecmp("one_point", optarg, strlen(optarg)) != 0 &&
                        strncasecmp("one point", optarg, strlen(optarg)) != 0 ){
                    fprintf(stderr, "Unknown crossover option : %s\n", optarg);
                    exit(1);
                }
                break;
            case 'c':
                setCrossoverRate(atof(optarg));
                break;
            case 'd':
                aif_filename = optarg;
                break;
            case 'g':
                STOP_CONDITION = atoi(optarg);
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
                fprintf(stdout, "Offline Scheduler version X.0.0  (Compare the runtimes and results of Napoleon and the Online scheduler on the same chromosome)\n");
                fprintf(stdout, "Please see https://github.com/Aalwattar/OfflineScheduler for more information\n");
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
    
    if(initArchLibrary(arch_filename) != true)
        exit(1);
    
//    // FIX
    initNapoleon(aif_filename);
    InitSimulator();
    
    
    if(POP_SIZE == 0){
        POP_SIZE = getNumGenes();
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
}


void freeParameters(void){
    freeArchLibrary();

    // FIX
	CleanSimulator();
    freeNapoleon();
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
