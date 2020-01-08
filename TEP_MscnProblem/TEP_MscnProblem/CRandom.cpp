#include "pch.h"
#include "CRandom.h"

int CRandom::iRange(int iMin, int iMax)
{
	if(iMin > iMax)
		return -1;

	return iMin + (rand() % (1 + iMax - iMin));
}

int CRandom::iRangeOpen(int iMin, int iMax)
{
	if(iMin >= iMax || iMax - iMin == 1)
		return -1;

	int i_random = rand() % (iMax - iMin);

	return iMin + ((i_random == 0) ? 1 : i_random);
}

int CRandom::iRangeClosedLeft(int iMin, int iMax)
{
	if(iMin >= iMax)
		return -1;

	return iMin + (rand() % (iMax - iMin));
}

int CRandom::iRangeClosedRight(int iMin, int iMax)
{
	if(iMin >= iMax)
		return -1;

	int i_random = rand() % (1 + iMax - iMin);

	return iMin + ((i_random == 0) ? 1 : i_random);
}

double CRandom::dRange(double dMin, double dMax)
{
	if(dMin > dMax)
		return -1.0;

	return dMin + d_random() * (dMax - dMin);;
}

double CRandom::d_random()
{
	int i_rand_a, i_rand_b;

	i_rand_a = rand();
	i_rand_b = rand();

	return (double) std::min(i_rand_a, i_rand_b) / std::max(i_rand_a, i_rand_b);
}
