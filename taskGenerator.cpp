#include "taskGeneratorClass.hpp"

int main(int argc, char** argv)
{
	taskGenerator task_generator;
	srand(time(NULL));
	// default parameters
	int utilizationPercent = 100;
	int numberOfTasks = 10;
	string outputName = "output_default";
	int precision = 0;
	
	if (argc % 2 == 0) throw logic_error("Bad number of parameters (was odd, need even)");

	// parse arguments
	for (int i = 1; i < argc; i += 2)
	{
		string command = argv[i];
		if (command == "-u") utilizationPercent = atoi(argv[i+1]);
		else if (command == "-n") numberOfTasks = atoi(argv[i+1]);
		else if (command == "-o") outputName = argv[i+1];
		else if (command == "-p") precision = atoi(argv[i+1]);
		else throw logic_error("parameter command : '" + command + "' not recognized");
	}

	vector<Task> tasks;
	while(tasks.empty())
	{
		tasks = task_generator.generateTasks(utilizationPercent, numberOfTasks, precision);
	}

	ofstream outfile;
	outfile.open (outputName.c_str());

	for (unsigned int t = 0; t < tasks.size(); ++t)
	{
		outfile << tasks[t].asString() << endl;
	}

	outfile.close();

	return 0;
}
