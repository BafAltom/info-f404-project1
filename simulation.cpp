#include "simulation.hpp"

Simulation::Simulation() : t(0), deltaT(1), numberOfCPU(1), tasks(vector<Task>()), preemption_counter(0), migration_counter(0), number_of_core_used(0), idle_time_counter(0)
{	}

Simulation::Simulation(int nCPU, vector<Task> t) : t(0), deltaT(1), numberOfCPU(nCPU), tasks(t), preemption_counter(0), migration_counter(0), number_of_core_used(0), idle_time_counter(0)
{	}

void Simulation::run()
{
	int studyInterval = computeStudyInterval();
	while (t <= studyInterval)
	{
		t += deltaT;
		// TODO (a lot)...
	}
}

bool Simulation::result()
// TODO : return true if schedulable, false if not, and throw an exception if run() has not yet been called
{
	return false; 
}

string Simulation::report()
{
	std::ostringstream r;
	r << "Number of preemption = " << preemption_counter << endl
	 << "Number of migration = " << migration_counter << endl
	 << "Core used = " << number_of_core_used << endl
	 << "idle time  = " << idle_time_counter << endl;

	return r.str();

}