CXX = g++
CXXFLAGS = -g -Wall

all: vreme

vreme: vreme.o
	$(CXX) $(CXXFLAGS) -o vreme vreme.o

vreme.o: Output.h Parser.h Time.h Token.h

test: vreme
	@./test.sh

test-all: vreme
	@./test.sh --all

clean:
	rm -f vreme.o vreme
