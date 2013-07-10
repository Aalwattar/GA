/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : June 17, 2013
 * Modified : July 10, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : config.h
 * 
 * Purpose  : a configuration file (to contain all unchanging information)
 ******************************************************************************/


#ifndef CONFIG_H
#define	CONFIG_H


// locations of input files
#define DEFAULT_ARCH_FILENAME "input/arch.conf"
#define DEFAULT_DFG_FILENAME  "input/B1_10_5.conf"
#define DEFAULT_PRR_FILENAME  "input/prr.conf"


// FIX - normalize power + runtime to make them 75:25 instead
// Fitness weights
#define DEFAULT_RUNTIME_WEIGHT 0.875

// Genetic operator rates
#define DEFAULT_CROSSOVER_RATE 0.85
#define DEFAULT_MUTATION_RATE  0.005

// misc
#define BUFF_SIZE 500

// AHMED'S STUFF
#define MAX_NUM_TYPES 10
#define MAX_NUM_GENES 500
#define PERCENT_POP_RANDOM 0.75
#define ACCEPTABLE_DEVIATION_ERROR 0

#endif	/* CONFIG_H */

