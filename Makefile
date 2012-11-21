CPP=g++
CPPFLAGS=-Wall -g -pedantic -Wextra -std=c++98 -Weffc++ -Wstrict-null-sentinel
LDFLAGS=
EXEC=simGlobalEDF simEDFk taskGenerator

.PHONY: clean, mrproper

all: $(EXEC)

simGlobalEDF: simGlobalEDF.o task.o simulation.o job.o
		$(CPP) -o $@ $^ $(LDFLAGS)
		
simEDFk: simEDFk.o task.o simulation.o job.o
		$(CPP) -o $@ $^ $(LDFLAGS)

taskGenerator: taskGenerator.o task.o
		$(CPP) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
		$(CPP) -o $@ -c $< $(CPPFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf $(EXEC)
		rm -rf out*
