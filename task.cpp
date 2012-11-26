#include "task.hpp"

Task::Task() : _offset(0), _period(0), _deadline(0), _wcet(0), _utilisation(0), _priority(false)
{	}

Task::Task(int offset, int period, int deadline, int wcet) : _offset(offset), _period(period), _deadline(deadline), _wcet(wcet), _utilisation((float)wcet/period), _priority(false)
{	}

Task::Task(int offset, int period, int deadline, int wcet, bool priority) : _offset(offset), _period(period), _deadline(deadline), _wcet(wcet), _utilisation((float)wcet/period), _priority(priority)
{	}

Task::Task(string parseString) : _offset(0), _period(0), _deadline(0), _wcet(0), _utilisation(0), _priority(false)
{
// This function does not check for correctness of input
	istringstream ss(parseString);
	string parsedStrings[5]; // 4 parameters + endline
	int counter = 0;
	while (!ss.eof())
	{
		string x;
		getline(ss, x,'\t');
		parsedStrings[counter++] =  x;
	}


	_offset = atoi(parsedStrings[0].c_str());
	_period = atoi(parsedStrings[1].c_str());
	_deadline = atoi(parsedStrings[2].c_str());
	_wcet = atoi(parsedStrings[3].c_str());
	
	_utilisation = (float)_wcet / _period;
	

}

deque<Task> Task::generateFromString(string tasks_text)
{
	deque<Task> tasks;
	istringstream ss(tasks_text);
	while (!ss.eof())
	{
		string one_task_text;
		getline(ss, one_task_text,'\n');
		if (one_task_text.size() > 4) // empty lines
			tasks.push_back(Task(one_task_text));
	}
	return tasks;
}

int Task::getUtilizationPercent()
{
	return (_wcet*100/_period);
}

string Task::asString(bool verbose)
{
	std::ostringstream s;
	s << getOffset() << "\t"
	  << getPeriod() << "\t"
	  << getDeadline() << "\t"
	  << getWcet();
	if (verbose)
	{
		s << "\t|\t" << getUtilisation()<< "\t"
		<< getPriority();
	}
	return s.str();
}

/****************************************
*	GETTERS/SETTERS
****************************************/

void Task::setOffset(int newOffset)
{
	if (newOffset < 0) throw std::logic_error("setOffset : offset must be > 0");
	_offset = newOffset;
}

int Task::getOffset()
{
	return _offset;
}

void Task::setPeriod(int newPeriod)
{
	if (newPeriod <= 0) throw std::logic_error("setPeriod : period must be > 0");
	//if (newPeriod < getWcet()) throw std::logic_error("setPeriod : period must be >= wcet");
	_period = newPeriod;
}

int Task::getPeriod()
{
	return _period;
}

void Task::setDeadline(int newDeadline)
{
	if (newDeadline <= 0) throw std::logic_error("setDeadline : deadline must be > 0");
	//if (newDeadline < getWcet()) throw std::logic_error("setDeadline : deadline must be >= wcet");
	_deadline = newDeadline;
}

int Task::getDeadline()
{
	return _deadline;
}

void Task::setWcet(int newWcet)
{
	if (newWcet <= 0) throw std::logic_error("setWcet : wcet must be > 0");
	_wcet = newWcet;
}

int Task::getWcet()
{
	return _wcet;
}

void Task::reComputeUtilisation()
{
	_utilisation = (float)_wcet / _period;
}
void Task::setUtilisation(float newUtilisation)
{
	if (newUtilisation <= 0) throw std::logic_error("newUtilisation : utilisation must be > 0");
	_utilisation = newUtilisation;
}

float Task::getUtilisation()
{
	return _utilisation;
}

void Task::setPriority(bool newPriority)
{
	_priority = newPriority;
}

bool Task::getPriority()
{
	return _priority;
}

ostream& operator << (ostream& s, Task t)
{
	s << t.asString();
	return s;
}
