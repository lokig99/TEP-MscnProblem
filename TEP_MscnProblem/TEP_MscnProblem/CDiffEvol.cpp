#include "pch.h"
#include "CDiffEvol.h"


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
	vector<Indiv*> v_population;
	vector<double> v_best_solution;
	vector<double> v_tmp;
	double d_best_quality;
	int i_err_code = 0;
	int i_fit_calls = 0;

	//initialize population
	for(int i = 0; i < iInitPopulation; ++i)
	{
		c_rs.dGenerateSolution(1, v_tmp, rand());
		v_population.push_back(new Indiv(v_tmp));
	}

	while(i_fit_calls < iFitnessCalls)
	{
		for(size_t i = 0; i < v_population.size(); ++i)
		{
			Indiv *p_ind = v_population[i];
			Indiv *p_base = v_population[c_rand.iRangeClosedLeft(0, v_population.size())];
			Indiv *p_add0 = v_population[c_rand.iRangeClosedLeft(0, v_population.size())];
			Indiv *p_add1 = v_population[c_rand.iRangeClosedLeft(0, v_population.size())];
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

						if(p_ind_new->pd_tab[gene_offset] < 0.0)
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
			}
			
			if(i_fit_calls % FITNESS_INTERVAL == 0)
				if(b_indivs_are_equal(v_population))
					i_fit_calls = iFitnessCalls;
		}
	}

	//find best solution
	v_best_solution = v_population[0]->v_vector();
	d_best_quality = pc_problem->dGetQuality(v_best_solution, i_err_code);

	for(size_t i = 1; i < v_population.size(); ++i)
	{
		v_tmp = v_population[i]->v_vector();
		double d_quality = pc_problem->dGetQuality(v_tmp, i_err_code);

		if(d_quality > d_best_quality)
		{
			v_best_solution = v_tmp;
			d_best_quality = d_quality;
		}
	}

	//clear population
	for(vector<Indiv*>::iterator it = v_population.begin(); it != v_population.end(); ++it)
		delete *it;
	v_population.clear();

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
	vector<double> v_tmp = ind.v_vector();
	return pc_problem->bConstraintsSatisfied(v_tmp, iErrCode);
}

bool CDiffEvol::b_indivs_are_different(vector<Indiv*> &vIndivs)
{
	for(size_t i = 0; i < vIndivs.size(); ++i)
		for(size_t j = i + 1; j < vIndivs.size(); ++j)
			if(vIndivs[i] == vIndivs[j])
				return false;

	return true;
}

bool CDiffEvol::b_indivs_are_equal(vector<Indiv*>& vIndivs)
{
	if(vIndivs.size() < 2)
		return true;

	vector<int> v_base;
	for(size_t i = 0; i < vIndivs[0]->i_genotype_size; ++i)
	{
		if(i < MIN_GENE_OFFSET)
			v_base.push_back(vIndivs[0]->pd_tab[i]);
		else
			v_base.push_back(vIndivs[0]->pd_tab[i] * PRECISION);
	}

	//check random indiv
	CRandom c_rand;
	int i_rand_index = c_rand.iRangeClosedLeft(1, vIndivs.size());
	for(int gene_offset = MIN_GENE_OFFSET; gene_offset < vIndivs[0]->i_genotype_size; ++gene_offset)
		if(v_base[gene_offset] != (int) (vIndivs[i_rand_index]->pd_tab[gene_offset] * PRECISION))
			return false;

	//if random indiv is equal to base continue
	for(size_t i = 1; i < vIndivs.size(); ++i)
	{
		if(i != i_rand_index)
			for(int gene_offset = MIN_GENE_OFFSET; gene_offset < vIndivs[0]->i_genotype_size; ++gene_offset)
				if(v_base[gene_offset] != (int) (vIndivs[i]->pd_tab[gene_offset] * PRECISION))
					return false;
	}

	return true;
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


