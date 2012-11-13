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
int MIN_DEADLINE = 1;
int MAX_DEADLINE = 30;
int MAX_PERIOD = 200;

int systemUtilization(vector<Task> tasks)
{
	int current_utiliz = 0;
	for (unsigned int i = 0; i < tasks.size(); ++i)
	{
		current_utiliz += tasks[i].getUtilizationPercent();
	}
	return current_utiliz;
}

vector<Task> generateTasks(int utPerc, int numT, int precision = 0)
{
	if (utPerc > numT * 100)
		throw logic_error("generateTasks : Impossible to do a feasible system with the given parameters");

	// Generate numT tasks randomly

	vector<Task> tasks = vector<Task>();
	for (int i = 0; i < numT; ++i)
	{
		int offset = (int) (rand() % MAX_OFFSET);
		int deadline = (int) ((rand() % (MAX_DEADLINE - MIN_DEADLINE)) + MIN_DEADLINE);
		int period = (int) ((rand() % (MAX_PERIOD - deadline)) + deadline);
		int wcet = (deadline == 1) ? 1 : (int) ((rand() % (deadline - 1)) + 1);

		tasks.push_back(Task(offset, period, deadline, wcet));
	}

	for (unsigned int t = 0; t < tasks.size(); ++t)
	{
		cout << tasks[t].asString() << endl;
	}


	// modify all tasks to get closer to the utilization parameter
	// This will not work exactly because of discrete time (and because each task must respect Ui < 100%)

	float current_utiliz = systemUtilization(tasks);

	float utilizFactor = utPerc / current_utiliz;

	for (unsigned int i = 0; i < tasks.size(); ++i)
	{
		// modify wcet or deadline? --> random choice
		if (rand()%2 == 0)
		{
			int newWcet = (int)(tasks[i].getWcet() * utilizFactor);
			tasks[i].setWcet(max(newWcet, 1));
			// wcet must be < deadline and period
			if (tasks[i].getWcet() > tasks[i].getPeriod())
				tasks[i].setPeriod(tasks[i].getWcet());
			if (tasks[i].getWcet() > tasks[i].getDeadline())
				tasks[i].setDeadline(tasks[i].getWcet());
		}
		else
		{
			int newDeadline = (int) (tasks[i].getDeadline() * (1/utilizFactor));
			tasks[i].setDeadline(max(newDeadline, 1));
			if (tasks[i].getWcet() > tasks[i].getDeadline())
				tasks[i].setWcet(tasks[i].getDeadline());
		}
	}

	cout << "---------------------" << endl;

	for (unsigned int t = 0; t < tasks.size(); ++t)
	{
		cout << tasks[t].asString() << endl;
	}

	// Try to get closer and closer to the desired utilization by small modifications
	current_utiliz = systemUtilization(tasks);
	unsigned int loop_counter = 0;
	while(abs(current_utiliz - utPerc) > precision)
	{
		bool mustDecreaseUtil = (current_utiliz - utPerc > 0);

		int rndTaskP = rand() % tasks.size();
		Task* rndTask = &tasks[rndTaskP];

		// which small modification do we make? --> random choice
		int rollOfDice = rand() % 2;
		if (rollOfDice == 0) // change wcet
		{
			int oldWcet = rndTask->getWcet();
			// can we change wcet ?
			if ((mustDecreaseUtil and (oldWcet == 1))
				or ((not mustDecreaseUtil) and (
					(oldWcet == rndTask->getDeadline()) or (oldWcet == rndTask->getPeriod()))))
				continue;
			int newWcet = mustDecreaseUtil ? --oldWcet : ++oldWcet;
			rndTask->setWcet(newWcet);
		}
		else // change deadline
		{
			int oldDeadline = rndTask->getDeadline();
			// can we change deadline ?
			if ((mustDecreaseUtil and oldDeadline >= rndTask->getPeriod())
				or ((not mustDecreaseUtil) and oldDeadline == 0))
				continue;
			int newDeadline = mustDecreaseUtil ? ++oldDeadline : --oldDeadline;
			rndTask->setDeadline(newDeadline);
		}

		current_utiliz = systemUtilization(tasks);
		++loop_counter;
	}

	cout << "best utilization I could do : " << systemUtilization(tasks) << " (and not " << utPerc << ")" << endl;
	cout << "delta : " << abs(utPerc - systemUtilization(tasks)) << "\tprecision : " << precision << "\tloops : "<< loop_counter << endl;
	return tasks;
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	// default parameters
	int utilizationPercent = 100;
	int numberOfTasks = 10;
	string outputName = "output_default";
	
	// parse arguments
	for (int i = 1; i < argc; i += 2)
	{
		string command = argv[i];
		if (command == "-u") utilizationPercent = atoi(argv[i+1]);
		else if (command == "-n") numberOfTasks = atoi(argv[i+1]);
		else if (command == "-o") outputName = argv[i+1];
		else throw logic_error("parameter command : '" + command + "' not recognized");
	}

	vector<Task> tasks = generateTasks(utilizationPercent, numberOfTasks);

	ofstream outfile;
	outfile.open (outputName.c_str());

	for (unsigned int t = 0; t < tasks.size(); ++t)
	{
		outfile << tasks[t].asString() << endl;
	}

	outfile.close();

	return 0;
}