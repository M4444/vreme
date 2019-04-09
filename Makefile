CXX = g++
CXXFLAGS = -g -Wall

all: vreme

vreme: vreme.o
	$(CXX) $(CXXFLAGS) -o vreme vreme.o

vreme.o: Time.h

clean:
	rm -f vreme.o vreme
