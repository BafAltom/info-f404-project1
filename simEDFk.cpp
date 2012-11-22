#include "simEDFk.hpp"

bool taskSortCriteria (Task t1,Task t2) { return (t1.getUtilisation() > t2.getUtilisation()); }


simEDFk::simEDFk() :
	_k(0),
	_numberCPU(1000),
	_initialTasks(deque<Task>())
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
	//trie les taches par utilisation
	sort(_initialTasks.begin(),_initialTasks.end(), taskSortCriteria);
	
	/*cout<<"after sort------------------------------"<<endl;
	for(unsigned int i=0; i< _initialTasks.size(); ++i)
	{
		cout<<"tache "<<i<<" : "<<endl<<_initialTasks.at(i).asString()<<endl;
	}*/
	
	
	std::vector<int> testNumberCPU;
	
	// On calcule les min CPU
	//cout<<"------------------------------------------------------"<<endl;
	for(unsigned int k = 1; k <= _initialTasks.size(); ++k)
	{
		//cout<<"k = "<<k<<endl;
		float Utot = 0;
		for(unsigned int i=k; i < _initialTasks.size(); ++i)
		{
			Utot += _initialTasks.at(i).getUtilisation() ;
			//cout<<"_initialTasks.at(i).getUtilisation() = "<<_initialTasks.at(i).getUtilisation()<<endl;
		}
		//cout<<"Utot = "<<Utot<<endl;
		int minCPU = (k-1) + ceil(Utot/(1-_initialTasks.at(k-1).getUtilisation()));
		//cout<<"minCPU = "<<minCPU<<endl;
		testNumberCPU.push_back(minCPU);
		//cout<<"------------------------------------------------------"<<endl;
	}
	
	for(unsigned int i = 0; i< testNumberCPU.size(); ++i)
	{
		if(testNumberCPU.at(i) < _numberCPU)
		{
			_numberCPU = testNumberCPU.at(i);
			_k = i+1;
		}
	}
	cout<<"_numberCPU min ="<<_numberCPU<<endl;
	cout<<"_k min ="<<_k<<endl;
	
}


void simEDFk::modifyPriority(){
	
	
}

void simEDFk::run(char* file)
{
	uploadTask(file);
	computeCPUandK();
}

int main(int argc, char** argv)
{
	simEDFk edfk;
	edfk.run(argv[1]);

	//Simulation s(3, tasks_generated);
	//s.runGlobal();

	return 0;
}
