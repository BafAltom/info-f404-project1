#include <iostream>
#include <string>
#include <exception>
using namespace std;

#include "task.hpp"

int main()
{
	cout << "hello" << endl;
	Task t = Task("1	1	1	1");
	cout << t << endl;
	return 0;
}