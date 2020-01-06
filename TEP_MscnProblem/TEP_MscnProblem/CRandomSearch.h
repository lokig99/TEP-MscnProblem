#pragma once
#include "CMscnProblem.h"
#include "CRandom.h"

#define MAX_ITERATION_LIMIT 1E42
#define ROUND_TO_ZERO_BELOW 0.1

class CRandomSearch
{
public:
	CRandomSearch();
	CRandomSearch(CMscnProblem &cProblem);
	void vSetInstance(CMscnProblem &cProblem);
	double dGenerateSolution(int iIterations, vector<double> &vSolution);

private:
	CMscnProblem *pc_problem;
	bool b_get_empty_solution(vector<double> &vOutput);
	bool b_fill_matrix(vector<vector<double>> &vMatrix, vector<vector<vector<double>>> &vRange, double dTopLimit);
	bool b_fill_matrix(vector<vector<double>> &vMatrix, vector<vector<vector<double>>> &vRange);
	double d_matrix_sum_of_values(vector<vector<double>> &vMatrix);
};

