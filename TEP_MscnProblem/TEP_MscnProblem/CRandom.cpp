#include "pch.h"
#include "CRandom.h"


int CRandom::iRange(int iMin, int iMax)
{
	if(iMin > iMax)
		return INT_INFINITY;

	return iMin + (rand() % (1 + iMax - iMin));
}

int CRandom::iRangeOpen(int iMin, int iMax)
{
	if(iMin >= iMax || iMax - iMin == 1)
		return INT_INFINITY;

	int i_random = rand() % (iMax - iMin);

	return iMin + ((i_random == 0) ? 1 : i_random);
}

int CRandom::iRangeClosedLeft(int iMin, int iMax)
{
	if(iMin >= iMax)
		return INT_INFINITY;

	return iMin + (rand() % (iMax - iMin));
}

int CRandom::iRangeClosedRight(int iMin, int iMax)
{
	if(iMin >= iMax)
		return INT_INFINITY;

	int i_random = rand() % (1 + iMax - iMin);

	return iMin + ((i_random == 0) ? 1 : i_random);
}

double CRandom::dRange(double dMin, double dMax)
{
	if(dMin > dMax)
		return INFINITY;

	return dMin + d_random() * (dMax - dMin);;
}

vector<int> CRandom::vGetVariedVector(int iMin, int iMax)
{
	vector<int> v_range;
	vector<int> v_res;

	for(int i = iMin; i <= iMax; ++i)
		v_range.push_back(i);

	int i_target_size = v_range.size();

	while(v_res.size() < i_target_size)
	{
		int i_pos = iRangeClosedLeft(0, v_range.size());
		v_res.push_back(v_range[i_pos]);
		v_range.erase(v_range.begin() + i_pos);
	}

	return v_res;
}

double CRandom::d_random()
{
	int i_rand_a, i_rand_b;

	i_rand_a = rand();
	i_rand_b = rand();

	return (double) std::min(i_rand_a, i_rand_b) / std::max(i_rand_a, i_rand_b);
}
