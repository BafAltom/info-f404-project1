#include "globalEDFvsEDFk.hpp"

GlobalEDFvsEDFk::GlobalEDFvsEDFk(){	}


/**
* \details	Make and display a comparaison between EDF-k and global EDF for those parameters
*/
void GlobalEDFvsEDFk::makeStat(int numTasks, int utilisation, int numTesting)
{
	vector<int> statGlobalTot = vector<int>(4);
	vector<int> statEDFkTot = vector<int>(4);
	
	srand(time(NULL));
	taskGenerator task_generator;
	simGlobalEDF sim_global;
	simEDFk sim_edfk;
	
	int cnt=0;
	while(cnt <= numTesting)
	{
		cout << "cnt : " << cnt << " / " << numTesting << endl;
		// we generate the tasks
		vector<Task> tasks = task_generator.generateTasks(utilisation, numTasks, 20);
		deque<Task> t;
		std::copy(tasks.begin(),tasks.end(),std::back_inserter(t));
		
		// we run the system and store the statistics
		if(tasks.size()>0)
		{
			vector<int> statGlobal = sim_global.run(t);
			vector<int> statEDFk = sim_edfk.run(t);
			if(statEDFk.size() > 1 && statGlobal.size() > 1)
			{
				for(unsigned int j=0; j<statGlobal.size(); ++j)
				{
					statGlobalTot.at(j)=statGlobalTot.at(j)+statGlobal.at(j);
				}	
				for(unsigned int j=0; j<statEDFk.size(); ++j)
				{
					statEDFkTot.at(j)=statEDFkTot.at(j)+statEDFk.at(j);
				}
				cnt ++;
			}
		}
	}
	cout << "statistics of the simulation : \t"<<" Global EDF \t  | \t "<<"EDF-k"<<endl;
	cout << "--------------------------------------------------------------------"<<endl;
	cout << "Average number of preemption = \t \t" << (float)statGlobalTot.at(0)/numTesting<<"\t | \t" << (float)statEDFkTot.at(0)/numTesting << endl;
	cout << "Average number of migration = 	\t" << (float)statGlobalTot.at(1)/numTesting<<"\t | \t" << (float)statEDFkTot.at(1)/numTesting << endl;
	cout << "Average idle time  = \t		" << (float)statGlobalTot.at(2)/numTesting<<"\t | \t" << (float)statEDFkTot.at(2)/numTesting << endl;
	cout << "Average number of Core used =  \t \t" << (float)statGlobalTot.at(3)/numTesting<<"\t | \t" << (float)statEDFkTot.at(3)/numTesting << endl;

	
}

