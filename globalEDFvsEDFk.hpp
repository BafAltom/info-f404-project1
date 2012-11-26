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
#include "simEDFk.hpp"
#include "simGlobalEDF.hpp"
#include "taskGenerator.hpp"



class GlobalEDFvsEDFk {
	
public:
	
	GlobalEDFvsEDFk();
	void makeStat(int numTasks, int utilisation, int numTesting);
	

private:


};

#endif



