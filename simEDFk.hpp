#ifndef SIMEDFK_H
#define SIMEDFK_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
using namespace std;

#include "task.hpp"
#include "simulation.hpp"

class simEDFk {

private:
	int _k;
	int _numberCPU;
	std::vector<Task> _initialTasks;
			
	
public:
	
	simEDFk();
	void uploadTask(char* file);
	void computeCPUandK();
	void modifyPriority();


};

#endif
