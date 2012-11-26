#include "simGlobalEDFClass.hpp"



simGlobalEDF::simGlobalEDF() :
	_numberCPU(1000),
	_tasks_generated(deque<Task>())
{	}

/**
* \details	Upload a set of tasks from a file
*/
void simGlobalEDF::uploadTask(char* file){
	
	ifstream taskFile(file);
	string tasks_text = "";
	while (!taskFile.eof())
	{
		// this does not check for input correctness
		string temp;
		for (int i = 0; i < 4; ++i)
		{
			taskFile >> temp;
			tasks_text += temp + "\t";
		}
		tasks_text += "\n";
		
	}
	_tasks_generated = Task::generateFromString(tasks_text);
}

/**
* \details	Compute the number of CPU needed by global EDF
*/
void simGlobalEDF::computeCPU(){
		int globalUtilizationPerc = 0;
		for (unsigned int i = 0; i < _tasks_generated.size(); ++i)
		{
			globalUtilizationPerc += _tasks_generated.at(i).getUtilizationPercent();
		}
		_numberCPU = ceil(globalUtilizationPerc/100.0);
}

/**
* \details	Run global EDF from a file and display the result on the terminal
*/
void simGlobalEDF::run(char* file){
	
	uploadTask(file);
	computeCPU();
	
	Simulation s(_numberCPU, _tasks_generated);
	vector<int> result = s.runGlobal();
	
	if(result.size()>0)
	{
		cout << "statistics of the simulation :"<<endl;
		cout << "Number of preemption = " << result.at(0) << endl;
		cout << "Number of migration = " << result.at(1) << endl;
		cout << "idle time  = " << result.at(2) << endl;
		cout << "Core used = " << _numberCPU << endl;
	}
	else
	{
		cout<<"This system is not schedulable"<<endl;
	}
	
}

/**
* \details	Run global EDF from a deque of tasks and return the result.
* \return 	A vector containing the statistics of the system
* 				vector[0]= average number of preemption
* 				vector[1]= average number of migration
*				vector[2]= average idle time
*				vector[3]= average number of core
*/
vector<int> simGlobalEDF::run(deque<Task> t)
{
	
	_tasks_generated = t;
	computeCPU();
	
	Simulation s(_numberCPU, _tasks_generated);
	vector<int> result = s.runGlobal();
	
	result.push_back(_numberCPU);
	
	return result;
	
}


