#include "pch.h"
#include "CRandomSearch.h"

CMscnSolution CRandomSearch::cGenerateSolution(int iIterations, double &dResultQuality)
{
	return cGenerateSolution(iIterations, 0, dResultQuality);
}

CMscnSolution CRandomSearch::cGenerateSolution(int iIterations, int iSeed, double &dResultQuality)
{
	if(pc_problem == NULL || iIterations < 1)
		return CMscnSolution();

	CRandom  c_rand;
	if(iSeed == 0)
		c_rand.vResetGlobalSeed();
	else
		c_rand.vSetGlobalSeed(iSeed);

	CMscnSolution c_best_solution;
	vector<double> v_solution_quality_history;
	double d_best_quality;
	int i_err_code;
	bool b_best_empty = true;
	CMscnSolution c_solution;
	c_solution.vSetProblem(*pc_problem);

	for(int i = 0; i < iIterations; ++i)
	{
		c_solution.vGenerateInstance(rand());

		double d_quality = pc_problem->dGetQuality(c_solution, i_err_code);
		v_solution_quality_history.push_back(d_quality);

		if(b_best_empty)
		{
			c_solution.vTranferDataTo(c_best_solution);
			d_best_quality = d_quality;
			b_best_empty = false;
		}
		else if(d_quality > d_best_quality)
		{
			c_solution.vTranferDataTo(c_best_solution);
			d_best_quality = d_quality;
		}
	}

	dResultQuality = d_best_quality;
	b_save_to_csv_file(v_solution_quality_history);
	return c_best_solution;
}

bool CRandomSearch::b_save_to_csv_file(vector<double> &vSolutionQualityHistory)
{
	FILE *pf_file = fopen(CSV_FILE_NAME, "w");

	if(pf_file != NULL)
	{
		for(size_t i = 0; i < vSolutionQualityHistory.size(); ++i)
			if(!fprintf(pf_file, "%f\n", vSolutionQualityHistory[i]))
				return false;
		
		fclose(pf_file);
		return true;
	}

	return false;
}

