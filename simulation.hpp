#ifndef SIMULATION_H
#define SIMULATION_H

#define DEBUG false

#include <algorithm>
#include <assert.h>
#include <deque>
#include <iostream>
#include <math.h>
#include <queue>
#include <set>
#include <stdio.h>
#include <vector>
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

	int maxOffsetOf(std::deque<Task> jobs);
	bool isInCPUs(Job* j);
	int positionOfFirstIdleCPU();
	int findInCPUs(Job* j);
	vector<int> runGlobal();
	void generateNewJobs(int studyInterval);
	priority_queue<Job*, std::vector<Job*>, EDFComp<false> > getReadyJobs();
	priority_queue<Job*, std::vector<Job*>, EDFComp<true> > getRunningJobs();
	void cleanAndCheckJobs(int t);
	bool result();
	bool JobNeedToBePreempted();

	//string report();

	// everything is public
	int _t;
	int _deltaT;
	deque<Task> _tasks;
	deque<Job> _jobs;
	deque<Job*> _CPUs;
	priority_queue<Job*, std::vector<Job*>, EDFComp<false> > _readyJobs;
	priority_queue<Job*, std::vector<Job*>, EDFComp<true> > _runningJobs;

	long preemption_counter; // an unfinished job leaves a CPU
	long migration_counter; // a job which previously was on a CPU enters another CPU
	int number_of_core_used;
	int idle_time_counter; // sum of the idle time count of each CPU (!= time steps where ALL CPUs were idle)
};

template<class T> int findInDeque(T t, deque<T> aDeque);

#endif
