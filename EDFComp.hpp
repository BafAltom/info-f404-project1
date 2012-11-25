#ifndef EDFCOMP_H
#define EDFCOMP_H

#include "job.hpp"

template<bool smallestDeadlineIsMax>
class EDFComp
{
public:
	EDFComp() {	}
	EDFComp(Job* a, Job* b) {	}
	bool operator()(Job* a, Job* b)
	{
		if (a == NULL or b == NULL)
		{
			return false;
		}
		if(a->getPriority())
		{
			//cout<<"a prior"<<endl;
			if (smallestDeadlineIsMax) // running
			{
				return false;
			}
			else // ready
			{
				return true;
			} 
		}
		else if(b->getPriority())
		{
			//cout<<"b prior"<<endl;
			if (smallestDeadlineIsMax) // runn
			{
				return true;
			}
			else //ready
			{
				return false;
			}
		}
		else
		{
			int deadLineA = a->getAbsoluteDeadline();
			int deadLineB = b->getAbsoluteDeadline();
			if (smallestDeadlineIsMax)
			{
				return deadLineA < deadLineB;
			}
			else
			{
				return deadLineA > deadLineB;
			}
		}
	}
};

#endif
