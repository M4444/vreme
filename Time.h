/*
 * Copyright (C) 2017-2019 Miloš Stojanović
 *
 * SPDX-License-Identifier: MIT
 */

#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

class Time
{
private:
	int m = 0;
public:
	Time() = default;

	Time(string time)
	{
		parseTime(time);
	}

	int getH() const
	{
		if (m < 0)
			return -m / 60;
		else
			return m / 60;
	}

	int getM() const
	{
		if (m < 0)
			return -m % 60;
		else
			return m % 60;
	}

	bool isNegative() const
	{
		return m < 0;
	}

	static bool checkFormat(string time)
	{
		return Time().parseTime(time);
	}

	void clearTime()
	{
		m = 0;
	}

	bool parseTime(string time_str)
	{
		if (time_str == "now") {
			time_t raw_time;
			struct tm *time_info;

			time(&raw_time);
			time_info = localtime(&raw_time);
			m = 60 * time_info->tm_hour;
			m += time_info->tm_min;
			// Round to the nearest minute
			if (time_info->tm_sec > 30)
				m++;
		} else {
			int buff = 0;
			double md = 0;
			char c;
			stringstream ss(time_str);

			ss >> buff;
			ss >> c;

			switch (c) {
			case ':':
			case '/':
				ss >> m;
				if (m > 59)
					return false;
				m += 60 * buff;
				break;
			case '.':
				ss >> c;
				if (c < '0' || c > '9')
					return false;
				ss >> md;
				if (md > 9)
					return false;
				else {
					md += (c - '0') * 10;
					m = md * 60 / 100;
				}
				m += 60 * buff;
				break;
			case 'h':
				m = 60 * buff;
				break;
			case 'm':
				m = buff;
				break;
			default:
				return false;
				break;
			}
		}

		return true;
	}

	Time operator+(const Time& time)
	{
		m += time.m;
		return *this;
	}

	Time& operator+=(const Time& time)
	{
		*this = *this + time;
		return *this;
	}

	Time operator-(const Time& time)
	{
		m -= time.m;
		return *this;
	}

	Time& operator-=(const Time& time)
	{
		*this = *this - time;
		return *this;
	}
};

ostream& operator<<(ostream& os, const Time& time)
{
	if (time.isNegative())
		os << '-';
	os << setfill('0') << setw(2)
	   << time.getH() << ':'
	   << setfill('0') << setw(2)
	   << time.getM();

	return os;
}
