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

vector<Task> generateTasks(int utPerc, int numT)
{
	// Generate numT tasks randomly

	vector<Task> tasks = vector<Task>();
	for (int i = 0; i < numT; ++i)
	{
		int offset = (int) (rand() % MAX_OFFSET);
		int deadline = (int) ((rand() % (MAX_DEADLINE - MIN_DEADLINE)) + MIN_DEADLINE);
		int period = (int) ((rand() % (MAX_PERIOD - deadline)) + deadline);
		int wcet = (deadline > 1) ? (int) ((rand() % (deadline - 1)) + 1) : 1;
		tasks.push_back(Task(offset, period, deadline, wcet));
	}

	// modify tasks to fit the utilization parameter

	float current_utiliz = systemUtilization(tasks);

	cout << "current utilization : " << current_utiliz << endl;
	fflush(stdout);

	float utilizFactor = utPerc / current_utiliz;

	cout << "utilizFactor " << utPerc << " / " << current_utiliz << " = " << utilizFactor << endl;
	fflush(stdout);

	for (unsigned int i = 0; i < tasks.size(); ++i)
	{
		tasks[i].setWcet((int) (tasks[i].getWcet() * utilizFactor));
	}

	cout << "utilization : " << systemUtilization(tasks) << endl;
	fflush(stdout);
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