#include "job.hpp"

Job::Job() :
	_task (NULL),
	_startTime(0),
	_computationLeft(0),
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

void Job::giveCPU(int duration, int CPU_id)
{
	_computationLeft -= duration;
	_lastCPU_id = CPU_id;
}

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

string Job::asString()
{
	std::ostringstream s;
	s << getStartTime() << "\t"
	  << getComputationLeft() << "\t";
	return s.str();
}



ostream& operator << (ostream& s, Job j)
{
	s << j.asString();
	return s;
}