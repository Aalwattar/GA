/*
 * rcsSimulator.h
 *
 *  Created on: Jun 24, 2013
 *      Author: aalwatta
 */

#ifndef RCSSIMULATOR_H_
#define RCSSIMULATOR_H_

#include "common_interfaces.h"

#define PRINT_DFG_DATA 0x01
#define INIT_FLAGS(flag) ((flag)= 0x0)
#define SET_FLAG_VALUE(x,flag)  ((x)|=(flag))
#define IS_FLAG_TRUE(x,flag) ((x)&(flag))
struct SimData
{
	unsigned int flags;
	int noOfNodes;
	int typeData[500];

};

struct SimResults
{
	unsigned int totalTime;
	unsigned int noOfReuse;
	unsigned int noOfConfiguration;
	unsigned int noSW2HWMigration;
	unsigned int noHW2SWMigration;
	unsigned int noOfSWTasks;
	unsigned int noSWBusyCounter;
	unsigned int noHWBusyCounter;
        unsigned int power;
};



void initPRRsConfigTime(unsigned int * prrTime, int noPRRs);
int InitSimulator(Common_Interface *);
int CleanSimulator();
int RunSimulator(struct SimData *simData, struct SimResults *simResults);




#endif /* RCSSIMULATOR_H_ */
