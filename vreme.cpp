/*
 * Copyright (C) 2017-2019 Miloš Stojanović
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Time.h"
#include "Token.h"

using namespace std;

struct ErrorOutput
{
	bool clean_enabled = false;

	template <typename T>
	ErrorOutput& operator<<(const T& obj)
	{
		if (!clean_enabled)
			cerr << obj;
		return *this;
	}
};

ErrorOutput error_output;

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

bool isOp(char c)
{
	switch (c) {
	case '+':
	case '-':
		return true;
	default:
		return false;
	}
}

bool isOp(const string& token)
{
	if (token.length() != 1)
		return false;
	return isOp(token[0]);
}

bool isCommand(const string& token)
{
	if (token == "exit" ||
	    token == "clear" ||
	    token == "help")
		return true;
	else
		return false;
}

TOKEN_TYPE getTokenType(string token)
{
	if (isOp(token))
		return OP;
	else if (isCommand(token))
		return COMMAND;
	else if (Time::isTime(token))
		return TIME;
	else
		return NONE;
}

int insertAndCheckToken(vector<Token>& tokens, const string& token_string)
{
	TOKEN_TYPE type = getTokenType(token_string);
	if (type != NONE) {
		tokens.emplace_back(token_string, type);
		return EXIT_SUCCESS;
	} else {
		error_output << "* Error: Unknown format" << '\n';
		error_output << print_format_help;
		error_output << "* Try '" << Bold("help") << "' for more info" << '\n';
		return EXIT_FAILURE;
	}
}

int lex(vector<Token>& tokens, const string& token_bunch)
{
	string token_string;
	size_t token_start = 0;
	size_t token_length = 0;
	for (size_t i = 0; i < token_bunch.length(); i++) {
		if (isOp(token_bunch[i])) {
			if (token_length > 0) {
				token_string = token_bunch.substr(token_start, token_length);
				if (insertAndCheckToken(tokens, token_string) == EXIT_FAILURE)
					return EXIT_FAILURE;
				token_start += token_length;
				token_length = 0;
			}

			token_length++;
			token_string = token_bunch.substr(token_start, token_length);
			if (insertAndCheckToken(tokens, token_string) == EXIT_FAILURE)
				return EXIT_FAILURE;
			token_start += token_length;
			token_length = 0;
		} else {
			token_length++;
		}
	}
	if (token_start < token_bunch.length()) {
		token_string = token_bunch.substr(token_start, token_length);
		if (insertAndCheckToken(tokens, token_string) == EXIT_FAILURE)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

struct State {
	TOKEN_TYPE expected_type;
	int step_counter;
	Time time;
	char operation;
};

int parse(State& state, const Token& token)
{
	switch (token.getType()) {
	case COMMAND:
		if (token.getStr() == "exit")
			exit(EXIT_SUCCESS);
		else if (token.getStr() == "clear") {
			state.time.clearTime();
			state.operation = '+';
			cout << state.step_counter++ << ") " << state.time << '\n';
			state.expected_type = TIME;
		} else if (token.getStr() == "help") {
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
		}
		break;
	case TIME:
		if (state.expected_type == OP) {
			error_output << "* Error: Operation not specified" << '\n';
			error_output << "After a time an operation needs to be specified next" << '\n';
			error_output << print_operation_help;
			error_output << "* Try '" << Bold("help") << "' for more info" << '\n';
			return EXIT_FAILURE;
		} else {
			switch (state.operation) {
			case '+':
				state.time += Time(token.getStr());
				break;
			case '-':
				state.time -= Time(token.getStr());
				break;
			default:
				// error
				break;
			}

			cout << state.step_counter++ << ") " << state.time << '\n';
			state.expected_type = OP;
		}
		break;
	case OP:
		if (state.expected_type == TIME) {
			error_output << "* Error: Time not specified" << '\n';
			error_output << "After an operation time needs to be specified next" << '\n';
			error_output << print_format_help;
			error_output << "* Try '" << Bold("help") << "' for more info" << '\n';
			return EXIT_FAILURE;
		} else {
			state.operation = token.getStr()[0];

			state.expected_type = TIME;
		}
		break;
	case NONE:
		// Lexer handles this case so this should be unreachable
		error_output << "* Error: Unknown format" << '\n';
		error_output << print_format_help;
		error_output << "* Try '" << Bold("help") << "' for more info" << '\n';
		return EXIT_FAILURE;
		break;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	bool lex_enabled = false;

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "--clean") == 0) {
				error_output.clean_enabled = true;
			} else if (strcmp(argv[i], "--lex") == 0) {
				lex_enabled = true;
			} else {
				error_output << "Unknown argument: " << argv[i] << '\n';
				return EXIT_FAILURE;
			}
		}
	}

	if (!error_output.clean_enabled)
		cout << "Time calculation:" << '\n';

	int exit_status = EXIT_SUCCESS;
	State state;
	state.expected_type = TIME;
	state.step_counter = 0;
	state.time = Time();
	state.operation = '+';
	// Iterate over user inputs (full lines)
	while (true) {
		string line;
		getline(cin, line);
		if (cin.eof())
			return exit_status;

		exit_status = EXIT_SUCCESS;
		istringstream ss(line);
		vector<Token> tokens;
		// Iterate over whitespace-separated strings
		while (!ss.eof() && exit_status != EXIT_FAILURE) {
			string token_group;
			ss >> token_group;
			exit_status = lex(tokens, token_group);
		}
		for (const Token& token : tokens) {
			if (lex_enabled) {
				cout << ' ' << token.getStr();
			} else {
				if (exit_status == EXIT_FAILURE)
					break;
				exit_status = parse(state, token);
			}
		}
		if (lex_enabled)
			cout << '\n';
	}
}
