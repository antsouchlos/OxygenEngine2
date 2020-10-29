all: run

CXX = g++
CFLAGS = -Wall -g -std=gnu++20

FileWrapper.o: FileWrapper.h FileWrapper.cpp LexerException.h
	$(CXX) FileWrapper.cpp -c $(CFLAGS) -o $@

main.o: main.cpp LexerException.h readspeed.h
	$(CXX) main.cpp -c $(CFLAGS) -o $@

run: main.o FileWrapper.o
	$(CXX) main.o FileWrapper.o -o run

clean:
	rm main.o run
