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
		// a faire
		_numberCPU=3;
	
}


void simGlobalEDF::run(char* file){
	
	uploadTask(file);
	computeCPU();
	
	Simulation s(_numberCPU, _tasks_generated);
	s.runGlobal();
	
}

int main(int argc, char** argv)
{
	simGlobalEDF globalEDF;
	globalEDF.run(argv[1]);

	return 0;
}

