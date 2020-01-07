#pragma once
#include "CMscnProblem.h"
#include "CRandom.h"

#define ROUND_TO_ZERO_BELOW 0.1

class CRandomSearch
{
public:
	CRandomSearch() { pc_problem = NULL; }
	CRandomSearch(CMscnProblem &cProblem) { pc_problem = &cProblem; }
	void vSetInstance(CMscnProblem &cProblem) { pc_problem = &cProblem; }
	double dGenerateSolution(int iIterations, vector<double> &vSolution);

private:
	CMscnProblem *pc_problem;

	bool b_fill_matrix(vector<vector<double>> &vMatrix, vector<vector<vector<double>>> &vRange, double dTopLimit);
	bool b_fill_matrix(vector<vector<double>> &vMatrix, vector<vector<vector<double>>> &vRange);
	double d_matrix_sum_of_values(vector<vector<double>> &vMatrix);
};

