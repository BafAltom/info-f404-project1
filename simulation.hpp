#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <assert.h>
#include "task.hpp"
#include "job.hpp"
using namespace std;

class Simulation
{
public:
	Simulation();
	Simulation(int nCPU, vector<Task> t);
	virtual ~Simulation() = 0;

	virtual int computeStudyInterval();

	bool isInCPUs(Job* j);
	Job* getEarliestDeadline(vector<Job*> vJobs);
	int positionOfMaxDeadlineInCPUs();
	int positionOfFirstIdleCPU();
	void runGlobal();
	void generateJobs(int studyInterval);
	vector<Job*> getActiveJobs();
	bool result();

	string report();

	// everything is public
	int _t;
	int _deltaT;
	vector<Task> _tasks;
	vector<Job> _jobs;
	vector<Job*> _CPUs;

	// not yet implemented
	int preemption_counter;
	int migration_counter;
	int number_of_core_used;
	int idle_time_counter;
};

#endif