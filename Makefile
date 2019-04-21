CXX = g++
CXXFLAGS = -g -Wall

all: vreme

vreme: vreme.o
	$(CXX) $(CXXFLAGS) -o vreme vreme.o

vreme.o: Output.h Parser.h Time.h Token.h

test: vreme
	@./test.sh; /bin/true

test-all: vreme
	@./test.sh --all; /bin/true

clean:
	rm -f vreme.o vreme
