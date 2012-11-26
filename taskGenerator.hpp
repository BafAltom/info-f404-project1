#ifndef TASKGENERATOR_H
#define TASKGENERATOR_H

#include <iostream>
#include <string>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <time.h>
using namespace std;

#include "task.hpp"

class taskGenerator {
	
public:
	// GENERATORS CONSTANTS

	static int MAX_OFFSET;
	static int ACCEPTED_PERIODS[19];
	static int ACCEPTED_PERIODS_size;

	taskGenerator(){};
	vector<Task> generateTasks(int utPerc, int numT, int precision);
	

private:
	int systemUtilization(vector<Task> tasks);

};

int taskGenerator::MAX_OFFSET = 20;
// values generated with http://www.dcode.fr/ppcm-plus-petit-commun-multiple
int taskGenerator::ACCEPTED_PERIODS[19] =
	{2, 3, 5, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 22, 24, 25, 28, 30, 32};
int taskGenerator::ACCEPTED_PERIODS_size = 19;
#endif


