#include "pch.h"
#include "CDiffEvol.h"
#include <iostream>


double CDiffEvol::dGenerateSolution(int iFitnessCalls, int iInitPopulation, vector<double>& vSolution)
{
	return dGenerateSolution(iFitnessCalls, iInitPopulation, vSolution, 0);
}

double CDiffEvol::dGenerateSolution(int iFitnessCalls, int iInitPopulation, vector<double>& vSolution, int iSeed)
{
	if(pc_problem == NULL || iFitnessCalls < 1 || iInitPopulation < MIN_POPULATION)
		return -1.0;

	CRandom  c_rand;
	if(iSeed == 0)
		c_rand.vResetGlobalSeed();
	else
		c_rand.vSetGlobalSeed(iSeed);

	CRandomSearch c_rs(*pc_problem);
	vector<vector<double>> v_solution_quality_history;
	vector<Indiv*> v_population;
	vector<double> v_best_solution;
	vector<double> v_tmp;
	double d_best_quality;
	int i_err_code = 0;
	int i_fit_calls = 0;
	int i_iteration_counter = 0;

	//initialize population
	for(int i = 0; i < iInitPopulation; ++i)
	{
		c_rs.dGenerateSolution(1, v_tmp, rand());
		v_population.push_back(new Indiv(v_tmp));
		v_solution_quality_history.push_back(vector<double>());
	}

	while(i_fit_calls < iFitnessCalls)
	{
		++i_iteration_counter;
		std::cout << "generation= " <<i_iteration_counter << "\tfitness evaluations= " << i_fit_calls << std::endl;

		for(size_t i = 0; i < v_population.size(); ++i)
		{
			if(i_fit_calls < iFitnessCalls) 
			{
				vector<int> v_rand_positions = c_rand.vGetVariedVector(0, v_population.size() - 1);
				vector<int>::iterator position = v_rand_positions.begin();

				Indiv *p_ind = v_population[i];
				Indiv *p_base = v_population[*position != i ? *position : *(++position)];
				Indiv *p_add0 = v_population[*(++position) != i ? *position : *(++position)];
				Indiv *p_add1 = v_population[*(++position) != i ? *position : *(++position)];
				vector<Indiv*> v_indivs = { p_ind, p_base, p_add0, p_add1 };

				if(b_indivs_are_different(v_indivs))
				{
					Indiv *p_ind_new = new Indiv(v_tmp);

					for(int gene_offset = MIN_GENE_OFFSET; gene_offset < p_base->i_genotype_size; ++gene_offset)
					{
						const double D_BACKUP_GENE = p_ind_new->pd_tab[gene_offset];

						if(c_rand.dRange(0, 1) < CROSS_PROBABILITY)
						{
							p_ind_new->pd_tab[gene_offset] = p_base->pd_tab[gene_offset] +
								DIFF_WEIGHT * (p_add0->pd_tab[gene_offset] - p_add1->pd_tab[gene_offset]);

							if(p_ind_new->pd_tab[gene_offset] < ROUND_TO_ZERO_BELOW)
								p_ind_new->pd_tab[gene_offset] = 0.0;
						}
						else
							p_ind_new->pd_tab[gene_offset] = p_ind->pd_tab[gene_offset];

						if(!b_validate_genotype(*p_ind_new, i_err_code))
							p_ind_new->pd_tab[gene_offset] = D_BACKUP_GENE;
					}

					double d_old_fitness, d_new_fitness;
					v_tmp = p_ind->v_vector();
					d_old_fitness = pc_problem->dGetQuality(v_tmp, i_err_code);

					v_tmp = p_ind_new->v_vector();
					d_new_fitness = pc_problem->dGetQuality(v_tmp, i_err_code);

					if(d_new_fitness >= d_old_fitness)
					{
						delete p_ind;
						v_population[i] = p_ind_new;
					}
					else
						delete p_ind_new;

					++i_fit_calls;
					v_solution_quality_history[i].push_back(std::max(d_new_fitness, d_old_fitness));
				}
			}
		}

		if(i_iteration_counter % ITERATION_INTERVAL == 0)
			if(b_indivs_are_equal(v_population))
				i_fit_calls = iFitnessCalls;	
	}

	v_best_solution = v_get_best_solution(v_population, d_best_quality);

	//clear population
	for(vector<Indiv*>::iterator it = v_population.begin(); it != v_population.end(); ++it)
		delete *it;
	v_population.clear();

	pc_problem->b_apply_solution(v_best_solution, i_err_code);
	b_save_to_csv_file(v_solution_quality_history);
	return d_best_quality;
}

