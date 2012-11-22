#ifndef SIMEDFK_H
#define SIMEDFK_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;

#include "task.hpp"
#include "simulation.hpp"

class simEDFk {
	
public:
	
	simEDFk();
	void run(char* file);
	

private:
	int _k;
	int _numberCPU;
	deque<Task> _initialTasks;
	
	
	void uploadTask(char* file);
	void computeCPUandK();
	void modifyPriority();

};

#endif
