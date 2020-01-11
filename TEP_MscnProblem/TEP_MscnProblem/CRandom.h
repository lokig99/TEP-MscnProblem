#pragma once
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <vector>
#include <iostream>

#define INT_INFINITY std::numeric_limits<int>::max()

using std::vector;

class CRandom
{
public:
	CRandom() {}
	int iRange(int iMin, int iMax);
	int iRangeOpen(int iMin, int iMax);
	int iRangeClosedLeft(int iMin, int iMax);
	int iRangeClosedRight(int iMin, int iMax);
	double dRange(double dMin, double dMax);
	void vResetGlobalSeed() { srand(time(NULL)); }
	void vSetGlobalSeed(int iSeed) { srand(iSeed); }
	vector<int> vGetVariedVector(int iMin, int iMax);

private:
	double d_random();
};

