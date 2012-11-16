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
	Job(Task task, int currentTime);

	void giveCPU(int duration);

	int getAbsoluteDeadline();
	int getComputationLeft();
	int getStartTime();

	string asString();

private:
	Task _task;
	int _startTime;
	int _computationLeft;
};

ostream& operator << (ostream& s, Job j);

#endif