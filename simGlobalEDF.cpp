#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <deque>
using namespace std;

#include "task.hpp"
#include "simulation.hpp"

int main(int argc, char** argv)
{
	ifstream taskFile(argv[1]);
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
	deque<Task> tasks_generated = Task::generateFromString(tasks_text);

	cout << "generated " << tasks_generated.size() << " tasks." << endl;

	Simulation s(3, tasks_generated);
	s.runGlobal();

	return 0;
}
