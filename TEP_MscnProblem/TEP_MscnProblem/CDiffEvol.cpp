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
	vector<Indiv> v_population;
	vector<double> v_best_solution;
	vector<double> v_tmp;
	double d_best_quality;
	int i_err_code;
	int i_fit_calls = 0;
	int i_error_count = 0;

	//initialize population
	for(int i = 0; i < iInitPopulation; ++i)
	{
		c_rs.dGenerateSolution(1, v_tmp, rand());
		v_population.push_back(Indiv(v_tmp));
	}

	while(i_fit_calls < iFitnessCalls)
	{
		for(size_t i = 0; i < v_population.size(); ++i)
		{
			Indiv ind = v_population[i];
			Indiv base = v_population[c_rand.iRangeClosedLeft(0, v_population.size())];
			Indiv add0 = v_population[c_rand.iRangeClosedLeft(0, v_population.size())];
			Indiv add1 = v_population[c_rand.iRangeClosedLeft(0, v_population.size())];
			Indiv ind_new(v_tmp);

			vector<Indiv> v_indivs = { ind, base, add0, add1 };

			if(b_indivs_are_different(v_indivs))
			{
				for(int gene_offset = MIN_GENE_OFFSET; gene_offset < base.i_genotype_size; ++gene_offset)
				{
					double d_backup = ind_new.pd_tab[gene_offset];

					if(c_rand.dRange(0, 1) < CROSS_PROBABILITY)
					{
						ind_new.pd_tab[gene_offset] = base.pd_tab[gene_offset] +
							DIFF_WEIGHT * (add0.pd_tab[gene_offset] - add1.pd_tab[gene_offset]);

						if(ind_new.pd_tab[gene_offset] < 0.0)
							ind_new.pd_tab[gene_offset] = 0.0;
					}
					else
					{
						ind_new.pd_tab[gene_offset] = ind.pd_tab[gene_offset];
					}

					v_tmp = v_get_vector(ind_new);
					if(!pc_problem->bConstraintsSatisfied(v_tmp, i_err_code))
						ind_new.pd_tab[gene_offset] = d_backup;
				}

				double d_old_fitness, d_new_fitness;
				v_tmp = v_get_vector(ind);

				d_old_fitness = pc_problem->dGetQuality(v_tmp, i_err_code);

				v_tmp = v_get_vector(ind_new);
				d_new_fitness = pc_problem->dGetQuality(v_tmp, i_err_code);

				if(i_err_code != 0)
					++i_error_count;

				if(i_err_code == 0 && d_new_fitness >= d_old_fitness)
				{
					v_population[i] = ind_new;
					std::cout << i_fit_calls << std::endl;
				}
				++i_fit_calls;
			}
			else if(b_population_is_equal(v_population))
				i_fit_calls = iFitnessCalls;
		}
	}

	//find best solution
	v_best_solution = v_get_vector(v_population[0]);
	d_best_quality = pc_problem->dGetQuality(v_best_solution, i_err_code);

	for(size_t i = 1; i < v_population.size(); ++i)
	{
		v_tmp = v_get_vector(v_population[i]);
		double d_quality = pc_problem->dGetQuality(v_tmp, i_err_code);

		if(d_quality > d_best_quality)
		{
			v_best_solution = v_tmp;
			d_best_quality = d_quality;
		}
	}


	std::cout << "ERRORS: " << i_error_count << std::endl;
	pc_problem->b_apply_solution(v_best_solution, i_err_code);
	return d_best_quality;
}

double CDiffEvol::dGenerateSolution(int iFitnessCalls, int iInitPopulation, int iSeed)
{
	vector<double> v_dummy;
	return dGenerateSolution(iFitnessCalls, iInitPopulation, v_dummy, iSeed);
}

bool CDiffEvol::b_validate_genotype(Indiv & ind, int iErrCode)
{
	vector<double> v_tmp = v_get_vector(ind);
	return pc_problem->bConstraintsSatisfied(v_tmp, iErrCode);
}

bool CDiffEvol::b_fix_genotype(Indiv & ind)
{
	int i_err = 0;
	int i_dummy;

	if(b_validate_genotype(ind, i_err))
		return true;

	vector<double> v_tmp = v_get_vector(ind);
	pc_problem->b_apply_solution(v_tmp, i_dummy);

	if(i_err == INVALID_PROD_CAP_SD)
	{
		for(size_t i = 0; i < pc_problem->v_amount_xd.size(); ++i)
		{
			double d_products = 0.0;

			for(size_t j = 0; j < pc_problem->v_amount_xd[0].size(); ++j)
			{
				d_products += pc_problem->v_amount_xd[i][j];
			}

		}

	}
	else if(i_err == INVALID_PROD_CAP_SF)
	{

	}
	else if(i_err == INVALID_AMOUNT_SM)
	{

	}
	else if(i_err == INVALID_AMOUNT_SS)
	{

	}
	else if(i_err == INVALID_AMOUNT_XD_XF)
	{

	}
	else if(i_err == INVALID_AMOUNT_XF_XM)
	{

	}

	return b_fix_genotype(ind);
}

bool CDiffEvol::b_indivs_are_different(vector<Indiv> &vIndivs)
{
	vector<vector<double>> v_tabs;

	for(size_t i = 0; i < vIndivs.size(); ++i)
	{
		v_tabs.push_back(v_get_vector(vIndivs[i]));
	}

	for(size_t i = 0; i < v_tabs.size(); ++i)
	{
		for(size_t j = 0; j < v_tabs.size(); ++j)
		{
			if(i != j)
			{
				if(v_tabs[i] == v_tabs[j])
					return false;
			}
		}
	}

	return true;
}

bool CDiffEvol::b_population_is_equal(vector<Indiv>& vIndivs)
{
	vector<int> v_base;

	for(size_t i = 0; i < vIndivs[0].i_genotype_size; ++i)
	{
		if(i < MIN_GENE_OFFSET)
			v_base.push_back(vIndivs[0].pd_tab[i]);
		else
			v_base.push_back(vIndivs[0].pd_tab[i] * PRECISION);
	}

	for(size_t i = 1; i < vIndivs.size(); ++i)
	{
		for(int gene_offset = MIN_GENE_OFFSET; gene_offset < vIndivs[0].i_genotype_size; ++gene_offset)
			if(v_base[gene_offset] != (int) (vIndivs[i].pd_tab[gene_offset] * PRECISION))
				return false;
	}

	return true;
}

vector<double> CDiffEvol::v_get_vector(Indiv & ind)
{
	vector<double> v_res;

	for(int i = 0; i < ind.i_genotype_size; ++i)
		v_res.push_back(ind.pd_tab[i]);

	return v_res;
}

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
	*this = other;
}

CDiffEvol::Indiv::~Indiv()
{
	if(pd_tab != NULL)
		delete[] pd_tab;
}

double * CDiffEvol::Indiv::operator[](int iOffset)
{
	if(iOffset >= i_genotype_size)
		return NULL;

	return (pd_tab + iOffset);
}

void CDiffEvol::Indiv::operator=(const Indiv & other)
{
	i_genotype_size = other.i_genotype_size;
	pd_tab = new double[i_genotype_size];

	for(int i = 0; i < i_genotype_size; ++i)
		pd_tab[i] = other.pd_tab[i];
}


