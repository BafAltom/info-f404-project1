#ifndef JOB_H
#define JOB_H

#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

#include "task.hpp"
using namespace std;

class Job
{
public:
	Job();
	Job(Task* task, int currentTime);

	void giveCPU(int duration, int CPU_id);

	int getAbsoluteDeadline();
	int getComputationLeft();
	int getStartTime();
	int getLastCPU_Id();
	bool getPriority();
	Task* getTask(); // a vire après ?????

	string asString();

	bool operator==(const Job &other);

private:
	Task* _task;
	int _startTime;
	int _computationLeft;
	int _lastCPU_id;
};

ostream& operator << (ostream& s, Job j);

#endif
