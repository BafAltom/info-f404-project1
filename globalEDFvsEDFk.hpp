#ifndef GLOBALEDFVSEDFK_H
#define GLOBALEDFVSEDFK_H

#include <iostream>
#include <string>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <time.h>
#include <algorithm>
using namespace std;

#include "task.hpp"
#include "simEDFkClass.hpp"
#include "simGlobalEDFClass.hpp"
#include "taskGeneratorClass.hpp"

class GlobalEDFvsEDFk {
	
public:
	
	GlobalEDFvsEDFk();
	void makeStat(int numTasks, int utilisation, int numTesting);
	void makeStats();
	

private:


};

#endif



