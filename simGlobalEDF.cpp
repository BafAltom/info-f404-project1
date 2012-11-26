#include "simGlobalEDF.hpp"



simGlobalEDF::simGlobalEDF() :
	_numberCPU(1000),
	_tasks_generated(deque<Task>())
{	}

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

	cout << "generated " << _tasks_generated.size() << " tasks." << endl;
	for (unsigned int i = 0; i < _tasks_generated.size(); ++i)
		cout << _tasks_generated[i].asString(true) << endl;
	
}

void simGlobalEDF::computeCPU(){
		int globalUtilizationPerc = 0;
		for (unsigned int i = 0; i < _tasks_generated.size(); ++i)
		{
			globalUtilizationPerc += _tasks_generated.at(i).getUtilizationPercent();
		}

		cout << "computeCPUS : " << globalUtilizationPerc << endl;
		_numberCPU = ceil(globalUtilizationPerc/100.0);
}


void simGlobalEDF::run(char* file){
	
	uploadTask(file);
	computeCPU();
	
	Simulation s(_numberCPU, _tasks_generated);
	vector<int> result = s.runGlobal();
	
	cout << "statistics of the simulation :"<<endl;
	cout << "Number of preemption = " << result.at(0) << endl;
	cout << "Number of migration = " << result.at(1) << endl;
	cout << "idle time  = " << result.at(2) << endl;
	cout << "Core used = " << _numberCPU << endl;
	
}

vector<int> simGlobalEDF::run(deque<Task> t)
{
	
	_tasks_generated = t;
	computeCPU();
	
	Simulation s(_numberCPU, _tasks_generated);
	vector<int> result = s.runGlobal();
	
	result.push_back(_numberCPU);
	
	return result;
	
}

int main(int argc, char** argv)
{
	simGlobalEDF globalEDF;
	globalEDF.run(argv[1]);

	return 0;
}

