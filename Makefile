CPP=g++
CPPFLAGS=-Wall -pedantic -Wextra -std=c++98 -Weffc++ -Wstrict-null-sentinel
LDFLAGS=
EXEC=main

.PHONY: clean, mrproper

all: $(EXEC)

main: main.o task.o
		$(CPP) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
		$(CPP) -o $@ -c $< $(CPPFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf $(EXEC)
