/*
 * Copyright (C) 2017-2019 Miloš Stojanović
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum TOKEN_TYPE {
	NONE,
	OP,
	TIME,
	COMMAND,
};

class Token {
private:
	TOKEN_TYPE type;
	string str;
public:
	Token(const string& token_string, TOKEN_TYPE token_type):
		type(token_type), str(token_string) {};

	string getStr() const { return str; }

	TOKEN_TYPE getType() const { return type; }
};

#endif
