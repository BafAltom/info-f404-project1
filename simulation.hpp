#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <algorithm>
#include <deque>
#include <queue>
#include <set>
#include <algorithm>
#include <stdio.h>
#include <assert.h>
#include "EDFComp.hpp"
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
	int positionOfFirstIdleCPU();
	int findInCPUs(Job* j);
	void runGlobal();
	void generateNewJobs(int studyInterval);
	priority_queue<Job*, std::vector<Job*>, EDFComp<false> > getReadyJobs();
	priority_queue<Job*, std::vector<Job*>, EDFComp<true> > getRunningJobs();
	void cleanJobs(int t);
	bool result();

	string report();

	// everything is public
	int _t;
	int _deltaT;
	deque<Task> _tasks;
	deque<Job> _jobs;
	deque<Job*> _CPUs;
	priority_queue<Job*, std::vector<Job*>, EDFComp<false> > _readyJobs;
	priority_queue<Job*, std::vector<Job*>, EDFComp<true> > _runningJobs;

	int preemption_counter;
	int migration_counter;
	// not yet implemented
	int number_of_core_used;
	int idle_time_counter;
};

#endif