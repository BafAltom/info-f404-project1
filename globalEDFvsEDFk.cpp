#include "globalEDFvsEDFk.hpp"

GlobalEDFvsEDFk::GlobalEDFvsEDFk(){	}



void GlobalEDFvsEDFk::makeStat(int numTasks, int utilisation, int numTesting)
{
	vector<int> statGlobalTot = vector<int>(4);
	vector<int> statEDFkTot = vector<int>(4);
	
	/*taskGenerator task_generator;
	simGlobalEDF sim_global;
	simEDFk sim_edfk;
	
	for(int i=0; i < numTesting ; ++i)
	{
		vector<Task> tasks = task_generator.generateTasks(utilisation, numTasks, 0);
		
		deque<Task> t;
		std::copy(tasks.begin(),tasks.end(),t.begin());
		
		
		vector<int> statGlobal = sim_global.run(t);
		for(unsigned int j=0; j<statGlobal.size(); ++j)
		{
			statGlobalTot.at(j)=statGlobalTot.at(j)+statGlobal.at(j);
		}
		
		
		vector<int> statEDFk = sim_edfk.run(t);
		for(unsigned int j=0; j<statGlobal.size(); ++j)
		{
			statEDFkTot.at(j)=statEDFkTot.at(j)+statEDFk.at(j);
		}

		
	}*/
	
	// plus qu'à afficher les deux vecteurs/nombre de testes
	
}



// prend en argument : le nombre de tâches, l'utilisation et le nombre d'essais à faire.
// Exemple : ./globalEDFvsEDFk -u 120 -n 8 -t 100
int main(int argc, char** argv)
{
	
	int utilizationPercent = 100;
	int numTasks = 10;
	int numTesting = 100;
	GlobalEDFvsEDFk globalEDFvsEDFk;
	
	
	if (argc % 2 == 0) throw "Bad number of parameters (was odd, need even)";

	// parse arguments
	for (int i = 1; i < argc; i += 2)
	{
		string command = argv[i];
		if (command == "-u") utilizationPercent = atoi(argv[i+1]);
		else if (command == "-n") numTasks = atoi(argv[i+1]);
		else if (command == "-t") numTesting = atoi(argv[i+1]);
		else throw "parameter not recognized";
	}
	
	globalEDFvsEDFk.makeStat(numTasks, utilizationPercent, numTesting);
}
