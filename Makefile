CPP=g++
CPPFLAGS=-Wall -g -pedantic -Wextra -std=c++98 -Weffc++ -Wstrict-null-sentinel
LDFLAGS=
EXEC=main taskGenerator

.PHONY: clean, mrproper

all: $(EXEC)

main: main.o task.o simulation.o
		$(CPP) -o $@ $^ $(LDFLAGS)

taskGenerator: taskGenerator.o task.o
		$(CPP) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
		$(CPP) -o $@ -c $< $(CPPFLAGS)

clean:
		rm -rf *.o
		rm -rf out*

mrproper: clean
		rm -rf $(EXEC)
