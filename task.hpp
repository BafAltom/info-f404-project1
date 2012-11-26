#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <deque>
using namespace std;

class Task
{
public:
	Task();
	Task(int offset, int period, int deadline, int wcet);
	Task(int offset, int period, int deadline, int wcet, bool priority);
	Task(string parseString);
	static deque<Task> generateFromString(string tasks_text);

	int getUtilizationPercent();

	string asString(bool verbose = false);

	// getter-setters
	void setOffset(int newOffset);
	int getOffset();
	void setPeriod(int newPeriod);
	int getPeriod();
	void setDeadline(int newDeadline);
	int getDeadline();
	void setWcet(int newWcet);
	int getWcet();
	void setUtilisation(float newUtilisation);
	void reComputeUtilisation();
	float getUtilisation();
	void setPriority(bool newPriority);
	bool getPriority();

private:
	int _offset;
	int _period;
	int _deadline;
	int _wcet;
	float _utilisation;
	bool _priority;
};

ostream& operator << (ostream& s, Task t);

#endif
