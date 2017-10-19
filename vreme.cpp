#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Time
{
private:
	int h;
	int m;
	double md;

	char op;
public:
	Time()
	{
		h = 0;
		m = 0;

		op = 0;
	}
	Time(int hh, int mm)
	{
		h = hh;
		m = mm;

		op = 0;
	}

	int getH() const
	{
		return h;
	}

	int getM() const
	{
		return m;
	}
	// Time(int h, double m)
	// {
	// 	this.h = h;
	// 	this.md = m;
	// }

	static int checkFormat(string time)
	{
		return Time().parseTime(time);
	}

	int setTime(string time)
	{
		return parseTime(time);
	}

	void clearTime()
	{
		h = 0;
		m = 0;
	}

	int parseTime(string time)
	{
		int buff, ret = 0;
		char c;
		stringstream ss(time);

		ss >> buff;
		ss >> c;

		switch(c) {
		case ':':
		case '/':
			h = buff;
			if (h > 24) {
				// printf("Warning: over 24h\n");
				ret = 1;
			}
			ss >> m;
			if (m > 59)
				return -2;
			break;
		case '.':
		 	h = buff;
			if (h > 24) {
				// printf("Warning: over 24h\n");
				ret = 1;
			}
			ss >> c;
			if (c < '0' || c > '9')
				return -1;
			ss >> md;
			if (md > 9)
				return -3;
			else {
				md += (c - '0') * 10;
				m = md * 60 / 100;
			}
		 	break;
		case 'h':
			h = buff;
			if (h > 24)
				ret = 1;
			break;
		case 'm':
			m = buff;
			if (m > 59) {
				h = m / 60;
				m = m % 60;
				// printf("Warning: over 60m\n");
				ret = 2;
			}
			break;
		default:
			// printf("* Error: unknown format\n");
			return -1;
			break;
		}

		return 1;
	}

	static bool checkOp(char op)
	{
		switch(op)
		{
		case '+':
		case '-':
			return true;
		default:
			return false;
		}
	}

	void setOp(char op)
	{
		this->op = op;
	}

	Time &doOp(string time)
	{
		switch(op)
		{
		case '+':
			add(time);
			break;
		case '-':
			sub(time);
			break;
		default:
			break;
		}

		return *this;
	}

	Time &add(int hh, int mm)
	{
		h += hh;
		m += mm;
		if (m >= 60) {
			m = m % 60;
			h++;
		}

		return *this;
	}
	Time &add(string time)
	{
		Time newTime = Time();
		// if (parseTime(time, hh, mm) < 0)
		// 	return // error
		newTime.parseTime(time);
		add(newTime.h, newTime.m);

		return *this;
	}

	Time &operator+(const Time &t2)
	{
		add(t2.h, t2.m);
	}

	Time &sub(int hh, int mm)
	{
		h -= hh;
		if (m < mm)
		{
			h--;
			m = 60 - (mm - m);
		}
		else {
			m -= mm;
		}

		return *this;
	}
	Time &sub(string time)
	{
		Time newTime = Time();
		newTime.parseTime(time);
		sub(newTime.h, newTime.m);

		return *this;
	}

	Time &operator-(const Time &t2)
	{
		sub(t2.h, t2.m);
	}

	// std::istream& operator>>(std::istream& is, T& obj)
	// {
	// 	// read obj from stream
	//	if( /* T could not be constructed */ )
	//	    is.setstate(std::ios::failbit);
	//	return is;
	// }
};

ostream &operator<<(ostream &os, const Time &obj)
{
	os << setfill ('0') << setw (2);
	os << obj.getH() << ':';
	os << setfill ('0') << setw (2);
	os << obj.getM();
	return os;
}

enum STATE {
	START,
	TIME,
	OP,
	EXIT,
	CLEAR
};

STATE state;
int i;
Time t;

void parse(string token)
{
	// check for commands
	if(token.compare("exit") == 0)
		exit(0);
	else if(token.compare("clear") == 0) {
		t.clearTime();
		t.setOp('+');
		cout << i++ << ") " << t << endl;
		state = TIME;
		return;
	}
	else if(token.compare("help") == 0) {
		cout << "possible format e.g. 4:45 = 4/45 = 4.75 = 4h + 45m" << endl;
		cout << "possible operations: +, -" << endl;
		return;
	}

	switch(state) {
	case TIME:
		if(Time::checkFormat(token) < 0) {
			cout << "* Error: unknown format (possible format e.g. 4:45 = 4/45 = 4.75 = 4h + 45m)" << endl;
			state = OP;
			return;
		}

		t.doOp(token);

		cout << i++ << ") " << t << endl;
		state = OP;
		break;
	case OP:
		if (token.length() != 1 || !Time::checkOp(token[0])) {
			cout << "* Error: unknown operation (possible operations: +, -)" << endl;
			state = OP;
			return;
		}

		t.setOp(token[0]);
		// in there is no error add it

		state = TIME;
		break;
	}
}

int main()
{
	t = Time();
	string token = string();
	string subtoken = string();
	state = TIME;

	cout << "Time calculation:" << endl;
	i = 0;
	t.setOp('+');
	while (true) {
		cin >> token;
		int low = 0;
		for (int j = 0; j < token.length(); j++) {
			if (Time::checkOp(token[j]) || j == (token.length() - 1)) {
				if (j == (token.length() - 1))
					subtoken = token.substr(low);
				else
					subtoken = token.substr(low, j-low);
				low = j;
				if (subtoken.length() > 1 && Time::checkOp(subtoken[0])) {
					//cout << "op: " << subtoken[0] << endl;
					parse(subtoken.substr(0,1));
					subtoken = subtoken.substr(1);
				}
				//cout << "substring: " << subtoken << endl;
				parse(subtoken);
			}
		}

	}
}
