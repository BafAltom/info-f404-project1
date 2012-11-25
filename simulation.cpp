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
			_jobs.push_back(Job(&_tasks[i], t));
		}
	}
}

void Simulation::cleanAndCheckJobs(int t)
{
	// remove all jobs whose deadline is in the past (and check that they were completed successfully)
	if (_jobs.empty()) return;

	// save CPUs
	vector<Job> savedCPUs(_CPUs.size());
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
				cout << "FAILURE! at t = " << t << " and job : " << (*it) << endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				assert(findInDeque(&(*it), _CPUs) == -1);
				it = _jobs.erase(it);
			}
		}
		if (it == _jobs.end())
			break;
	}

	// restore CPU
	for (unsigned int i = 0; i < _CPUs.size(); ++i)
	{
		if (savedCPUs[i].getStartTime() != -1) // CPU was not NULL
		{	int posJob = findInDeque<Job>(savedCPUs[i], _jobs);
			assert(posJob != -1);
			_CPUs[i] = &_jobs[posJob];
		}
	}
}

/*
long ppcm2(long X, long Y)// from http://www.cppfrance.com/codes/PPCM-DEUX-NOMBRES-TOUT-COMPILATEUR_9638.aspx
{
	long A = X;
	long B = Y;
	while (A!=B)
	{
		while (A>B) B=B+Y;
		while (A<B) A=A+X;
	}
	return A;
}
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
		else if (B > 1 and d <= B and B % d == 0) // yes, else if
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

long Simulation::computeStudyInterval()
{
	// TODO : should return 2*LCM(periods) + max offset
	long current_ppcm = 1;
	for (deque<Task>::iterator it = _tasks.begin(); it != _tasks.end(); ++it)
	{
		current_ppcm = ppcm(current_ppcm, it->getPeriod());
		cout << "current_ppcm : " << current_ppcm << endl;
	}
	return (2*current_ppcm + maxOffsetOf(_tasks));
}

int Simulation::maxOffsetOf(std::deque<Task> tasks)
{
	int maxOffset = 0;
	for (deque<Task>::iterator it = tasks.begin(); it != tasks.end(); ++it)
	{
		maxOffset = max(maxOffset, (*it).getOffset());
	}
	return maxOffset;
}

priority_queue<Job*, std::vector<Job*>, EDFComp<false> > Simulation::getReadyJobs()
// !! does not return Running jobs
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
	/*if(readyJobs.size()>0) // teste ready job, a virer si fait du pure globale
	{
		priority_queue<Job*, std::vector<Job*>, EDFComp<false> > readyJobsTest;
		readyJobsTest = readyJobs;
		if(readyJobsTest.top()->getPriority())
		{
			Job* newJob = readyJobsTest.top();
			readyJobsTest.pop();
			assert(newJob->getPriority());
		}
		else if(readyJobsTest.size()>0)
		{
			for(unsigned int i=0; i< readyJobsTest.size() ; ++i)
			{
				Job* newJob = readyJobsTest.top();
				readyJobsTest.pop();
				assert(not newJob->getPriority());
			}
		}
		//cout<<"readyJobsTest :"<<readyJobsTest.size()<<"  sur :"<<readyJobs.size()<<endl;
	}*/
	//cout<<"------------------------------------------------------------------------------"<<endl;
	return readyJobs;
}

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

bool Simulation::isInCPUs(Job* j)
{
	return (findInDeque<Job*>(j, _CPUs) != -1);
}

int Simulation::positionOfFirstIdleCPU()
// return -1 if all CPUs are busy
{
	return findInDeque<Job*>(NULL, _CPUs);
}

int Simulation::findInCPUs(Job* j)
// return -1 if j is not in CPUs
{
	return findInDeque<Job*> (j, _CPUs);
}

