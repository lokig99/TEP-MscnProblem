#pragma once
#include "CMscnProblem.h"

#define ROUND_TO_ZERO_BELOW 0.1

class COptimizer
{
public:
	COptimizer(){ pc_problem = NULL; }
	COptimizer(CMscnProblem &cProblem) { pc_problem = &cProblem; }

	virtual void vSetInstance(CMscnProblem &cProblem) { pc_problem = &cProblem; }
	virtual CMscnSolution cGenerateSolution(int iIterations, 
											int iInitPopulation, int iSeed, double &dResultQuality) = 0;

protected:
	CMscnProblem *pc_problem;
};