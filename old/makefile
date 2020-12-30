CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -Wall -g

tests.exe: main.o libdarformula.so
	$(CXX) $(CXXFLAGS) -o tests.exe main.o libdarformula.so

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

libdarformula.so: darformula.o Tokens.o Exceptions.o
	$(CXX) $(CXXFLAGS) -shared -o libdarformula.so darformula.o Tokens.o Exceptions.o

darformula.o: darformula.cpp Tokens.o Exceptions.o

Tokens.o: Tokens.cpp

Exceptions.o: Exceptions.cpp