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

// GENERATORS CONSTANTS

int MAX_OFFSET = 20;
int MAX_PERIOD = 20;
int MIN_PERIOD = 2;
// useless (implicit deadline)
int MIN_DEADLINE = 1;
int MAX_DEADLINE = 20;



class taskGenerator {
	
public:
	
	taskGenerator(){};
	vector<Task> generateTasks(int utPerc, int numT, int precision);
	

private:
	int systemUtilization(vector<Task> tasks);

};

#endif


