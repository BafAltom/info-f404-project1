#include "simEDFk.hpp"

simEDFk::simEDFk() :
	_k(0),
	_numberCPU(1000),
	_initialTasks(vector<Task>())
{	}

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
	

	cout << "generated " << _initialTasks.size() << " tasks." << endl;
	for(unsigned int i=0; i< _initialTasks.size(); ++i)
	{
		cout<<"tache "<<i<<" : "<<endl<<_initialTasks.at(i).asString()<<endl;
	}
	
	
}


void simEDFk::computeCPUandK(){
	//trie les taches par utilisation (heap ?)
	
	//vector<int> first;
	std::vector<int> testNumberCPU;
	
	// On calcule les min CPU
	for(unsigned int k = 0; k < _initialTasks.size(); ++k)
	{
		int minCPU = 0; // taper la formule
		testNumberCPU.push_back(minCPU);
	}
	
	for(unsigned int i = 0; i< testNumberCPU.size(); ++i)
	{
		if(testNumberCPU.at(i) < _numberCPU)
		{
			_numberCPU = testNumberCPU.at(i);
			_k = i;
		}
	}
	
}


void simEDFk::modifyPriority(){
	
	
}

int main(int argc, char** argv)
{
	simEDFk edfk;
	edfk.uploadTask(argv[1]);

	//Simulation s(3, tasks_generated);
	//s.runGlobal();

	return 0;
}
