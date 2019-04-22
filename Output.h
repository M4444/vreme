/*
 * Copyright (C) 2017-2019 Miloš Stojanović
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <ostream>
#include <string>

using namespace std;

struct ErrorOutput {
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

#endif
