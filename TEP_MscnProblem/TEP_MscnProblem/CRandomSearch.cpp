#include "pch.h"
#include "CRandomSearch.h"

double CRandomSearch::dGenerateSolution(int iIterations, vector<double>& vSolution)
{
	return dGenerateSolution(iIterations, vSolution, 0);
}

double CRandomSearch::dGenerateSolution(int iIterations, vector<double> &vSolution, int iSeed)
{
	if(pc_problem == NULL || iIterations < 1)
		return -1.0;

	CRandom  c_rand;
	if(iSeed == 0)
		c_rand.vResetGlobalSeed();
	else
		c_rand.vSetGlobalSeed(iSeed);

	vector<double> v_best_solution;
	vector<double> v_solution_quality_history;
	double d_best_quality;
	int i_err_code;

	for(int i = 0; i < iIterations; ++i)
	{
		vector<double> v_solution;

		while(!pc_problem->bConstraintsSatisfied(v_solution, i_err_code))
		{
			bool b_is_filled = false;

			//fill xd
			while(!b_is_filled || !pc_problem->b_validate_prod_cap_sd())
			{
				b_fill_safe_xd();
				b_is_filled = true;
			}
			
			//fill xf
			b_is_filled = false;
			while(!b_is_filled || !pc_problem->b_validate_prod_cap_sf()
				  || !pc_problem->b_validate_amount_xd_xf())
			{
				b_fill_safe_xf();
				b_is_filled = true;
			}

			//fill xm
			b_is_filled = false;
			while(!b_is_filled || !pc_problem->b_validate_amount_sm()
				  || !pc_problem->b_validate_amount_ss() || !pc_problem->b_validate_amount_xf_xm())
			{
				b_fill_safe_xm();
				b_is_filled = true;
			}

			v_solution = pc_problem->vGetSolutionVector();
		}

		double d_quality = pc_problem->dGetQuality(v_solution, i_err_code);
		v_solution_quality_history.push_back(d_quality);

		if(v_best_solution.empty())
		{
			v_best_solution = v_solution;
			d_best_quality = d_quality;
		}
		else if(d_quality > d_best_quality)
		{
			v_best_solution = v_solution;
			d_best_quality = d_quality;
		}
	}

	pc_problem->b_apply_solution(v_best_solution, i_err_code);
	vSolution = v_best_solution;
	b_save_to_csv_file(v_solution_quality_history);
	return d_best_quality;
}

double CRandomSearch::dGenerateSolution(int iIterations, int iSeed)
{
	vector<double> v_dummy;
	return dGenerateSolution(iIterations, v_dummy, iSeed);
}

double CRandomSearch::d_resources_amount(vector<vector<double>> &vMatrix, int iPosition)
{
	double d_res = 0.0;

	if(iPosition >= vMatrix[0].size())
		return d_res;

	for(size_t i = 0; i < vMatrix.size(); ++i)
		d_res += vMatrix[i][iPosition];

	return d_res;
}

//fills xd matrix with random values which satisfy constraints of CMscnProblem
bool CRandomSearch::b_fill_safe_xd()
{
	if(pc_problem == NULL)
		return false;

	CRandom c_rand;
	vector<vector<vector<double>>> v_range = pc_problem->v_minmax_xd;

	for(size_t i = 0; i < pc_problem->v_amount_xd.size(); ++i)
	{
		double d_deliverers_prod_cap_left = pc_problem->v_production_cap_sd[i];

		for(size_t j = 0; j < pc_problem->v_amount_xd[0].size(); ++j)
		{
			pc_problem->v_amount_xd[i][j] = 
				c_rand.dRange(v_range[i][j][0], std::min(v_range[i][j][1], 
							  std::min(d_deliverers_prod_cap_left, pc_problem->v_production_cap_sf[j])));

			if(pc_problem->v_amount_xd[i][j] < ROUND_TO_ZERO_BELOW)
				pc_problem->v_amount_xd[i][j] = 0.0;

			d_deliverers_prod_cap_left -= pc_problem->v_amount_xd[i][j];
		}
	}

	return true;
}

//fills xf matrix with random values which satisfy constraints of CMscnProblem
bool CRandomSearch::b_fill_safe_xf()
{
	if(pc_problem == NULL)
		return false;

	CRandom c_rand;
	vector<vector<vector<double>>> v_range = pc_problem->v_minmax_xf;

	for(size_t i = 0; i < pc_problem->v_amount_xf.size(); ++i)
	{
		double d_products_left = d_resources_amount(pc_problem->v_amount_xd, i);
		double d_factory_prod_cap_left = std::min(pc_problem->v_production_cap_sf[i], d_products_left);
		
		for(size_t j = 0; j < pc_problem->v_amount_xf[0].size(); ++j)
		{
			pc_problem->v_amount_xf[i][j] = 
				c_rand.dRange(v_range[i][j][0], 
							  std::min(v_range[i][j][1], 
							  std::min(d_factory_prod_cap_left, pc_problem->v_capacity_sm[j])));

			if(pc_problem->v_amount_xf[i][j] < ROUND_TO_ZERO_BELOW)
				pc_problem->v_amount_xf[i][j] = 0.0;

			d_factory_prod_cap_left -= pc_problem->v_amount_xf[i][j];
		}
	}

	return true;
}

//fills xm matrix with random values which satisfy constraints of CMscnProblem
bool CRandomSearch::b_fill_safe_xm()
{
	if(pc_problem == NULL)
		return false;

	CRandom c_rand;
	vector<vector<vector<double>>> v_range = pc_problem->v_minmax_xm;

	for(size_t i = 0; i < pc_problem->v_amount_xm.size(); ++i)
	{
		double d_products_left = d_resources_amount(pc_problem->v_amount_xf, i);
		double d_magazine_cap_left = std::min(pc_problem->v_capacity_sm[i], d_products_left);

		for(size_t j = 0; j < pc_problem->v_amount_xm[0].size(); ++j)
		{
			pc_problem->v_amount_xm[i][j] = 
				c_rand.dRange(v_range[i][j][0], 
							  std::min(v_range[i][j][1], 
							  std::min(d_magazine_cap_left, pc_problem->v_need_ss[j])));

			if(pc_problem->v_amount_xm[i][j] < ROUND_TO_ZERO_BELOW)
				pc_problem->v_amount_xm[i][j] = 0.0;

			d_magazine_cap_left -= pc_problem->v_amount_xm[i][j];
		}
	}

	return true;
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

