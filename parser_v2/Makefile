all: run

CXX = g++
CXXFLAGS = -Wall -g --std=gnu++14
OBJFILES = obj/Lexer.o obj/Exception.o

obj/%.o : %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

obj/main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

run: $(OBJFILES) obj/main.o
	$(CXX) -o run $(OBJFILES) obj/main.o

clean:
	rm $(OBJFILES) obj/main.o
	rm run
