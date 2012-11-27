#include "taskGeneratorClass.hpp"

int taskGenerator::MAX_OFFSET = 20;
// values generated with http://www.dcode.fr/ppcm-plus-petit-commun-multiple
int taskGenerator::ACCEPTED_PERIODS[19] =
	{2, 3, 5, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 22, 24, 25, 28, 30, 32};
int taskGenerator::ACCEPTED_PERIODS_size = 19;

/**
* \details	Compute the current utilisation of the system
* \return 	the current utilisation of the system
*/
int taskGenerator::systemUtilization(vector<Task> tasks)
{
	int current_utiliz = 0;
	for (unsigned int i = 0; i < tasks.size(); ++i)
		current_utiliz += tasks[i].getUtilizationPercent();
	return current_utiliz;
}

/**
* \details	Generate a set of tasks
* \return 	A vector containing thoses tasks
*/
vector<Task> taskGenerator::generateTasks(int utPerc, int numT, int precision)
{
	if (utPerc > numT * 100)
		throw logic_error("generateTasks : Impossible to do a feasible system with the given parameters");

	// Generate numT tasks randomly

	vector<Task> tasks = vector<Task>(numT);
	for (int i = 0; i < numT; ++i)
	{
		int offset = (int) (rand() % taskGenerator::MAX_OFFSET);
		int period_pos = (int) (rand() % taskGenerator::ACCEPTED_PERIODS_size);
		int period = taskGenerator::ACCEPTED_PERIODS[period_pos];
		int deadline = period;
		int wcet = (deadline == 1) ? 1 : (int) ((rand() % (deadline - 1)) + 1);

		tasks[i] = Task(offset, period, deadline, wcet);
	}

	/*cout << "Initial generation :" << endl;
	for (unsigned int t = 0; t < tasks.size(); ++t)
		cout << tasks[t].asString() << endl;*/


	// modify all tasks to get closer to the utilization parameter
	// This will not work exactly because of discrete time (and because each task must respect Ui < 100%)

	float current_utiliz = systemUtilization(tasks);

	float utilizFactor = utPerc / current_utiliz;

	for (unsigned int i = 0; i < tasks.size(); ++i)
	{
		int newWcet = (int)(tasks[i].getWcet() * utilizFactor);
		tasks[i].setWcet(max(newWcet, 1));

		if (tasks[i].getWcet() > tasks[i].getPeriod())
			tasks[i].setPeriod(tasks[i].getWcet());
	}

	//cout << "--------------------- after rescaling" << endl;

	/*for (unsigned int t = 0; t < tasks.size(); ++t)
		cout << tasks[t].asString() << "\t" << tasks[t].getUtilizationPercent() << endl;*/

	// Try to get closer and closer to the desired utilization by small modifications
	current_utiliz = systemUtilization(tasks);
	unsigned int loop_counter = 0;
	while(abs(current_utiliz - utPerc) > precision)
	{
		bool mustDecreaseUtil = (current_utiliz - utPerc > 0);

		// check that we are not stuck
		bool stuck = true;
		for (unsigned int i = 0; i < tasks.size(); ++i)
		{
			if (mustDecreaseUtil)
				stuck = (stuck and tasks.at(i).getWcet() == 1);
			else
				stuck = (stuck and tasks.at(i).getWcet() == tasks.at(i).getPeriod());
		}
		if (stuck)
		{
			//cout << "Dead end. Try again." << endl;
			return (vector<Task>());
		}

		int rndTaskP = rand() % tasks.size();
		Task* rndTask = &tasks[rndTaskP];

		if (rndTask->getWcet() == 1 or rndTask->getWcet() == rndTask->getPeriod()) continue;

		int oldWcet = rndTask->getWcet();
		int newWcet = mustDecreaseUtil ? --oldWcet : ++oldWcet;
		rndTask->setWcet(newWcet);

		current_utiliz = systemUtilization(tasks);
		++loop_counter;

		/*cout << "--------------------- step " << loop_counter << endl;
		for (unsigned int t = 0; t < tasks.size(); ++t)
			cout << tasks[t].asString() << "\tu: " << tasks[t].getUtilizationPercent() << endl;
		cout << "Global Utilization : " << systemUtilization(tasks) << endl;*/
	}

	// explicit deadline
	for (unsigned int i =0 ; i < tasks.size(); ++i)
	{
		tasks[i].setDeadline(tasks[i].getPeriod());
		tasks[i].reComputeUtilisation();
	}

	/*int delta = abs(utPerc - systemUtilization(tasks));

	cout << "best utilization I could do : " << systemUtilization(tasks);
	if (delta > 0)
		cout << " (and not " << utPerc << ")";
	cout << endl;
	cout << "delta : " << delta << "\tprecision : " << precision << "\tloops : "<< loop_counter << endl;*/
	return tasks;
}
