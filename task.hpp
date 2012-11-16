#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

class Task
{
public:
	Task();
	Task(int offset, int period, int deadline, int wcet);
	Task(string parseString);
	static vector<Task> generateFromString(string tasks_text);

	int getUtilizationPercent();

	string asString();

	// getter-setters
	void setOffset(int newOffset);
	int getOffset();
	void setPeriod(int newPeriod);
	int getPeriod();
	void setDeadline(int newDeadline);
	int getDeadline();
	void setWcet(int newWcet);
	int getWcet();

private:
	int _offset;
	int _period;
	int _deadline;
	int _wcet;
};

ostream& operator << (ostream& s, Task t);

#endif