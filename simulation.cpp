#include "simulation.hpp"

Simulation::Simulation() :
	_t(0),
	_deltaT(1),
	_tasks(),
	_jobs(),
	_CPUs(),
	_readyJobs(priority_queue<Job*, std::vector<Job*>, EDFComp<false> >()),
	_runningJobs(priority_queue<Job*, std::vector<Job*>, EDFComp<true> > ()),
	preemption_counter(0),
	migration_counter(0),
	number_of_core_used(0),
	idle_time_counter(0)
{	}

Simulation::Simulation(int nCPU, deque<Task> t) :
	_t(0),
	_deltaT(1),
	_tasks(t),
	_jobs(),
	_CPUs(nCPU),
	_readyJobs(),
	_runningJobs(),
	preemption_counter(0),
	migration_counter(0),
	number_of_core_used(0),
	idle_time_counter(0)
{	}

Simulation::~Simulation()
{	}

void Simulation::generateNewJobs(int t)
{
	// generate new jobs
	for (unsigned int i = 0; i < _tasks.size(); ++i)
	{
		Task* task = &_tasks[i];
		if (t % task->getPeriod() == task->getOffset())
		{
			_jobs.push_back(Job(_tasks[i], t));
		}
	}
}

void Simulation::cleanJobs(int t)
{
	// remove all jobs whose deadline is in the past (and check that they were completed successfully)
	// ugly code :)
	if (_jobs.empty()) return;
	for (deque<Job>::iterator it = _jobs.begin(); it != _jobs.end() and not _jobs.empty(); it++)
	{
		if (it->getAbsoluteDeadline() < t)
		{
			assert(it->getComputationLeft() == 0);
			it = _jobs.erase(it);
		}
		if (it != _jobs.end())
			break;
	}
}

long ppcm(int X,int Y)// from http://www.cppfrance.com/codes/PPCM-DEUX-NOMBRES-TOUT-COMPILATEUR_9638.aspx
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
	long current_ppcm = 1;
	for (deque<Task>::iterator it = _tasks.begin(); it != _tasks.end(); ++it)
	{
		current_ppcm = ppcm(current_ppcm, it->getPeriod());
		cout << "current_ppcm : " << current_ppcm << endl;
	}
	return 2*current_ppcm;

}

priority_queue<Job*, std::vector<Job*>, EDFComp<false> > Simulation::getReadyJobs()
// !! does not return running jobs
{
	priority_queue<Job*, std::vector<Job*>, EDFComp<false> > readyJobs;
	int currentTime = _t;
	for (unsigned int i = 0; i < _jobs.size(); ++i)
	{
		if (currentTime >= _jobs[i].getStartTime()
		and currentTime < _jobs[i].getAbsoluteDeadline()  // < deadline or <= deadline ???
		and _jobs[i].getComputationLeft() > 0
		and not isInCPUs(&_jobs[i]))
		{
			readyJobs.push(&_jobs[i]);
		}
	}
	return readyJobs;
}

priority_queue<Job*, std::vector<Job*>, EDFComp<true> > Simulation::getRunningJobs()
{
	priority_queue<Job*, std::vector<Job*>, EDFComp<true> > runningJobs;
	for (deque<Job*>::iterator it = _CPUs.begin(); it != _CPUs.end(); it++)
	{
		runningJobs.push(*it);
	}
	return runningJobs;
}

bool Simulation::isInCPUs(Job* j)
{
	return findInCPUs(j) != -1;
}

int Simulation::positionOfFirstIdleCPU()
// return -1 if all CPUs are busy
{
	return findInCPUs(NULL);
}

int Simulation::findInCPUs(Job* j)
// return -1 if j is not in CPUs
{
	if (_CPUs.empty())
		throw std::logic_error("findInCPUs : No CPUs !");

	for (unsigned int i = 0; i < _CPUs.size(); ++i)
		if (_CPUs[i] == j)
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

	_jobs.clear();

	_t = 0;
	cout << "study interval : " << studyInterval << endl;
	while (_t < studyInterval) // main loop
	{
		generateNewJobs(_t);
		cleanJobs(_t);
		if (_t % 10000 == 0) cout << "t > " << _t << "\t/\t" << studyInterval << endl;
		_readyJobs = getReadyJobs();
		_runningJobs = getRunningJobs();
		int firstIdleCPU = positionOfFirstIdleCPU(); // -1 if all CPUs 
		bool availableCPUs = (firstIdleCPU != -1);

		// scheduling : assign which jobs goes to which CPUs
		while (not _readyJobs.empty()
			and (availableCPUs
				or _readyJobs.top()->getAbsoluteDeadline() < _runningJobs.top()->getAbsoluteDeadline()))
		{
			// If we get in the loop, we know that the earliest-deadline active job should get a CPU

			if (availableCPUs)
			{
				assert(_CPUs[firstIdleCPU] == NULL);
				Job* newJob = _readyJobs.top();
				_CPUs[firstIdleCPU] = newJob;
				_runningJobs.push(newJob);
				_readyJobs.pop();
			}
			else // all CPUs are busy, preempt the one with the latest deadline
			{
				assert(_runningJobs.top()->getAbsoluteDeadline() > _readyJobs.top()->getAbsoluteDeadline());
				Job* oldJob = _runningJobs.top();
				Job* newJob = _readyJobs.top();
				int posOldJob = findInCPUs(oldJob);
				assert (posOldJob != -1);
				_CPUs[posOldJob] = newJob;
				_runningJobs.pop(); //oldJob
				_readyJobs.pop(); // newJob
				_readyJobs.push(oldJob);
				_runningJobs.push(newJob);
				preemption_counter++;
			}

			// update variables (should be in a function, duplicate code = beaaaarg)
			// + there should be a way to remove the refreshing of ready & running jobs
			_readyJobs = getReadyJobs();
			_runningJobs = getRunningJobs();
			firstIdleCPU = positionOfFirstIdleCPU(); // -1 if all CPUs 
			availableCPUs = (firstIdleCPU != -1);
		}

		// CPUs
		for (unsigned int i = 0; i < _CPUs.size(); ++i)
		{
			// compute jobs in CPUs
			if (_CPUs[i] != NULL)
			{
				if (_CPUs[i]->getLastCPU_Id() != -1 and _CPUs[i]->getLastCPU_Id() != (int) i)
				{
					++migration_counter;
				}
				_CPUs[i]->giveCPU(_deltaT, i);
			}
			// check if a job is done
			if (_CPUs[i] != NULL and _CPUs[i]->getComputationLeft() == 0)
				_CPUs[i] = NULL;
		}

		// advance time
		_t += _deltaT;
	}

	bool success = true;
	for (deque<Job>::iterator it = _jobs.begin(); it != _jobs.end(); it++)
	{
		if (it->getComputationLeft() != 0 and it->getAbsoluteDeadline() < studyInterval)
		{
			success = false;
			cout << "FAIL!!!! job : " << it->asString() << endl;
		}
	}
	if (success) 
	{
		cout << "SUCCESS!!" << endl;
		cout << "preemption : " << preemption_counter << endl << "migration : " << migration_counter << endl;
	}
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
