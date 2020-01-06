#pragma once
#include <cstdlib>
#include <algorithm>
#include <ctime>

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

private:
	double d_random();
};

