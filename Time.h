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
	int h = 0;
	int m = 0;
public:
	Time() = default;

	Time(string time)
	{
		parseTime(time);
	}

	int getH() const
	{
		if (!isNegative())
			return h;
		else
			if (m == 0)
				return -h;
			else
				return -h - 1;
	}

	int getM() const
	{
		if (!isNegative())
			return m;
		else
			if (m == 0)
				return m;
			else
				return 60 - m;
	}

	bool isNegative() const
	{
		return h < 0;
	}

	static bool checkFormat(string time)
	{
		return Time().parseTime(time);
	}

	void clearTime()
	{
		h = 0;
		m = 0;
	}

	bool parseTime(string time_str)
	{
		if (time_str == "now") {
			time_t raw_time;
			struct tm *time_info;

			time(&raw_time);
			time_info = localtime(&raw_time);
			h = time_info->tm_hour;
			m = time_info->tm_min;
			// Round to the nearest minute
			if (time_info->tm_sec > 30) {
				if (++m > 59) {
					h = m / 60;
					m = m % 60;
				}
			}
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
				h = buff;
				ss >> m;
				if (m > 59)
					return false;
				break;
			case '.':
				h = buff;
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
				break;
			case 'h':
				h = buff;
				break;
			case 'm':
				m = buff;
				if (m > 59) {
					h = m / 60;
					m = m % 60;
				}
				break;
			default:
				return false;
				break;
			}
		}

		return true;
	}

	Time operator+(const Time &time)
	{
		h += time.h;
		m += time.m;
		if (m >= 60) {
			m = m % 60;
			h++;
		}

		return *this;
	}

	Time &operator+=(const Time &time)
	{
		*this = *this + time;
		return *this;
	}

	Time operator-(const Time &time)
	{
		h -= time.h;
		if (m < time.m) {
			h--;
			m = 60 - (time.m - m);
		} else {
			m -= time.m;
		}

		return *this;
	}

	Time &operator-=(const Time &time)
	{
		*this = *this - time;
		return *this;
	}
};

ostream &operator<<(ostream &os, const Time &obj)
{
	if (obj.isNegative())
		os << '-';
	os << setfill('0') << setw(2);
	os << obj.getH() << ':';
	os << setfill('0') << setw(2);
	os << obj.getM();

	return os;
}
