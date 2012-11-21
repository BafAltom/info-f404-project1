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
};

#endif