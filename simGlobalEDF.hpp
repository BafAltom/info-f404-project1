#ifndef SIMGLOBALEDF_H
#define SIMGLOBALEDF_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <deque>
#include <queue>
using namespace std;

#include "task.hpp"
#include "job.hpp"
#include "simulation.hpp"
#include "EDFComp.hpp"

class simGlobalEDF {
	
public:
	
	simGlobalEDF();
	void run(char* file);
	

private:
	int _numberCPU;
	deque<Task> _tasks_generated;
	
	
	void uploadTask(char* file);
	void computeCPU();

};

#endif

