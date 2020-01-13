#include "pch.h"
#include "CDiffEvol.h"
#include <iostream>


CMscnSolution CDiffEvol::cGenerateSolution(int iFitnessCalls, int iInitPopulation, double &dResultQuality)
{
	return cGenerateSolution(iFitnessCalls, iInitPopulation, 0, dResultQuality);
}

CMscnSolution CDiffEvol::cGenerateSolution(int iFitnessCalls, int iInitPopulation, int iSeed, double &dResultQuality)
{
	if(pc_problem == NULL || iFitnessCalls < 1 || iInitPopulation < MIN_POPULATION)
		return CMscnSolution();

	CRandom  c_rand;
	if(iSeed == 0)
		c_rand.vResetGlobalSeed();
	else
		c_rand.vSetGlobalSeed(iSeed);

	vector<vector<double>*> *pv_solution_quality_history = new vector<vector<double>*>;
	vector<CMscnSolution*> v_population;
	CMscnSolution c_best_solution;
	vector<double> v_tmp;
	double d_best_quality;
	int i_err_code = 0;
	int i_fit_calls = 0;
	int i_iteration_counter = 0;
	int i_errors = 0;

	//initialize population
	for(int i = 0; i < iInitPopulation; ++i)
	{
		CMscnSolution *pc_sol = new CMscnSolution();
		pc_sol->vSetProblem(*pc_problem);
		pc_sol->vGenerateInstance(rand());
		v_population.push_back(pc_sol);
		pv_solution_quality_history->push_back(new vector<double>());
	}

	while(i_fit_calls < iFitnessCalls)
	{
		++i_iteration_counter;
		std::cout << "generation= " <<i_iteration_counter << "\tfitness evaluations= " << i_fit_calls << std::endl;

		for(size_t i = 0; i < v_population.size(); ++i)
		{
			vector<int> v_rand_positions = c_rand.vGetVariedVector(0, v_population.size() - 1);
			vector<int>::iterator position = v_rand_positions.begin();

			CMscnSolution *p_ind = v_population[i];
			CMscnSolution *p_base = v_population[*position != i ? *position : *(++position)];
			CMscnSolution *p_add0 = v_population[*(++position) != i ? *position : *(++position)];
			CMscnSolution *p_add1 = v_population[*(++position) != i ? *position : *(++position)];
			vector<CMscnSolution*> v_indivs = { p_ind, p_base, p_add0, p_add1 };

			if(b_indivs_are_different(v_indivs))
			{
				CMscnSolution *p_ind_new = new CMscnSolution();
				*p_ind_new = *p_base;

				v_mutate_genotype(v_indivs, *p_ind_new, DEF_XD);
				v_mutate_genotype(v_indivs, *p_ind_new, DEF_XF);
				v_mutate_genotype(v_indivs, *p_ind_new, DEF_XM);

				double d_old_fitness, d_new_fitness;
				pc_problem->b_apply_solution(*p_ind, i_err_code);
				d_old_fitness = pc_problem->d_calculate_quality();
				if(i_err_code != 0)
					++i_errors;
				pc_problem->b_apply_solution(*p_ind_new, i_err_code);
				d_new_fitness = pc_problem->d_calculate_quality();
				if(i_err_code != 0)
					++i_errors;

				if(d_new_fitness >= d_old_fitness)
				{
					delete v_population[i];
					v_population[i] = p_ind_new;
				}
				else
					delete p_ind_new;
						
				++i_fit_calls;
				pv_solution_quality_history->at(i)->push_back(std::max(d_new_fitness, d_old_fitness));
			}
		}

		if(i_iteration_counter % ITERATION_INTERVAL == 0)
			if(b_indivs_are_equal(v_population))
				i_fit_calls = iFitnessCalls;	
	}

	printf("\nErrors: %d", i_errors);
	c_best_solution = *v_population[i_get_best_solution(v_population, d_best_quality)];
	b_save_to_csv_file(pv_solution_quality_history);
	
	//clear population
	for(vector<CMscnSolution*>::iterator it = v_population.begin(); it != v_population.end(); ++it)
		delete *it;

	//clear solution history
	for(vector<vector<double>*>::iterator it = pv_solution_quality_history->begin(); 
		it != pv_solution_quality_history->end(); ++it)
		delete *it;
	delete pv_solution_quality_history;

	dResultQuality = d_best_quality;
	return c_best_solution;
}

