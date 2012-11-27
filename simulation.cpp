#include "simulation.hpp"

Simulation::Simulation() :
	_t(0),
	_deltaT(1),
	_tasks(),
	_jobs(),
	_CPUs(),
	_readyJobs(),
	_runningJobs(),
	preemption_counter(0),
	migration_counter(0),
	number_of_core_used(0),
	number_of_core_necessary(0),
	idle_time_counter(0)
{	}

Simulation::Simulation(int nCPU, deque<Task> t) :
	_t(0),
	_deltaT(1),
	_tasks(t),
	_jobs(),
	_CPUs(nCPU, NULL),
	_readyJobs(),
	_runningJobs(),
	preemption_counter(0),
	migration_counter(0),
	number_of_core_used(0),
	number_of_core_necessary(0),
	idle_time_counter(0)
{	}

Simulation::~Simulation()
{	}


/**
* \details	Generate the new jobs that appears at the time t
*/
void Simulation::generateNewJobs(int t)
{
	for (unsigned int i = 0; i < _tasks.size(); ++i)
	{
		Task* task = &_tasks[i];
		if (t % task->getPeriod() == task->getOffset())
		{
			_jobs.push_back(Job(&_tasks[i], t));
		}
	}
}

/**
* \details	Remove all jobs whose deadline is in the past and check that they were completed successfully.
* \return 	true if all jobs whose deadline is in the past are completed successfully, false otherwise.
*/
bool Simulation::cleanAndCheckJobs(int t)
{
	if (_jobs.empty()) return true;

	// save CPUs
	deque<Job> savedCPUs(_CPUs.size());
	for (unsigned int i = 0; i < _CPUs.size(); ++i)
	{
		if (_CPUs[i] != NULL)
		{
			savedCPUs[i] = (*_CPUs[i]);
		}
	}

	for (deque<Job>::iterator it = _jobs.begin(); it != _jobs.end() and not _jobs.empty(); it++)
	{
		if (it->getAbsoluteDeadline() < t)
		{
			if (it->getComputationLeft() != 0)
			{
				return false;
			}
			else
			{
				assert(findInDeque((*it), savedCPUs) == -1);
				it = _jobs.erase(it);
			}
		}
		if (it == _jobs.end())
			break;
	}

	// restore CPU
	for (unsigned int i = 0; i < _CPUs.size(); ++i)
	{
		if (savedCPUs[i].getStartTime() != -1)
		{	int posJob = findInDeque<Job>(savedCPUs[i], _jobs);
			assert(posJob != -1);
			_CPUs[i] = &_jobs[posJob];
		}
	}
	
	return true;
}

/**
* \details	Compute the lowest common multiple
* \return 	The lowest common multiple
*/
long ppcm(long A, long B)
{
	if (A == 1) return B;
	if (B == 1) return A;
	if (A == B) return A;

	deque<long> divisorsA;
	deque<long> divisorsB;
	for (long d = 2; d <= max(A, B); ++d)
	{
		if (A > 1 and d <= A and A % d == 0)
		{
			divisorsA.push_back(d);
			A /= d;
			--d;
		}
		else if (B > 1 and d <= B and B % d == 0)
		{
			divisorsB.push_back(d);
			B /= d;
			--d;
		}
	}
	if (divisorsA.empty()) divisorsA.push_back(A);
	if (divisorsB.empty()) divisorsB.push_back(B);

	long current_ppcm = 1;
	while(not divisorsA.empty())
	{
		long divisor = divisorsA[0];
		divisorsA.erase(divisorsA.begin());
		current_ppcm *= divisor;

		if (not divisorsB.empty())
		{
			int positionB = findInDeque<long> (divisor, divisorsB);
			if (positionB != -1) divisorsB.erase(divisorsB.begin() + positionB);
		}
	}
	while (not divisorsB.empty())
	{
		current_ppcm *= (*divisorsB.begin());
		divisorsB.erase(divisorsB.begin());
	}

	return current_ppcm;
}

