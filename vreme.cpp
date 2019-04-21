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
			exit_status = lex(tokens, token_group);
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
