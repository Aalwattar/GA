/*
 * rcsSimulator.h
 *
 *  Created on: Jun 24, 2013
 *      Author: aalwatta
 */

#ifndef RCSSIMULATOR_H_
#define RCSSIMULATOR_H_

struct SimData {
	int dFGID;
	int noPRR;
	int noGPP;
	int noOfNodes;
	int typeData[500];

};

struct SimResults {
	unsigned int totalTime;
	unsigned int noOfReuse;
	unsigned int noOfConfiguration;
	unsigned int noSW2HWMigration;
	unsigned int noHW2SWMigration;
	unsigned int noOfSWTasks;
	unsigned int noSWBusyCounter;
	unsigned int noHWBusyCounter;
};






void initPRRsConfigTime(unsigned int * prrTime, int noPRRs);
int InitSimulator();
int CleanSimulator();
int RunSimulator(struct SimData *simData, struct SimResults *simResults);









#endif /* RCSSIMULATOR_H_ */
