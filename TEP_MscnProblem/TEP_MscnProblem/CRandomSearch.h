#pragma once
#include "CMscnProblem.h"
#include "CRandom.h"

#define ROUND_TO_ZERO_BELOW 0.1
#define CSV_FILE_NAME "RS-Data.csv"

class CRandomSearch
{
public:
	CRandomSearch() { pc_problem = NULL; }
	CRandomSearch(CMscnProblem &cProblem) { pc_problem = &cProblem; }
	void vSetInstance(CMscnProblem &cProblem) { pc_problem = &cProblem; }
	CMscnSolution cGenerateSolution(int iIterations, double &dResultQuality);
	CMscnSolution cGenerateSolution(int iIterations, int iSeed, double &dResultQuality);

private:
	CMscnProblem *pc_problem;
	bool b_save_to_csv_file(vector<double> &vSolutionQualityHistory);
};

