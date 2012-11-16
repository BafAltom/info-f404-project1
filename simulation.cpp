#include "simulation.hpp"

Simulation::Simulation() :
	_t(0),
	_deltaT(1),
	_tasks(vector<Task>()),
	_jobs(vector<Job>()),
	_CPUs(vector<Job*>(1,NULL)),
	preemption_counter(0),
	migration_counter(0),
	number_of_core_used(0),
	idle_time_counter(0)
{	}

Simulation::Simulation(int nCPU, vector<Task> t) :
	_t(0),
	_deltaT(1),
	_tasks(t),
	_jobs(vector<Job>()),
	_CPUs(vector<Job*>(nCPU, NULL)),
	preemption_counter(0),
	migration_counter(0),
	number_of_core_used(0),
	idle_time_counter(0)
{	}

Simulation::~Simulation()
{	}

void Simulation::generateJobs(int studyInterval)
// Paul me dit que c'est mieux de seulement générer un job quand le précédent commence, mais bref.
{
	// clear current jobs list
	_jobs.clear();

	// generate new jobs
	for (unsigned int i = 0; i < _tasks.size(); ++i)
	{
		int t = _tasks[i].getOffset();
		while (t < studyInterval)
		{
			_jobs.push_back(Job(_tasks[i], t));
			t += _tasks[i].getPeriod();
		}
	}
}


int ppcm(int X,int Y)// from http://www.cppfrance.com/codes/PPCM-DEUX-NOMBRES-TOUT-COMPILATEUR_9638.aspx
{
	int A = X;
	int B = Y;
	while (A!=B)
	{
		while (A>B) B=B+Y;
		while (A<B) A=A+X;
	}
	return A;
}

long Simulation::computeStudyInterval()
{
	// TODO : should return 2*LCM(periods) + max offset
	int current_ppcm = 1;
	for (vector<Task>::iterator it = _tasks.begin(); it != _tasks.end(); ++it)
	{
		current_ppcm = ppcm(current_ppcm, it->getPeriod());
		cout << "current_ppcm : " << current_ppcm << endl;
	}
	return 2*current_ppcm;

}

vector<Job*> Simulation::getActiveJobs()
// !! does not return jobs currently computed by a CPU (which is good but not intuitive)
{
	vector<Job*> activeJobs;
	int currentTime = _t;
	for (unsigned int i = 0; i < _jobs.size(); ++i)
	{
		if (currentTime >= _jobs[i].getStartTime()
		and currentTime < _jobs[i].getAbsoluteDeadline()  // < deadline or <= deadline ???
		and not _jobs[i].getComputationLeft() == 0
		and not isInCPUs(&_jobs[i]))
		{
			activeJobs.push_back(&_jobs[i]);
		}
	}
	return activeJobs;
}

bool Simulation::isInCPUs(Job* j)
{
	if (j == NULL)
		throw std::logic_error("isInCPUs : j is NULL");

	for (unsigned int i = 0; i < _CPUs.size(); ++i)
		if (j == _CPUs[i])
			return true;
	return false;
}

Job* Simulation::getEarliestDeadline(vector<Job*> vJobs)
{
	// edge cases
	if (vJobs.empty())
		throw std::logic_error("getEarliestDeadline : vJobs is empty");
	if (vJobs.size() == 1)
		return vJobs[0];

	Job* min_deadline_job = vJobs[0];
	for (unsigned int i = 1; i < vJobs.size(); ++i)
	{
		int this_deadline = vJobs[i]->getAbsoluteDeadline();
		if (this_deadline < min_deadline_job->getAbsoluteDeadline())
			min_deadline_job = vJobs[i];
	}
	return min_deadline_job;
}

int Simulation::positionOfMaxDeadlineInCPUs()
// return -1 if all CPUs are idle
{
	if (_CPUs.empty())
		throw std::logic_error("positionOfMaxDeadlineInCPUs : No CPUs !");
	if (_CPUs.size() == 1)
		return 0;

	int pos_max_deadline = -1;
	int max_deadline = 0;
	for (unsigned int i = 1; i < _CPUs.size(); ++i)
	{
		if (_CPUs[i] == NULL) continue;

		int this_deadline = _CPUs[i]->getAbsoluteDeadline();
		if (this_deadline > max_deadline)
		{
			max_deadline = this_deadline;
			pos_max_deadline = i;
		}
	}
	return pos_max_deadline;
}

