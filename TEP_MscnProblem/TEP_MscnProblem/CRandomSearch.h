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
	double dGenerateSolution(int iIterations, vector<double> &vSolution);
	double dGenerateSolution(int iIterations, vector<double> &vSolution, int iSeed);
	double dGenerateSolution(int iIterations, int iSeed);

private:
	CMscnProblem *pc_problem;
	double d_resources_amount(vector<vector<double>> &vMatrix, int iPosition);
	bool b_fill_safe_xd();
	bool b_fill_safe_xf();
	bool b_fill_safe_xm();
	bool b_save_to_csv_file(vector<double> &vSolutionQualityHistory);
};

