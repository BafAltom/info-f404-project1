#include "simEDFkClass.hpp"

bool taskSortCriteria (Task t1,Task t2) { return (t1.getUtilisation() > t2.getUtilisation()); }


simEDFk::simEDFk() :
	_k(0),
	_numberCPU(0),
	_initialTasks(deque<Task>())
{	}

/**
* \details	Upload a set of tasks from a file
*/
void simEDFk::uploadTask(char* file){
	
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
	_initialTasks = Task::generateFromString(tasks_text);	
	
}

/**
* \details	Compute the number of CPU and the k, needed by EDF-k
*/
void simEDFk::computeCPUandK(){
	
	//sort tasks by utilisation
	sort(_initialTasks.begin(),_initialTasks.end(), taskSortCriteria);
	

	std::vector<int> testNumberCPU;
	
	
	// we compute minCPU
	for(unsigned int k = 1; k <= _initialTasks.size(); ++k)
	{
		float Utot = 0;
		int minCPU = 0;
		for(unsigned int i=k; i < _initialTasks.size(); ++i)
		{
			Utot += _initialTasks.at(i).getUtilisation() ;
		}
		if(_initialTasks.at(k-1).getUtilisation()!=1)
		{
			minCPU = (k-1) + ceil(Utot/(1-_initialTasks.at(k-1).getUtilisation()));
		}
		
		testNumberCPU.push_back(minCPU);
	}

	// we initialise tne number of CPU and k before we can search for the min.
	unsigned int cnt=0;
	while(_numberCPU==0 && cnt < testNumberCPU.size())
	{
		if(testNumberCPU.at(cnt) != 0)
		{
			_numberCPU = testNumberCPU.at(cnt);
			_k = cnt+1;
		}
		cnt ++;
	}
	
	// we search the minimal number of CPU and k
	if(_numberCPU==0)
	{
		// if every tasks have an utilisation of 1, then we need a core for every tasks
		_numberCPU = _initialTasks.size();
	}
	else
	{
		// we search for the minimal number of CPU
		for(unsigned int i = 0; i< testNumberCPU.size(); ++i)
		{
			if(testNumberCPU.at(i) < _numberCPU && testNumberCPU.at(i) != 0)
			{
				_numberCPU = testNumberCPU.at(i);
				_k = i+1;
			}
		}
	}
	if(_numberCPU < _k)
	{
		// We can't have less core than priority tasks
		_numberCPU = _k;
	}

	
}

/**
* \details	Modify the priority of the k first tasks
*/
void simEDFk::modifyPriority(){
	for(int i=0; i<_k; ++i)
	{
		_initialTasks.at(i).setPriority(true);
	}
	
}


/**
* \details	Run EDF-k from a file and display the result on the terminal
*/
void simEDFk::run(char* file)
{
	uploadTask(file);
	computeCPUandK();
	modifyPriority();
	
	Simulation s(_numberCPU, _initialTasks);
	vector<int> result = s.runGlobal();
	
	if(result.size()>0)
	{
		cout << "statistics of the simulation :"<<endl;
		cout << "Number of preemption = " << result.at(0) << endl;
		cout << "Number of migration = " << result.at(1) << endl;
		cout << "idle time  = " << result.at(2) << endl;
		cout << "studyInterval  = " << result.at(3) << endl;
		cout << "Core used = " << _numberCPU << endl;
	}
	else
	{
		cout<<"This system is not schedulable"<<endl;
	}
}


/**
* \details	Run EDF-k from a deque of tasks and return the result.
* \return 	A vector containing the statistics of the system
* 				vector[0]= average number of preemption
* 				vector[1]= average number of migration
*				vector[2]= average idle time
*				vector[3]= studyInterval
*				vector[4]= average number of core
*/
vector<int> simEDFk::run(deque<Task> t)
{
	_initialTasks = t;
	computeCPUandK();
	modifyPriority();
	
	Simulation s(_numberCPU, _initialTasks);
	vector<int> result = s.runGlobal();
	
	result.push_back(_numberCPU);
	
	return result;
}