int Simulation::positionOfFirstIdleCPU()
// return -1 if all CPUs are busy
{
	if (_CPUs.empty())
		throw std::logic_error("positionOfFirstIdleCPU : No CPUs !");

	for (unsigned int i = 0; i < _CPUs.size(); ++i)
		if (_CPUs[i] == NULL)
			return (int)i;
	return -1;
}

// TODO:
// - active jobs must be organized in a min-deadline-heap
// - CPUs must be organized in a max-deadline-of-its-job-heap
// this will simplify the code a lot :)
void Simulation::runGlobal()
{
	long studyInterval = computeStudyInterval();
	generateJobs(studyInterval);
	_t = 0;
	cout << "study interval : " << studyInterval << endl;
	while (_t < studyInterval) // main loop
	{
		if (_t % 1000 == 0) cout << "t : " << _t << endl;
		vector<Job*> activeJobs = getActiveJobs();
		Job* earliestDeadlineJob = activeJobs.empty() ? NULL : getEarliestDeadline(activeJobs);
		int biggerDeadlineCPU = positionOfMaxDeadlineInCPUs(); // -1 if all CPUs are idle
		int firstIdleCPU = positionOfFirstIdleCPU(); // -1 if all CPUs are busy
		bool availableCPUs = (firstIdleCPU != -1);

		// scheduling : assign which jobs go to which CPUs
		while (not activeJobs.empty()
			and (availableCPUs
				or earliestDeadlineJob->getAbsoluteDeadline() < _CPUs[biggerDeadlineCPU]->getAbsoluteDeadline()))
		{
			// If we get in the loop, we know that the earliest-deadline active job should get a CPU

			if (availableCPUs)
			{
				assert(_CPUs[firstIdleCPU] == NULL);
				_CPUs[firstIdleCPU] = earliestDeadlineJob;
			}
			else // all CPUs are busy, preempt the one with the latest deadline
			{
				assert(_CPUs[biggerDeadlineCPU]->getAbsoluteDeadline() > earliestDeadlineJob->getAbsoluteDeadline());
				_CPUs[biggerDeadlineCPU] = earliestDeadlineJob;
				preemption_counter++;
			}

			// update variables (should be in a function, duplicate code = beaaaarg)
			activeJobs = getActiveJobs();
			earliestDeadlineJob = activeJobs.empty() ? NULL : getEarliestDeadline(activeJobs);
			biggerDeadlineCPU = positionOfMaxDeadlineInCPUs();
			firstIdleCPU = positionOfFirstIdleCPU();
			availableCPUs = (firstIdleCPU != -1);
		}

		// launch computation for this point in time
		for (unsigned int i = 0; i < _CPUs.size(); ++i)
		{
			if (_CPUs[i] != NULL)
				_CPUs[i]->giveCPU(1);
			/*those two 'if's must NOT be combined!!*/
			if (_CPUs[i] != NULL and _CPUs[i]->getComputationLeft() == 0)
				_CPUs[i] = NULL;
		}

		// advance time
		_t += _deltaT;
	}

	bool success = true;
	for (vector<Job>::iterator it = _jobs.begin(); it != _jobs.end(); it++)
	{
		if (it->getComputationLeft() != 0)
		{
			success = false;
			cout << "FAIL!!!! job : " << it->asString() << endl;
		}
	}
	if (success) cout << "SUCCESS!!" << endl;
}

bool Simulation::result()
// TODO : return true if schedulable, false if not, and throw an exception if run() has not yet been called
{
	return false; 
}

string Simulation::report()
{
	std::ostringstream r;
	r << "Number of preemption = " << preemption_counter << endl
	 << "Number of migration = " << migration_counter << endl
	 << "Core used = " << number_of_core_used << endl
	 << "idle time  = " << idle_time_counter << endl;

	return r.str();

}
