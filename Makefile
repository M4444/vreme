CXX = g++
CXXFLAGS = -g -Wall

all: vreme

vreme: vreme.o
	$(CXX) $(CXXFLAGS) -o vreme vreme.o

vreme.o: Time.h

test: vreme
	@./test.sh; /bin/true

clean:
	rm -f vreme.o vreme