bool CDiffEvol::b_validate_genotype(CMscnSolution & cInd, const string &sMatrixName)
{
	int i_err_code;
	if(!pc_problem->b_apply_solution(cInd, i_err_code))
		return false;

	if(sMatrixName == DEF_XD)
	{
		return pc_problem->b_validate_prod_cap_sd() 
			&& pc_problem->b_validate_amount_xd_xf();
	}
	else if(sMatrixName == DEF_XF)
	{
		return  pc_problem->b_validate_amount_xd_xf()
			&& pc_problem->b_validate_prod_cap_sf() && pc_problem->b_validate_amount_xf_xm();
	}
	else if(sMatrixName == DEF_XM)
	{
		return pc_problem->b_validate_amount_sm()
			&& pc_problem->b_validate_amount_ss() && pc_problem->b_validate_amount_xf_xm();
	}
	else
		return false;
}

bool CDiffEvol::b_indivs_are_different(vector<CMscnSolution*> &vIndivs)
{
	return !b_indivs_are_equal(vIndivs);
}

bool CDiffEvol::b_indivs_are_equal(vector<CMscnSolution*>& vIndivs)
{
	for(size_t i = 1; i < vIndivs.size(); ++i)
		for(size_t j = 0; j < vIndivs[0]->pv_amount_xd->size(); ++j)
			for(size_t k = 0; k < (*vIndivs[0]->pv_amount_xd)[0].size(); ++k)
				if(std::abs((*vIndivs[0]->pv_amount_xd)[j][k] - (*vIndivs[i]->pv_amount_xd)[j][k]) > PRECISION)
					return false;

	for(size_t i = 1; i < vIndivs.size(); ++i)
		for(size_t j = 0; j < vIndivs[0]->pv_amount_xf->size(); ++j)
			for(size_t k = 0; k < (*vIndivs[0]->pv_amount_xf)[0].size(); ++k)
				if(std::abs((*vIndivs[0]->pv_amount_xf)[j][k] - (*vIndivs[i]->pv_amount_xf)[j][k]) > PRECISION)
					return false;

	for(size_t i = 1; i < vIndivs.size(); ++i)
		for(size_t j = 0; j < vIndivs[0]->pv_amount_xm->size(); ++j)
			for(size_t k = 0; k < (*vIndivs[0]->pv_amount_xm)[0].size(); ++k)
				if(std::abs((*vIndivs[0]->pv_amount_xm)[j][k] - (*vIndivs[i]->pv_amount_xm)[j][k]) > PRECISION)
					return false;

	return true;
}

bool CDiffEvol::b_save_to_csv_file(vector<vector<double>*> *pvSolutionQualityHistory)
{
	FILE *pf_file = fopen(CSV_FILE_NAME ".csv", "r");
	const int MAX_FOPEN_TRIES = 255;
	int i_tries = 0;
	string s_filename = CSV_FILE_NAME;

	while(pf_file != NULL && i_tries < MAX_FOPEN_TRIES)
	{
		++i_tries;
		s_filename += "-";
		fclose(pf_file);
		pf_file = fopen((s_filename + ".csv").c_str(), "r");
	}

	pf_file = fopen((s_filename + ".csv").c_str(), "w");
		
	if(pf_file != NULL)
	{
		for(size_t i = 0; i < pvSolutionQualityHistory->size(); ++i)
		{
			for(size_t j = 0; j < pvSolutionQualityHistory->at(i)->size(); ++j)
				if(!fprintf(pf_file, "%f;", pvSolutionQualityHistory->at(i)->at(j)))
					return false;
			if(!fprintf(pf_file, "\n"))
				return false;
		}
			
		fclose(pf_file);
		return true;
	}

	return false;
}