/**
* \details	Make and display comparaisons between EDF-k and global EDF for a utilisation from 30 to 160 and a number of task from 5 to 20
*/
void GlobalEDFvsEDFk::makeStats()
{
	srand(time(NULL));
	taskGenerator task_generator;
	simGlobalEDF sim_global;
	simEDFk sim_edfk;
	ofstream outfile;
	outfile.open ("statistics.txt");
	
	cout<<"The result are display with this form (stat for global EDF / stat for EDF-k)"<<endl;
	outfile <<"The result are display with this form (stat for global EDF / stat for EDF-k)"<<endl;
	cout<<"If the value is -1, it means that the generator has not happened to create systems with these settings"<<endl;
	outfile <<"If the value is -1, it means that the generator has not happened to create systems with these settings"<<endl;
	cout << "statistics of the simulation : \t"<<" \t 5 tasks \t | "<<" 10 tasks \t  |  "<<" 15 tasks \t  |  "<<" 20 tasks \t"<<endl;
	outfile << "statistics of the simulation : \t"<<" \t 5 tasks \t | "<<" 10 tasks \t  |  "<<" 15 tasks \t  |  "<<" 20 tasks \t"<<endl;
		
	for(int utilisation = 30; utilisation <= 160 ; utilisation+= 20)
	{
		cout<<"-----------------"<<endl<<"Utilisation of "<<utilisation<<endl;
		outfile<<"-----------------"<<endl<<"Utilisation of "<<utilisation<<endl;
		
		int NumberOftask = 5;
		vector < vector<float> > statGlobalFinale;
		vector < vector<float> > statEDFkFinale;
		for(int i=0; i <4 ; ++i)
		{
			int cnt = 0;
			float systSchedulable = 0;
			vector<float> statGlobalAverage = vector<float>(4);
			vector<float> statEDFkAverage = vector<float>(4);
			while(cnt < 50)
			{
				// we generate the tasks
				vector<Task> tasks = task_generator.generateTasks(utilisation, NumberOftask, 20);
				deque<Task> t;
				std::copy(tasks.begin(),tasks.end(),std::back_inserter(t));
				
				// we run the system and store the statistics
				if(tasks.size()>0)
				{
					vector<int> statGlobal = sim_global.run(t);
					vector<int> statEDFk = sim_edfk.run(t);
					if(statEDFk.size() > 1 && statGlobal.size() > 1)
					{
						for(unsigned int j=0; j<statGlobal.size(); ++j)
							statGlobalAverage.at(j)=(float)statGlobalAverage.at(j)+statGlobal.at(j);
					
						for(unsigned int j=0; j<statEDFk.size(); ++j)
							statEDFkAverage.at(j)=(float)statEDFkAverage.at(j)+statEDFk.at(j);
						
						systSchedulable++;
					}
				}
				cnt ++;
			}
			
			// we compute the average statistics
			for(unsigned int i = 0; i< statGlobalAverage.size(); ++i)
			{
				if(systSchedulable > 0.0)
				{
					statEDFkAverage.at(i)=statEDFkAverage.at(i)/systSchedulable;
					statGlobalAverage.at(i)=statGlobalAverage.at(i)/systSchedulable;
				}
				else
				{
					statEDFkAverage.at(i)=-1;
					statGlobalAverage.at(i)=-1;
				}
			}
			NumberOftask += 5;
			statGlobalFinale.push_back(statGlobalAverage);
			statEDFkFinale.push_back(statEDFkAverage);	
		}
		std::ostringstream s;
		s << "--------------------------------------------------------------------------------------------------------------------------------------------"<< "\n"
		 << "Average number of preemption = \t \t" <<"("<<statGlobalFinale.at(0).at(0)<<"/"<<statEDFkFinale.at(0).at(0)<<")"<<"\t \t | \t"  <<"("<<statGlobalFinale.at(1).at(0)<<"/"<<statEDFkFinale.at(1).at(0)<<")"<<"\t | \t"  <<"("<<statGlobalFinale.at(2).at(0)<<"/"<<statEDFkFinale.at(2).at(0)<<")"<<"\t | \t"  <<"("<<statGlobalFinale.at(3).at(0)<<"/"<<statEDFkFinale.at(3).at(0)<<")" << "\n"
		 << "Average number of migration = 	\t" <<"("<<statGlobalFinale.at(0).at(1)<<"/"<<statEDFkFinale.at(0).at(1)<<")"<<"\t \t | \t" <<"("<<statGlobalFinale.at(1).at(1)<<"/"<<statEDFkFinale.at(1).at(1)<<")"<<"\t | \t" <<"("<<statGlobalFinale.at(2).at(1)<<"/"<<statEDFkFinale.at(2).at(1)<<")"<<"\t | \t"  <<"("<<statGlobalFinale.at(3).at(1)<<"/"<<statEDFkFinale.at(3).at(1)<<")" << "\n"
		 << "Average idle time  = \t		" <<"("<<statGlobalFinale.at(0).at(2)<<"/"<<statEDFkFinale.at(0).at(2)<<")"<<"\t | \t" <<"("<<statGlobalFinale.at(1).at(2)<<"/"<<statEDFkFinale.at(1).at(2)<<")"<<"\t | \t"  <<"("<<statGlobalFinale.at(2).at(2)<<"/"<<statEDFkFinale.at(2).at(2)<<")"<<"\t | \t" <<"("<<statGlobalFinale.at(3).at(2)<<"/"<<statEDFkFinale.at(3).at(2)<<")" << "\n"
		 << "Average number of Core used =  \t \t" <<"("<<statGlobalFinale.at(0).at(3)<<"/"<<statEDFkFinale.at(0).at(3)<<")"<<"\t \t | \t" <<"("<<statGlobalFinale.at(1).at(3)<<"/"<<statEDFkFinale.at(1).at(3)<<")"<<"\t | \t"  <<"("<<statGlobalFinale.at(2).at(3)<<"/"<<statEDFkFinale.at(2).at(3)<<")"<<"\t | \t" <<"("<<statGlobalFinale.at(3).at(3)<<"/"<<statEDFkFinale.at(3).at(3)<<")" << "\n";
		cout << s.str();
		outfile << s.str();
	}
}


/**
* \details	Run globalEDFvsEDFk
* 				can be launched with :
* 					./globalEDFvsEDFk -u 120 -n 8 -t 100 (create the statistics for 8 tasks with an utilization of 100)
* 					./globalEDFvsEDFk (create statistics for a utilisation from 30 to 160 and a number of task from 5 to 20)
*/
int main(int argc, char** argv)
{
	
	int utilizationPercent = 100;
	int numTasks = 10;
	int numTesting = 100;
	GlobalEDFvsEDFk globalEDFvsEDFk;
	
	if (argc == 1)
	{
		globalEDFvsEDFk.makeStats();
	}
	else
	{
		if (argc % 2 == 0) throw "Bad number of parameters (was odd, need even)";

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
}
