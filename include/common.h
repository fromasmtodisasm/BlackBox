#pragma once

#include <functional>
#include <vector>
#include <string>

using std::string;
using std::vector;

enum {
	TSTR,
	TIVAL,
	TDVAL,
	TLIVAL,
	TLDVAL
};

struct UniType {
	union {
		char* sval;
		int ival;
		double dval;
		long int lival;
		long double ldval;
	};
	short int t;
};
