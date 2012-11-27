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

	static int MAX_OFFSET;
	static int ACCEPTED_PERIODS[19];
	static int ACCEPTED_PERIODS_size;

	taskGenerator(){};
	vector<Task> generateTasks(int utPerc, int numT, int precision);
	

private:
	int systemUtilization(vector<Task> tasks);

};

#endif
