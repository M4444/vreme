/*
 * Copyright (C) 2017-2019 Miloš Stojanović
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstring>
#include <iostream>
#include <string>

#include "Time.h"

using namespace std;

bool clean_enabled = false;

enum STATE {
	TIME,
	OP
};

STATE state;
int step_counter;
Time time_state;
char operation;

bool checkOp(char op)
{
	switch(op) {
	case '+':
	case '-':
		return true;
	default:
		return false;
	}
}

void doOp(Time &time_state, string token)
{
	switch(operation) {
	case '+':
		time_state += Time(token);
		break;
	case '-':
		time_state -= Time(token);
		break;
	default:
		// error
		break;
	}
}

struct Bold {
	string bold_string;
	Bold(string str) : bold_string(str) {}
	friend std::ostream& operator<<(ostream &os, const Bold &bold_stirng);
};

std::ostream& operator<<(ostream &os, const Bold &b)
{
    return os << "\033[1m" << b.bold_string << "\033[0m";
}

std::ostream& print_format_help(std::ostream& os)
{
	os << "Possible " << Bold("time") << " formats: "
	   << Bold("now, X:X, X/X, X.X, Xh, Xm") << endl;
	os << "Example of equal " << Bold("time") << ": 4:45 = 4/45 = 4.75 = 4h + 45m" << endl;
	cout << "* Try '" << Bold("help") << "' for more info" << endl;
	return os;
}

std::ostream& print_operation_help(std::ostream& os)
{
	os << "Possible operations: " << Bold("+, -") << endl;
	cout << "* Try '" << Bold("help") << "' for more info" << endl;
	return os;
}

void parse(string token)
{
	// check for commands
	if (token.compare("exit") == 0)
		exit(0);
	else if (token.compare("clear") == 0) {
		time_state.clearTime();
		operation = '+';
		cout << step_counter++ << ") " << time_state << endl;
		state = TIME;
		return;
	} else if (token.compare("help") == 0) {
		cout << "----------------------------------------------------" << endl;
		cout << "Example command: " << Bold("9:16 + 7.5 - 10m") << endl;
		cout << "Regular command format:"
		     << " (+|-)? " << Bold("time") << " ((+|-) " << Bold("time") << ")*" << endl;
		cout << endl;
		cout << print_format_help;
		cout << print_operation_help;
		cout << endl;
		cout << Bold("clear") << "	- reset the time to 00:00" << endl;
		cout << Bold("help") << "	- print this help menu" << endl;
		cout << Bold("exit") << "	- quit the program" << endl;
		cout << "----------------------------------------------------" << endl;
		return;
	}

	switch(state) {
	case TIME:
		if (Time::checkFormat(token) < 0) {
			if (!clean_enabled) {
				cout << "* Error: Unknown format" << endl;
				cout << print_format_help;
			}
			return;
		}

		doOp(time_state, token);

		cout << step_counter++ << ") " << time_state << endl;
		state = OP;
		break;
	case OP:
		if (token.length() != 1 || !checkOp(token[0])) {
			if (!clean_enabled) {
				cout << "* Error: Unknown operation" << endl;
				cout << print_operation_help;
			}
			return;
		}

		operation = token[0];
		// in there is no error add it

		state = TIME;
		break;
	}
}

int main(int argc, char *argv[])
{
	state = TIME;
	step_counter = 0;
	time_state = Time();
	operation = '+';

	string token = string();
	string subtoken = string();

	if (argc > 1) {
		for (int i = 1; i < argc; i++)
			if (strcmp(argv[i], "--clean") == 0)
				clean_enabled = true;
			else {
				cout << "Unknown argument: " << argv[i] << endl;
				return -1;
			}
	}
	if (!clean_enabled)
		cout << "Time calculation:" << endl;
	while (true) {
		cin >> token;
		if (cin.eof())
			return 0;
		int low = 0;
		for (size_t i = 0; i < token.length(); i++) {
			if ((i > 0 && checkOp(token[i])) || i == (token.length() - 1)) {
				if (i == (token.length() - 1))
					subtoken = token.substr(low);
				else
					subtoken = token.substr(low, i-low);
				low = i;
				if (subtoken.length() > 1 && checkOp(subtoken[0])) {
					//cout << "operation: " << subtoken[0] << endl;
					state = OP;
					parse(subtoken.substr(0,1));
					subtoken = subtoken.substr(1);
				}
				//cout << "substring: " << subtoken << endl;
				parse(subtoken);
			}
		}
	}
}
