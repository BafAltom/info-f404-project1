#include <iostream>
#include <string>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
using namespace std;

#include "task.hpp"

vector<Task> generateTasks(int utPerc, int numT)
// for now this is really stupid and deterministic
// TODO : add randomness
{
	vector<Task> tasks = vector<Task>();
	for (int i = 0; i < numT; ++i)
	{
		int offset = 0;
		int period = 150;
		int deadline = 100;
		int wcet = utPerc;
		tasks.push_back(Task(offset, period, deadline, wcet));
	}
	return tasks;
}

int main(int argc, char** argv)
{
	// default parameters
	int utilizationPercent = 100;
	int numberOfTasks = 10;
	string outputName = "defaultOutput";
	
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