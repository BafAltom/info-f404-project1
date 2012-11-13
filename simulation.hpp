#include <iostream>
#include <vector>
#include "task.hpp"
using namespace std;

class Simulation
{
public:
	Simulation();
	Simulation(int nCPU, vector<Task> t);
	virtual ~Simulation() = 0;

	virtual int computeStudyInterval() = 0;

	void run();
	bool result();
	string report();

	// everything is public
	int t;
	int deltaT;
	int numberOfCPU;
	vector<Task> tasks;

	int preemption_counter;
	int migration_counter;
	int number_of_core_used;
	int idle_time_counter;
};