int CDiffEvol::i_get_best_solution(vector<CMscnSolution*>& vIndivs, double &dQualityOutput)
{
	int i_err;
	double d_best_quality = pc_problem->dGetQuality(*vIndivs[0], i_err);
	int i_position_of_best = 0;

	for(size_t i = 1; i < vIndivs.size(); ++i)
	{
		double d_quality = pc_problem->dGetQuality(*vIndivs[i], i_err);

		if(d_quality > d_best_quality)
		{
			d_best_quality = d_quality;
			i_position_of_best = i;
		}			
	}

	dQualityOutput = d_best_quality;
	return i_position_of_best;
}

void CDiffEvol::v_mutate_genotype(vector<CMscnSolution*>& vBasePool, CMscnSolution & cMutatedIndiv, const string &sMatrixName)
{
	if(vBasePool.size() < 4)
		return;

	CRandom c_rand;
	CMscnSolution* p_ind_new = &cMutatedIndiv;
	vector<vector<double>> *pv_matrix_old;
	vector<vector<double>> *pv_matrix_base;
	vector<vector<double>> *pv_matrix_add0;
	vector<vector<double>> *pv_matrix_add1;
	vector<vector<double>> *pv_matrix_new;

	if(sMatrixName == DEF_XD)
	{
		pv_matrix_old = vBasePool[0]->pv_amount_xd;
		pv_matrix_base = vBasePool[1]->pv_amount_xd;
		pv_matrix_add0 = vBasePool[2]->pv_amount_xd;
		pv_matrix_add1 = vBasePool[3]->pv_amount_xd;
		pv_matrix_new = cMutatedIndiv.pv_amount_xd;
	}
	else if(sMatrixName == DEF_XF)
	{
		pv_matrix_old = vBasePool[0]->pv_amount_xf;
		pv_matrix_base = vBasePool[1]->pv_amount_xf;
		pv_matrix_add0 = vBasePool[2]->pv_amount_xf;
		pv_matrix_add1 = vBasePool[3]->pv_amount_xf;
		pv_matrix_new = cMutatedIndiv.pv_amount_xf;
	}
	else if(sMatrixName == DEF_XM)
	{
		pv_matrix_old = vBasePool[0]->pv_amount_xm;
		pv_matrix_base = vBasePool[1]->pv_amount_xm;
		pv_matrix_add0 = vBasePool[2]->pv_amount_xm;
		pv_matrix_add1 = vBasePool[3]->pv_amount_xm;
		pv_matrix_new = cMutatedIndiv.pv_amount_xm;
	}
	else
		return;

	for(size_t i = 0; i < pv_matrix_base->size(); ++i)
	{
		for(size_t j = 0; j < pv_matrix_base->at(0).size(); ++j)
		{
			const double D_BACKUP_GENE = pv_matrix_new->at(i).at(j);

			if(c_rand.dRange(0, 1) < CROSS_PROBABILITY)
			{
				pv_matrix_new->at(i).at(j) = pv_matrix_base->at(i).at(j) +
					DIFF_WEIGHT * (pv_matrix_add0->at(i).at(j) - pv_matrix_add1->at(i).at(j));

				if(pv_matrix_new->at(i).at(j) < ROUND_TO_ZERO_BELOW)
					pv_matrix_new->at(i).at(j) = 0.0;
			}
			else
				pv_matrix_new->at(i).at(j) = pv_matrix_base->at(i).at(j);

			if(!b_validate_genotype(cMutatedIndiv, sMatrixName))
				pv_matrix_new->at(i).at(j) = D_BACKUP_GENE;
		}
	}
}
