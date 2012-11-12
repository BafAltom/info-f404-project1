#include "task.hpp"


Task::Task(int offset, int period, int deadline, int wcet) : _offset(offset), _period(period), _deadline(deadline), _wcet(wcet)
{	}

int Task::getUtilisation()
{
	return (float)(_wcet/_deadline);
}

string Task::asString()
{
	std::ostringstream s;
	s << "("
	  << getOffset() << ",\t"
	  << getPeriod() << ",\t"
	  << getDeadline() << ",\t"
	  << getWcet()
	  << ")";
	return s.str();
}

/****************************************
*	GETTERS/SETTERS
****************************************/

void Task::setOffset(int newOffset)
{
	if (newOffset < 0) throw std::logic_error("offset must be > 0");
	_offset = newOffset;
}

int Task::getOffset()
{
	return _offset;
}

void Task::setPeriod(int newPeriod)
{
	if (newPeriod < 0) throw std::logic_error("period must be > 0");
	_period = newPeriod;
}

int Task::getPeriod()
{
	return _period;
}

void Task::setDeadline(int newDeadline)
{
	if (newDeadline < 0) throw std::logic_error("deadline must be > 0");
	_deadline = newDeadline;
}

int Task::getDeadline()
{
	return _deadline;
}

void Task::setWcet(int newWcet)
{
	if (newWcet < 0) throw std::logic_error("wcet must be > 0");
	_wcet = newWcet;
}

int Task::getWcet()
{
	return _wcet;
}

ostream& operator << (ostream& s, Task t)
{
	s << t.asString();
	return s;
}