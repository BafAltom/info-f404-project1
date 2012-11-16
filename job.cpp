#include "job.hpp"

Job::Job() : _task (Task(0,0,0,0)), _startTime(0), _computationLeft(0)
{	}

Job::Job(Task task, int currentTime) : _task(task), _startTime(currentTime), _computationLeft(task.getWcet())
{
	if (_startTime % _task.getOffset() != 0)
		throw std::logic_error("Job : given startTime does not correspond to the given task offset");
}

void Job::giveCPU(int duration)
{
	_computationLeft -= duration;
}

int Job::getAbsoluteDeadline()
{
	return getStartTime() + _task.getDeadline();
}

int Job::getComputationLeft()
{
	return _computationLeft;
}

int Job::getStartTime()
{
	return _startTime;
}

string Job::asString()
{
	std::ostringstream s;
	s << _task.asString() << "\t"
	  << getStartTime() << "\t"
	  << getComputationLeft() << "\t";
	return s.str();
}



ostream& operator << (ostream& s, Job j)
{
	s << j.asString();
	return s;
}