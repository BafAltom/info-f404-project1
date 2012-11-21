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

int systemUtilization(vector<Task> tasks)
{
	int current_utiliz = 0;
	for (unsigned int i = 0; i < tasks.size(); ++i)
		current_utiliz += tasks[i].getUtilizationPercent();
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
		int period = (int) ((rand() % (MAX_PERIOD - MIN_PERIOD)) + MIN_PERIOD);
		int deadline = period;
		int wcet = (deadline == 1) ? 1 : (int) ((rand() % (deadline - 1)) + 1);

		tasks.push_back(Task(offset, period, deadline, wcet));
	}

	cout << "Initial generation :" << endl;
	for (unsigned int t = 0; t < tasks.size(); ++t)
		cout << tasks[t].asString() << endl;


	// modify all tasks to get closer to the utilization parameter
	// This will not work exactly because of discrete time (and because each task must respect Ui < 100%)

	float current_utiliz = systemUtilization(tasks);

	float utilizFactor = utPerc / current_utiliz;

	for (unsigned int i = 0; i < tasks.size(); ++i)
	{
		// modify wcet or period? --> random choice
		if (rand()%2 == 0)
		{
			int newWcet = (int)(tasks[i].getWcet() * utilizFactor);
			tasks[i].setWcet(max(newWcet, 1));

			// wcet must be < deadline and period
			if (tasks[i].getWcet() > tasks[i].getDeadline())
				tasks[i].setDeadline(tasks[i].getWcet());
			if (tasks[i].getWcet() > tasks[i].getPeriod())
				tasks[i].setPeriod(tasks[i].getWcet());
		}
		else
		{
			int newPeriod = (int) (tasks[i].getPeriod() * (1/utilizFactor));
			tasks[i].setPeriod(max(newPeriod, 1));

			if (tasks[i].getWcet() > tasks[i].getPeriod())
				tasks[i].setWcet(tasks[i].getPeriod());
		}
	}

	cout << "--------------------- after rescaling" << endl;

	for (unsigned int t = 0; t < tasks.size(); ++t)
		cout << tasks[t].asString() << "\t" << tasks[t].getUtilizationPercent() << endl;

	// Try to get closer and closer to the desired utilization by small modifications
	current_utiliz = systemUtilization(tasks);
	unsigned int loop_counter = 0;
	while(abs(current_utiliz - utPerc) > precision)
	{
		bool mustDecreaseUtil = (current_utiliz - utPerc > 0);

		int rndTaskP = rand() % tasks.size();
		Task* rndTask = &tasks[rndTaskP];

		// can we use this task?
		if (mustDecreaseUtil)
		{
			if (rndTask->getWcet() == 1)
				continue;
		}
		else
		{
			if (rndTask->getUtilizationPercent() == 100)
				continue;
		}

		// which small modification do we make? --> random choice
		int rollOfDice = rand() % 2;
		if (rollOfDice == 0) // change wcet
		{
			int oldWcet = rndTask->getWcet();
			int newWcet = mustDecreaseUtil ? --oldWcet : ++oldWcet;
			rndTask->setWcet(newWcet);
		}
		else // change period
		{
			int oldPeriod = rndTask->getPeriod();
			int newPeriod = mustDecreaseUtil ? ++oldPeriod : --oldPeriod;
			rndTask->setPeriod(newPeriod);
		}

		current_utiliz = systemUtilization(tasks);
		++loop_counter;

		cout << "--------------------- step " << loop_counter << endl;
		for (unsigned int t = 0; t < tasks.size(); ++t)
			cout << tasks[t].asString() << "\tu: " << tasks[t].getUtilizationPercent() << endl;
	}

	int delta = abs(utPerc - systemUtilization(tasks));

	cout << "best utilization I could do : " << systemUtilization(tasks);
	if (delta > 0)
		cout << " (and not " << utPerc << ")";
	cout << endl;
	cout << "delta : " << delta << "\tprecision : " << precision << "\tloops : "<< loop_counter << endl;
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