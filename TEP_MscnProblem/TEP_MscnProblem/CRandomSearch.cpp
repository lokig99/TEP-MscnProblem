#include "pch.h"
#include "CRandomSearch.h"

double CRandomSearch::dGenerateSolution(int iIterations, vector<double> &vSolution)
{
	if(pc_problem == NULL || iIterations < 1)
		return -1.0;

	CRandom  c_rand;
	c_rand.vResetGlobalSeed();

	vector<double> v_best_solution;
	double d_best_quality;
	int i_err_code;

	for(int i = 0; i < iIterations; ++i)
	{
		vector<double> v_solution;

		while(!pc_problem->bConstraintsSatisfied(v_solution, i_err_code))
		{
			double d_xd_sum, d_xf_sum = INFINITY, d_xm_sum = INFINITY;
			bool b_is_filled = false;

			//fill xd
			while(!b_is_filled || !pc_problem->b_validate_prod_cap_sd())
			{
				b_fill_matrix(pc_problem->v_amount_xd, pc_problem->v_minmax_xd);
				d_xd_sum = d_matrix_sum_of_values(pc_problem->v_amount_xd);
				b_is_filled = true;
			}
			
			//fill xf
			b_is_filled = false;
			while(!b_is_filled || d_xd_sum < d_xf_sum || !pc_problem->b_validate_prod_cap_sf()
				  || !pc_problem->b_validate_amount_xd_xf())
			{
				b_fill_matrix(pc_problem->v_amount_xf, pc_problem->v_minmax_xf, d_xd_sum);
				d_xf_sum = d_matrix_sum_of_values(pc_problem->v_amount_xf);
				b_is_filled = true;
			}

			//fill xm
			b_is_filled = false;
			while(!b_is_filled || d_xf_sum < d_xm_sum || !pc_problem->b_validate_amount_sm()
				  || !pc_problem->b_validate_amount_ss() || !pc_problem->b_validate_amount_xf_xm())
			{
				b_fill_matrix(pc_problem->v_amount_xm, pc_problem->v_minmax_xm, d_xf_sum);
				d_xm_sum = d_matrix_sum_of_values(pc_problem->v_amount_xm);
				b_is_filled = true;
			}

			v_solution = pc_problem->vGetSolutionVector();
		}

		if(v_best_solution.empty())
		{
			v_best_solution = v_solution;
			d_best_quality = pc_problem->dGetQuality(v_solution, i_err_code);
		}
		else
		{
			double d_quality = pc_problem->dGetQuality(v_solution, i_err_code);
			if(d_quality > d_best_quality)
			{
				v_best_solution = v_solution;
				d_best_quality = d_quality;
			}
		}
	}

	pc_problem->b_apply_solution(v_best_solution, i_err_code);
	vSolution = v_best_solution;
	return d_best_quality;
}

bool CRandomSearch::b_fill_matrix(vector<vector<double>>& vMatrix, vector<vector<vector<double>>>& vRange, double dTopLimit)
{
	CRandom c_rand;

	if(pc_problem == NULL)
		return false;

	double d_sum = 0.0;

	for(size_t i = 0; i < vMatrix.size(); ++i)
	{
		for(size_t j = 0; j < vMatrix[0].size(); ++j)
		{
			vMatrix[i][j] = c_rand.dRange(vRange[i][j][0], std::min(vRange[i][j][1], dTopLimit - d_sum));
			
			if(vMatrix[i][j] < ROUND_TO_ZERO_BELOW)
				vMatrix[i][j] = 0.0;

			d_sum += vMatrix[i][j];
		}
	}

	return true;
}

bool CRandomSearch::b_fill_matrix(vector<vector<double>>& vMatrix, vector<vector<vector<double>>>& vRange)
{
	CRandom c_rand;

	if(pc_problem == NULL)
		return false;

	for(size_t i = 0; i < vMatrix.size(); ++i)
	{
		for(size_t j = 0; j < vMatrix[0].size(); ++j)
		{
			vMatrix[i][j] = c_rand.dRange(vRange[i][j][0], vRange[i][j][1]);

			if(vMatrix[i][j] < ROUND_TO_ZERO_BELOW)
				vMatrix[i][j] = 0.0;
		}
	}

	return true;
}

double CRandomSearch::d_matrix_sum_of_values(vector<vector<double>>& vMatrix)
{
	double d_sum = 0.0;

	for(size_t i = 0; i < vMatrix.size(); ++i)
	{
		for(size_t j = 0; j < vMatrix[0].size(); ++j)
		{
			d_sum += vMatrix[i][j];
		}
	}

	return d_sum;
}

