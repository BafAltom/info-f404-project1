CPP=g++
CPPFLAGS=-Wall -g -pedantic -Wextra -std=c++98 -Weffc++ -Wstrict-null-sentinel
LDFLAGS=
EXEC=simGlobalEDF taskGenerator simEDF-k globalEDFvsEDF-k

.PHONY: clean, mrproper

all: $(EXEC)

simGlobalEDF: simGlobalEDF.o simGlobalEDFClass.o task.o simulation.o job.o EDFComp.o
		$(CPP) -o $@ $^ $(LDFLAGS)

simEDF-k: simEDF-k.o simEDFkClass.o task.o simulation.o job.o EDFComp.o
		$(CPP) -o $@ $^ $(LDFLAGS)

taskGenerator: taskGenerator.o task.o EDFComp.o taskGeneratorClass.o
		$(CPP) -o $@ $^ $(LDFLAGS)
		
globalEDFvsEDF-k: globalEDFvsEDFk.o taskGeneratorClass.o simEDFkClass.o simGlobalEDFClass.o task.o simulation.o job.o EDFComp.o
		$(CPP) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
		$(CPP) -o $@ -c $< $(CPPFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf $(EXEC)
		rm -rf out*
