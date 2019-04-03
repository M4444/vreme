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
	int h;
	int m;
public:
	Time(int hh = 0, int mm = 0) : h(hh), m(mm) {}
	Time(string time) { parseTime(time); }

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

	int parseTime(string time_str)
	{
		if (time_str.compare("now") == 0) {
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
			return 1;
		}

		int buff, ret = 0;
		double md = 0;
		char c;
		stringstream ss(time_str);

		ss >> buff;
		ss >> c;

		switch (c) {
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

		return ret;
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

	Time operator+(const Time &time)
	{
		add(time.h, time.m);
		return *this;
	}

	Time &operator+=(const Time &time)
	{
		*this = *this + time;
		return *this;
	}

	Time &sub(int hh, int mm)
	{
		h -= hh;
		if (m < mm) {
			h--;
			m = 60 - (mm - m);
		} else {
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

	Time operator-(const Time &time)
	{
		sub(time.h, time.m);
		return *this;
	}

	Time &operator-=(const Time &time)
	{
		*this = *this - time;
		return *this;
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
	if (obj.isNegative())
		os << '-';
	os << setfill('0') << setw(2);
	os << obj.getH() << ':';
	os << setfill('0') << setw(2);
	os << obj.getM();

	return os;
}