double CDiffEvol::dGenerateSolution(int iFitnessCalls, int iInitPopulation, int iSeed)
{
	vector<double> v_dummy;
	return dGenerateSolution(iFitnessCalls, iInitPopulation, v_dummy, iSeed);
}

bool CDiffEvol::b_validate_genotype(Indiv & ind, int iErrCode)
{
	vector<double> v_tmp = ind.v_vector();
	return pc_problem->bConstraintsSatisfied(v_tmp, iErrCode);
}

bool CDiffEvol::b_indivs_are_different(vector<Indiv*> &vIndivs)
{
	return !b_indivs_are_equal(vIndivs);
}

bool CDiffEvol::b_indivs_are_equal(vector<Indiv*>& vIndivs)
{
	for(size_t i = 1; i < vIndivs.size(); ++i)
		for(int gene_offset = MIN_GENE_OFFSET; gene_offset < vIndivs[0]->i_genotype_size; ++gene_offset)
			if(std::abs(vIndivs[0]->pd_tab[gene_offset] - vIndivs[i]->pd_tab[gene_offset]) > PRECISION)
				return false;

	return true;
}

bool CDiffEvol::b_save_to_csv_file(vector<vector<double>>& vSolutionQualityHistory)
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
		for(size_t i = 0; i < vSolutionQualityHistory.size(); ++i)
		{
			for(size_t j = 0; j < vSolutionQualityHistory[i].size(); ++j)
				if(!fprintf(pf_file, "%f;", vSolutionQualityHistory[i][j]))
					return false;
			if(!fprintf(pf_file, "\n"))
				return false;
		}
			
		fclose(pf_file);
		return true;
	}

	return false;
}

vector<double> CDiffEvol::v_get_best_solution(vector<Indiv*>& vIndivs, double &dQualityOutput)
{
	int i_err;
	vector<double> v_best_solution = vIndivs[0]->v_vector();
	double d_best_quality = pc_problem->dGetQuality(v_best_solution, i_err);

	for(size_t i = 1; i < vIndivs.size(); ++i)
	{
		vector<double> v_tmp = vIndivs[i]->v_vector();
		double d_quality = pc_problem->dGetQuality(v_tmp, i_err);

		if(d_quality > d_best_quality)
		{
			v_best_solution = v_tmp;
			d_best_quality = d_quality;
		}
	}

	dQualityOutput = d_best_quality;
	return v_best_solution;
}


//////////////////////// Indiv ///////////////////////////

CDiffEvol::Indiv::Indiv(int iGenotypeSize)
{
	i_genotype_size = iGenotypeSize;
	pd_tab = new double[i_genotype_size];
}

CDiffEvol::Indiv::Indiv(vector<double> &vSolution)
{
	i_genotype_size = vSolution.size();
	pd_tab = new double[i_genotype_size];

	for(size_t gene_offset = 0; gene_offset < vSolution.size(); ++gene_offset)
		pd_tab[gene_offset] = vSolution[gene_offset];
}

CDiffEvol::Indiv::Indiv(const Indiv & other)
{
	v_copy(other);
}

CDiffEvol::Indiv::~Indiv()
{
	if(pd_tab != NULL)
		delete[] pd_tab;
}

void CDiffEvol::Indiv::operator=(const Indiv & other)
{
	this->~Indiv();
	v_copy(other);
}

void CDiffEvol::Indiv::v_copy(const Indiv & other)
{
	i_genotype_size = other.i_genotype_size;
	pd_tab = new double[i_genotype_size];

	for(int i = 0; i < i_genotype_size; ++i)
		pd_tab[i] = other.pd_tab[i];
}

vector<double> CDiffEvol::Indiv::v_vector()
{
	vector<double> v_res;

	for(int i = 0; i < i_genotype_size; ++i)
		v_res.push_back(pd_tab[i]);

	return v_res;
}


