#include "simGlobalEDFClass.hpp"

int main(int argc, char** argv)
{
	simGlobalEDF globalEDF;
	globalEDF.run(argv[1]);

	return 0;
}
