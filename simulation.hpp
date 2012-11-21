#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <deque>
#include <set>
#include <algorithm>
#include <stdio.h>
#include <assert.h>
#include "task.hpp"
#include "job.hpp"
using namespace std;

class Simulation
{
public:
	Simulation();
	Simulation(int nCPU, deque<Task> t);
	virtual ~Simulation();

	virtual long computeStudyInterval();

	bool isInCPUs(Job* j);
	Job* getEarliestDeadline(deque<Job*> vJobs);
	int positionOfMaxDeadlineInCPUs();
	int positionOfFirstIdleCPU();
	void runGlobal();
	void generateJobs(int studyInterval);
	deque<Job*> getActiveJobs();
	void cleanJobs(int t);
	bool result();

	string report();

	// everything is public
	int _t;
	int _deltaT;
	deque<Task> _tasks;
	deque<Job> _jobs;
	deque<Job*> _CPUs;

	// not yet implemented
	int preemption_counter;
	int migration_counter;
	int number_of_core_used;
	int idle_time_counter;
};

#endif