bool Simulation::JobNeedToBePreempted()
{
	if(_readyJobs.top()->getPriority() and not _runningJobs.top()->getPriority())
	{
		return true;
	}	
	else if(_readyJobs.top()->getPriority() and _runningJobs.top()->getPriority())
	{
		cout<<"cela ne devrait jamais arriver"<<endl;
		return false;
	}
	else if(_runningJobs.top()->getPriority())
	{
		return false;
	}
	else
	{
		return _readyJobs.top()->getAbsoluteDeadline() < _runningJobs.top()->getAbsoluteDeadline();
	}
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
		//cout << endl << "t: " << _t << endl;

		/*
		cout << "initial CPUs" << endl;
		for (unsigned int i = 0; i < _CPUs.size(); ++i)
		{
			cout << "\tCPU[" << i << "]: ";
			if (_CPUs[i] != NULL)
				cout << _CPUs[i]->asString();
			cout << endl;
		}
		*/

		generateNewJobs(_t);
		if (_t % 10000 == 0) cout << "t > " << _t << "\t/\t" << studyInterval << endl;
		_readyJobs = getReadyJobs();
		_runningJobs = getRunningJobs();
		int firstIdleCPU = positionOfFirstIdleCPU(); // -1 if all CPUs 
		bool availableCPUs = (firstIdleCPU != -1);

		// scheduling : assign which jobs goes to which CPUs
		while (not _readyJobs.empty()
			and (availableCPUs or JobNeedToBePreempted() ))
				//or _readyJobs.top()->getAbsoluteDeadline() < _runningJobs.top()->getAbsoluteDeadline()))
		{
			// If we get in this loop, we know that the earliest-deadline active job should get a CPU
			assert(_readyJobs.top()->getComputationLeft() > 0);

			if (availableCPUs)
			{
				assert(_CPUs[firstIdleCPU] == NULL);
				Job* newJob = _readyJobs.top(); _readyJobs.pop();
				_CPUs[firstIdleCPU] = newJob;
				_runningJobs.push(newJob);
				//cout << "hi to\t" << newJob->asString() << "\tat CPU " << firstIdleCPU << endl;
			}
			else // all CPUs are busy, preempt the one with the latest deadline
			{
				if(_readyJobs.top()->getPriority()) // a virer si ok, juste des testes pour edfk
				{
					assert(_readyJobs.top()->getPriority());
					assert(not _runningJobs.top()->getPriority());
				}
				else
				{
					assert(not _runningJobs.top()->getPriority());
					assert(_runningJobs.top()->getAbsoluteDeadline() > _readyJobs.top()->getAbsoluteDeadline());
				}
				Job* oldJob = _runningJobs.top(); _runningJobs.pop();
				Job* newJob = _readyJobs.top(); _readyJobs.pop();
				int posOldJob = findInDeque<Job*>(oldJob, _CPUs);
				assert (posOldJob != -1);
				_CPUs[posOldJob] = newJob;
				_readyJobs.push(oldJob);
				_runningJobs.push(newJob);
				preemption_counter++;
				//cout << "bye to\t" << oldJob->asString() << "\tat CPU " << firstIdleCPU << endl;
				//cout << "hi to\t" << newJob->asString() << "\tat CPU " << firstIdleCPU << endl;
			}

			// update variables (duplicate code...)
			// + there should be a way to remove the refreshing of ready & running jobs
			_readyJobs = getReadyJobs();
			_runningJobs = getRunningJobs();
			firstIdleCPU = positionOfFirstIdleCPU(); // -1 if all CPUs 
			availableCPUs = (firstIdleCPU != -1);
		}

		// CPUs
		//cout << "CPUS" << endl;
		for (unsigned int i = 0; i < _CPUs.size(); ++i)
		{
			//cout << "\tCPU[" << i << "]: ";
			if (_CPUs[i] != NULL)
			{
				// cout << _CPUs[i]->asString();

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
					// cout << "\tbye!";
					_CPUs[i] = NULL;
				}
			else
				++idle_time_counter;
			}
			//cout << endl;
		}

		// advance time
		_t += _deltaT;
		cleanAndCheckJobs(_t);

	}

	cout << report() << endl;
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
	 << "Core used = " << _CPUs.size() << endl
	 << "idle time  = " << idle_time_counter << endl;

	return r.str();

}

template <class T>
int findInDeque (T t, deque<T> aDeque)
// return -1 if T is not in aDeque
{
	if (aDeque.empty())
		throw std::logic_error("findInDeque : Empty Deque !");

	for (unsigned int i = 0; i < aDeque.size(); ++i)
		if (aDeque[i] == t)
			return (int)i;
	return -1;
}

