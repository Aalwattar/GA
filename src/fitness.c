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
 * Filename : fitness.c
 * 
 * Purpose  : A library for integration with Ziad's Napoleon
 * 
 *            Napoleon will act as our fitness function, taking a mapping of 
 *              architectures to each task as input, and returning the runtime
 *              and power required for that schedule as a fitness function
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#include "fitness.h"
#include "config.h"

#include "rcsSimulator.h"
#include "offlineScheduler.h"

#include "common_interfaces.h"
#include "architecture_library.h"
#include "dfg_library.h"
#include "hardware_library.h"

// FIX 
static double RUNTIME_WEIGHT = DEFAULT_RUNTIME_WEIGHT;

static Architecture_Library arch_library;
static DFG dfg;
static Hardware hardware;


/******************************************************************************
 *****************            ARCHITECTURE FILE I/O           *****************
 *****************************************************************************/

int getNumArch(int task){
    return arch_library.task[task].num_impl;
}

/******************************************************************************
 **********        FITNESS FUNCTION (INTERFACE WITH NAPOLEON)        **********
 *****************************************************************************/


void setRuntimeWeight(double weight){
    if(0 <= weight && weight <= 1){
        RUNTIME_WEIGHT = weight;
        return;
    }
    
    fprintf(stderr, "Invalid runtime weight %.3lf.\n", weight);
    fprintf(stderr, "The runtime rate must be a decimal number between 0 and 1\n");
    exit(1);
}

double getRuntimeWeight(void){
    return RUNTIME_WEIGHT;
}


bool initScheduler(char * arch_filename, char * dfg_filename, char * prr_filename){
    if(initArchLibrary(arch_filename, &arch_library) != EXIT_SUCCESS)
        exit(EXIT_FAILURE);
    
    if(initDFG(dfg_filename, &dfg) != EXIT_SUCCESS)
        exit(EXIT_FAILURE);
    
    if(initHardwareLibrary(prr_filename, &hardware) != EXIT_SUCCESS)
        exit(EXIT_FAILURE);
    
    
    // FIX - all rcScheduler's stuff
    InitSimulator();
    
    // FIX - all Napoleon's stuff
    initNapoleon(arch_filename, dfg_filename);
    
    
    // FIX - check the return value from both above funtions?
    // FIX - Make which function is compiled into a command line arg???
    
    // FIX - change verbose / exe flags into command line arguments with function pointers
    //          eg. if the pointer is null, don't print anything. Else use the printing function provided each generation????
    
    return true;
}


void freeScheduler(void){
    // FIX - all rcScheduler's stuff
    CleanSimulator();
    // FIX - Napoleon's stuff
    freeNapoleon();
    
    freeHardwareLibrary(&hardware);
    freeArchLibrary(&arch_library);
    freeDFG(&dfg);
    // FIX - Needs a header comment

// FIX - Needs a header comment
    

}

//// FUTURE - consider passing in an integer array and returning one value as
////          fitness (return int, take integer array)
//void evaluateFitness(Individual * ind){
//    struct SimData input;
//    struct SimResults output;
//    int i;
//    
//    // Zero the output
//    output.totalTime = 0;
//	output.noOfReuse = 0;
//	output.noOfConfiguration = 0;
//	output.noSW2HWMigration = 0;
//	output.noHW2SWMigration = 0;
//	output.noOfSWTasks = 0;
//	output.noSWBusyCounter = 0;
//	output.noHWBusyCounter = 0;
//
//    //initialize the input
//    input.dFGID = 4;    // IMPORTANT FIX - make this set from initParameters (currently B1_10_5.aif)
//    input.noPRR = 5;    // FIXME - make a variable
//    input.noGPP = 0;    // FIX - make a variable
//    input.noOfNodes = getNumGenes();
//    for(i=0; i<input.noOfNodes; i++)
//        input.typeData[i] = ind->encoding[i];// FIX - ASSUMES THAT HIS ARRAY STARTS AT 0
//
//    
//	RunSimulator(&input, &output);
//    
//    ind->fitness = output.totalTime;
//    ind->energy = 0;    
//    ind->exec_time = output.totalTime;
//    ind->num_reuse = 0;
//    ind->prefetch = 0;
//}

// FIX - add error checkingThis has
void evaluateFitness(Individual * ind){
    struct SimData input;
    struct SimResults output;
    int i;

    input.noOfNodes = getNumGenes();
    for(i=0; i<input.noOfNodes; i++){
        input.typeData[i] = ind->encoding[i];
    }
    
    // FIX - check return value
    getSchedule(&input, &output);

    ind->fitness = (output.totalTime * RUNTIME_WEIGHT) + (output.power * (1.0 - RUNTIME_WEIGHT));
    ind->energy = output.power;
    ind->exec_time = output.totalTime;
    ind->num_reuse = output.noOfReuse;
    ind->prefetch = output.noOfConfiguration;
}


int getTaskType(int task_num){
    // this function interfaces the GA's indices (which all start at 0) with Napoleon (starts at 1)
    return dfg.node[task_num].task_type - 1;
}


int getNumGenes(void){
    return dfg.num_nodes;
}