/**
* \details	Compute the study interval
* \return 	The study interval
*/
long Simulation::computeStudyInterval()
{
	long current_ppcm = 1;
	for (deque<Task>::iterator it = _tasks.begin(); it != _tasks.end(); ++it)
	{
		current_ppcm = ppcm(current_ppcm, it->getPeriod());
	}
	return (2*current_ppcm + maxOffsetOf(_tasks));
}

/**
* \details	Compute the largest offset from a deque of tasks
* \return 	this offset
*/
int Simulation::maxOffsetOf(std::deque<Task> tasks)
{
	int maxOffset = 0;
	for (deque<Task>::iterator it = tasks.begin(); it != tasks.end(); ++it)
	{
		maxOffset = max(maxOffset, (*it).getOffset());
	}
	return maxOffset;
}

/**
* \details	Compute the jobs that are actives but not running on the CPU
* \return 	priority_queue of thoses jobs (sort by deadline)
*/
priority_queue<Job*, std::vector<Job*>, EDFComp<false> > Simulation::getReadyJobs()
{
	priority_queue<Job*, std::vector<Job*>, EDFComp<false> > readyJobs;
	int currentTime = _t;
	for (unsigned int i = 0; i < _jobs.size(); ++i)
	{	
		if (currentTime >= _jobs[i].getStartTime()
		and currentTime < _jobs[i].getAbsoluteDeadline() 
		and _jobs[i].getComputationLeft() > 0
		and not isInCPUs(&_jobs[i]))
		{
			readyJobs.push(&_jobs[i]);
		}
	}
	return readyJobs;
}

/**
* \details	Compute the jobs that are running on the CPU
* \return 	priority_queue of thoses jobs (sort by deadline)
*/
priority_queue<Job*, std::vector<Job*>, EDFComp<true> > Simulation::getRunningJobs()
{
	priority_queue<Job*, std::vector<Job*>, EDFComp<true> > runningJobs;
	for (deque<Job*>::iterator it = _CPUs.begin(); it != _CPUs.end(); it++)
	{
		if (*it != NULL)
		{
			runningJobs.push(*it);
		}
	}
	return runningJobs;
}

/**
* \details	Check if a job is running on a CPU
* \return 	true if this job is running on a CPU, false otherwise
*/
bool Simulation::isInCPUs(Job* j)
{
	return (findInDeque<Job*>(j, _CPUs) != -1);
}

/**
* \details	Compute the position of the first idle CPU
* \return 	This position or -1 if all CPUs are busy
*/
int Simulation::positionOfFirstIdleCPU()
{
	return findInDeque<Job*>(NULL, _CPUs);
}

/**
* \details	Compute the position of the CPU where the job j is running
* \return 	This position or -1 if j is not in CPUs
*/
int Simulation::findInCPUs(Job* j)
{
	return findInDeque<Job*> (j, _CPUs);
}

/**
* \details	Check if a ready job need to be preempted with a job running on a CPU
* \return 	True if a ready job need to be preempted, false otherwise
*/
bool Simulation::JobNeedToBePreempted()
{
	if(_readyJobs.top()->getPriority() and not _runningJobs.top()->getPriority())
	{
		return true;
	}
	else if(_runningJobs.top()->getPriority() and not _readyJobs.top()->getPriority())
	{
		return false;
	}
	else // pure Global EDF
	{
		return _readyJobs.top()->getAbsoluteDeadline() < _runningJobs.top()->getAbsoluteDeadline();
	}
}


