#include "job.hpp"

Job::Job() :
	_task (NULL),
	_startTime(-1),
	_computationLeft(-1),
	_lastCPU_id(-1)
{	}

Job::Job(Task* task, int currentTime) :
	_task(task),
	_startTime(currentTime),
	_computationLeft(task->getWcet()),
	_lastCPU_id(-1)
{
	if (_startTime % _task->getPeriod() != _task->getOffset() % _task->getPeriod())
	{
		throw std::logic_error("Job : given startTime does not correspond to the given task offset");
	}
}


/**
* \details	The job "run" on a CPU, so we decrease the computation left and store the "current" CPU of this job
*/
void Job::giveCPU(int duration, int CPU_id)
{
	_computationLeft -= duration;
	_lastCPU_id = CPU_id;
}

/****************************************
*	GETTERS/SETTERS
****************************************/

int Job::getAbsoluteDeadline()
{
	return getStartTime() + _task->getDeadline();
}

int Job::getComputationLeft()
{
	return _computationLeft;
}

int Job::getStartTime()
{
	return _startTime;
}

int Job::getLastCPU_Id()
{
	return _lastCPU_id;
}
bool Job::getPriority()
{
	return _task->getPriority();
}

Task* Job::getTask()
{
	return _task;
}

string Job::asString()
{
	std::ostringstream s;
	s << getStartTime() << "\t"
	  << getComputationLeft() << "\t";
	return s.str();
}

bool Job::operator==(const Job &other)
{
	return (_task == other._task and _startTime == other._startTime);
}

ostream& operator << (ostream& s, Job j)
{
	s << j.asString();
	return s;
}
