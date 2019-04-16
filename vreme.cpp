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

#include "Output.h"
#include "Parser.h"
#include "Time.h"
#include "Token.h"

using namespace std;

bool isOp(const char c)
{
	switch (c) {
	case '+':
	case '-':
		return true;
	default:
		return false;
	}
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
	if (isOp(token[0]))
		return OP;
	else if (isCommand(token))
		return COMMAND;
	else if (Time::isTime(token))
		return TIME;
	else
		return NONE;
}

int lex(vector<Token>& tokens, const char token_group[])
{
	string token_string;
	const char *token_start = token_group;
	size_t token_length = 0;
	bool op_encountered = false;

	if (!token_group)
		return EXIT_FAILURE;
	do {
		if (isOp(*token_group) || op_encountered || *token_group == '\0') {
			op_encountered = isOp(*token_group);
			if (token_length > 0) {
				token_string = string(token_start, token_length);
				TOKEN_TYPE type = getTokenType(token_string);
				if (type != NONE) {
					tokens.emplace_back(token_string, type);
				} else {
					error_output << "* Error: Unknown format" << '\n';
					error_output << print_format_help;
					error_output << "* Try '" << Bold("help") << "' for more info" << '\n';
					return EXIT_FAILURE;
				}
			}
			token_start += token_length;
			token_length = 0;
		}
		token_length++;
	} while (*token_group++);

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
	Parser parser;
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
			exit_status = lex(tokens, token_group.c_str());
		}
		for (const Token& token : tokens) {
			if (lex_enabled) {
				cout << ' ' << token.getStr();
			} else {
				if (exit_status == EXIT_FAILURE)
					break;
				exit_status = parser.parse(token);
			}
		}
		if (lex_enabled && !tokens.empty())
			cout << '\n';
	}
}