/**
* \details	Run global EDF (adapted to "pure" global EDF and to EDF-k).
* \return 	A vector containing the statistics of the system
* 				vector[0]= average number of preemption
* 				vector[1]= average number of migration
*				vector[2]= average idle time
* 				vector[3]= number_of_core_necessary
*				vector[4]= studyInterval
*/
vector<int> Simulation::runGlobal()  
{
	long studyInterval = computeStudyInterval();
	bool isSchedulable = true;

	_jobs.clear();

	_t = 0;

	while (_t < studyInterval && isSchedulable) // main loop
	{
		if (DEBUG)
		{
			cout << endl << "t: " << _t << endl;

			cout << "initial CPUs" << endl;
			for (unsigned int i = 0; i < _CPUs.size(); ++i)
			{
				cout << "\tCPU[" << i << "]: ";
				if (_CPUs[i] != NULL)
					cout << _CPUs[i]->asString();
				cout << endl;
			}
		}

		generateNewJobs(_t);
		_readyJobs = getReadyJobs();
		_runningJobs = getRunningJobs();
		int firstIdleCPU = positionOfFirstIdleCPU();
		bool availableCPUs = (firstIdleCPU != -1);

		// scheduling : assign which jobs goes to which CPUs
		while (not _readyJobs.empty() and (availableCPUs or JobNeedToBePreempted() ))
		{
			// If we get in this loop, we know that the earliest-deadline active job should get a CPU
			assert(_readyJobs.top()->getComputationLeft() > 0);

			if (availableCPUs)
			{
				assert(_CPUs[firstIdleCPU] == NULL);
				Job* newJob = _readyJobs.top(); _readyJobs.pop();
				_CPUs[firstIdleCPU] = newJob;
				_runningJobs.push(newJob);
				if (DEBUG) cout << "hi to\t" << newJob->asString() << "\tat CPU " << firstIdleCPU << endl;
			}
			else // all CPUs are busy, preempt the one with the latest deadline
			{
				Job* oldJob = _runningJobs.top(); _runningJobs.pop();
				Job* newJob = _readyJobs.top(); _readyJobs.pop();
				int posOldJob = findInDeque<Job*>(oldJob, _CPUs);
				assert (posOldJob != -1);
				_CPUs[posOldJob] = newJob;
				_readyJobs.push(oldJob);
				_runningJobs.push(newJob);
				preemption_counter++;
				if (DEBUG) cout << "bye to\t" << oldJob->asString() << "\tat CPU " << firstIdleCPU << endl;
				if (DEBUG) cout << "hi to\t" << newJob->asString() << "\tat CPU " << firstIdleCPU << endl;
			}

			_readyJobs = getReadyJobs();
			_runningJobs = getRunningJobs();
			firstIdleCPU = positionOfFirstIdleCPU();
			availableCPUs = (firstIdleCPU != -1);
		}

		// CPUs
		if (DEBUG) cout << "CPUS" << endl;
		int idle_cpus_count = 0;
		for (unsigned int i = 0; i < _CPUs.size(); ++i)
		{
			if (DEBUG) cout << "\tCPU[" << i << "]: ";
			if (_CPUs[i] != NULL)
			{
				if (DEBUG) cout << _CPUs[i]->asString();

				// verifications
				assert(findInDeque(_CPUs[i], _CPUs) == (int)i);
				assert(_CPUs[i]->getComputationLeft() > 0);

				// compute jobs in CPUs
				if (_CPUs[i]->getLastCPU_Id() != -1 and _CPUs[i]->getLastCPU_Id() != (int) i)
					++migration_counter;
				_CPUs[i]->giveCPU(_deltaT, i);

				// check if a job is done
				if (_CPUs[i]->getComputationLeft() == 0)
				{
					if (DEBUG) cout << "\tbye!";
					_CPUs[i] = NULL;
				}
			}
			else
			{
				++idle_cpus_count;
				++idle_time_counter;
			}
			if (DEBUG) cout << endl;
		}
		if ((int)_CPUs.size() - idle_cpus_count > number_of_core_necessary)
		{
			number_of_core_necessary = _CPUs.size() - idle_cpus_count;
		}

		// advance time
		_t += _deltaT;
		isSchedulable = cleanAndCheckJobs(_t);

	}

	vector<int> result;
	if(isSchedulable)
	{	
		result.push_back(preemption_counter);
		result.push_back(migration_counter);
		result.push_back(idle_time_counter);
		result.push_back(number_of_core_necessary);
		result.push_back(studyInterval);
	}
	return result;
}


template <class T>
int findInDeque (T t, deque<T> aDeque)
// return -1 if T is not in aDeque
{
	if (aDeque.empty())
		throw std::logic_error("findInDeque : Empty Deque !");

	for (unsigned int i = 0; i < aDeque.size(); ++i)
		if (aDeque[i] == t)
		{
			return (int)i;
		}
	return -1;
}

