#include "task.hpp"

Task::Task() : _offset(0), _period(0), _deadline(0), _wcet(0)
{	}

Task::Task(int offset, int period, int deadline, int wcet) : _offset(offset), _period(period), _deadline(deadline), _wcet(wcet)
{	}

Task::Task(string parseString) : _offset(0), _period(0), _deadline(0), _wcet(0)
{

	string s = "string, to, split";
	istringstream ss(parseString);
	string parsedStrings[4];
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

}

int Task::getUtilizationPercent()
{
	return (_wcet*100/_deadline);
}

string Task::asString()
{
	std::ostringstream s;
	s << getOffset() << "\t"
	  << getPeriod() << "\t"
	  << getDeadline() << "\t"
	  << getWcet();
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
	//if (newWcet > getDeadline()) throw std::logic_error("setWcet : deadline must be >= wcet");
	//if (newWcet > getPeriod()) throw std::logic_error("setWcet : period must be >= wcet");
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