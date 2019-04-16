/*
 * Copyright (C) 2017-2019 Miloš Stojanović
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "Time.h"

using namespace std;

enum STATE {
	TIME,
	OP
};

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

struct Full_state {
	STATE state;
	int step_counter;
	Time time;
	char operation;
};

int parse(Full_state& full_state, string token, const bool clean_enabled)
{
	// check for commands
	if (token.compare("exit") == 0)
		exit(0);
	else if (token.compare("clear") == 0) {
		full_state.time.clearTime();
		full_state.operation = '+';
		cout << full_state.step_counter++ << ") " << full_state.time << '\n';
		full_state.state = TIME;
		return EXIT_SUCCESS;
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
		return EXIT_SUCCESS;
	}

	switch (full_state.state) {
	case TIME:
		if (!Time::checkFormat(token)) {
			if (!clean_enabled) {
				cout << "* Error: Unknown format" << '\n';
				cout << print_format_help;
				cout << "* Try '" << Bold("help") << "' for more info" << '\n';
			}
			return EXIT_FAILURE;
		}

		switch (full_state.operation) {
		case '+':
			full_state.time += Time(token);
			break;
		case '-':
			full_state.time -= Time(token);
			break;
		default:
			// error
			break;
		}

		cout << full_state.step_counter++ << ") " << full_state.time << '\n';
		full_state.state = OP;
		break;
	case OP:
		if (token.length() != 1 || !checkOp(token[0])) {
			if (!clean_enabled) {
				cout << "* Error: Unknown operation" << '\n';
				cout << print_operation_help;
				cout << "* Try '" << Bold("help") << "' for more info" << '\n';
			}
			return EXIT_FAILURE;
		}

		full_state.operation = token[0];

		full_state.state = TIME;
		break;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	bool clean_enabled = false;

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "--clean") == 0) {
				clean_enabled = true;
			} else {
				cout << "Unknown argument: " << argv[i] << '\n';
				return EXIT_FAILURE;
			}
		}
	}
	if (!clean_enabled)
		cout << "Time calculation:" << '\n';

	int exit_status = EXIT_SUCCESS;
	Full_state full_state;
	full_state.state = TIME;
	full_state.step_counter = 0;
	full_state.time = Time();
	full_state.operation = '+';

	string token = string();
	string subtoken = string();
	while (true) {
		cin >> token;
		if (cin.eof())
			return exit_status;
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
					full_state.state = OP;
					exit_status = parse(full_state, subtoken.substr(0, 1), clean_enabled);
					subtoken = subtoken.substr(1);
				}
				//cout << "substring: " << subtoken << '\n';
				exit_status = parse(full_state, subtoken, clean_enabled);
			}
		}
	}
}
