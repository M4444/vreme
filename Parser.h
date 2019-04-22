/*
 * Copyright (C) 2017-2019 Miloš Stojanović
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef PARSER_H
#define PARSER_H

#include <cstdlib>
#include <iostream>

#include "Output.h"
#include "Time.h"
#include "Token.h"

using namespace std;

class Parser {
private:
	TOKEN_TYPE expected_type = TIME;
	int step_counter = 0;
	Time time = Time();
	char operation = '+';
public:
	int parse(const Token& token)
	{
		switch (token.getType()) {
		case COMMAND:
			if (token.getStr() == "exit")
				exit(EXIT_SUCCESS);
			else if (token.getStr() == "clear") {
				time.clearTime();
				operation = '+';
				cout << step_counter++ << ") " << time << '\n';
				expected_type = TIME;
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
			if (expected_type == OP) {
				error_output << "* Error: Operation not specified" << '\n';
				error_output << "After a time an operation needs to be specified next" << '\n';
				error_output << print_operation_help;
				error_output << "* Try '" << Bold("help") << "' for more info" << '\n';
				return EXIT_FAILURE;
			} else {
				switch (operation) {
				case '+':
					time += Time(token.getStr());
					break;
				case '-':
					time -= Time(token.getStr());
					break;
				default:
					// error
					break;
				}

				cout << step_counter++ << ") " << time << '\n';
				expected_type = OP;
			}
			break;
		case OP:
			if (expected_type == TIME) {
				error_output << "* Error: Time not specified" << '\n';
				error_output << "After an operation time needs to be specified next" << '\n';
				error_output << print_format_help;
				error_output << "* Try '" << Bold("help") << "' for more info" << '\n';
				return EXIT_FAILURE;
			} else {
				operation = token.getStr()[0];

				expected_type = TIME;
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

};

#endif
