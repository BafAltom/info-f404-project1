#include <iostream>
#include <string>
#include <exception>

#include "task.hpp"
using namespace std;

int main(int argc, char** argv)
{
	cout << "hello" << endl;
	Task t = Task(1,1,1,1);
	cout << t << endl;
	return 0;
}