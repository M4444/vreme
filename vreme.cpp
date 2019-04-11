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
	switch (op) {
	case '+':
	case '-':
		return true;
	default:
		return false;
	}
}

inline string Bold(const string& text)
{
	return "\033[1m" + text + "\033[0m";
}

inline ostream& print_format_help(ostream& os)
{
	os << "Possible " << Bold("time") << " formats: "
	   << Bold("now, X:X, X/X, X.X, Xh, Xm") << '\n';
	os << "Example of equal " << Bold("time") << ": 4:45 = 4/45 = 4.75 = 4h + 45m" << '\n';
	return os;
}

inline ostream& print_operation_help(ostream& os)
{
	os << "Possible operations: " << Bold("+, -") << '\n';
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
		cout << step_counter++ << ") " << time_state << '\n';
		state = TIME;
		return;
	} else if (token.compare("help") == 0) {
		cout << "----------------------------------------------------" << '\n';
		cout << "Example command: " << Bold("9:16 + 7.5 - 10m") << '\n';
		cout << "Regular command format:"
		     << " (+|-)? " << Bold("time") << " ((+|-) " << Bold("time") << ")*" << '\n';
		cout << '\n';
		cout << print_format_help;
		cout << print_operation_help;
		cout << '\n';
		cout << Bold("clear") << "\t- reset the time to 00:00" << '\n';
		cout << Bold("help") << "\t- print this help menu" << '\n';
		cout << Bold("exit") << "\t- quit the program" << '\n';
		cout << "----------------------------------------------------" << '\n';
		return;
	}

	switch (state) {
	case TIME:
		if (!Time::checkFormat(token)) {
			if (!clean_enabled) {
				cout << "* Error: Unknown format" << '\n';
				cout << print_format_help;
				cout << "* Try '" << Bold("help") << "' for more info" << '\n';
			}
			return;
		}

		switch (operation) {
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

		cout << step_counter++ << ") " << time_state << '\n';
		state = OP;
		break;
	case OP:
		if (token.length() != 1 || !checkOp(token[0])) {
			if (!clean_enabled) {
				cout << "* Error: Unknown operation" << '\n';
				cout << print_operation_help;
				cout << "* Try '" << Bold("help") << "' for more info" << '\n';
			}
			return;
		}

		operation = token[0];

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
				cout << "Unknown argument: " << argv[i] << '\n';
				return -1;
			}
	}
	if (!clean_enabled)
		cout << "Time calculation:" << '\n';
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
					//cout << "operation: " << subtoken[0] << '\n';
					state = OP;
					parse(subtoken.substr(0, 1));
					subtoken = subtoken.substr(1);
				}
				//cout << "substring: " << subtoken << '\n';
				parse(subtoken);
			}
		}
	}
}
