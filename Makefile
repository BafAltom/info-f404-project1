CPP=g++
CPPFLAGS=-Wall -g -pedantic -Wextra -std=c++98 -Weffc++ -Wstrict-null-sentinel
LDFLAGS=
EXEC=main taskGenerator

.PHONY: clean, mrproper

all: $(EXEC)

main: main.o task.o simulation.o job.o
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
