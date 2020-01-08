#pragma once
#include "CMscnProblem.h"
#include "CRandomSearch.h"

#define MIN_POPULATION 10
#define MIN_GENE_OFFSET 4
#define CROSS_PROBABILITY 0.75
#define DIFF_WEIGHT 1
#define PRECISION 1E4
#define ITERATION_INTERVAL 50
#define MAX_ITERATION_LIMIT 1E42

class CDiffEvol
{
public:
	CDiffEvol() { pc_problem = NULL; }
	CDiffEvol(CMscnProblem &cProblem) { pc_problem = &cProblem; }

	void vSetInstance(CMscnProblem &cProblem) { pc_problem = &cProblem; }
	double dGenerateSolution(int iFitnessCalls, int iInitPopulation, vector<double> &vSolution);
	double dGenerateSolution(int iFitnessCalls, int iInitPopulation, vector<double> &vSolution, int iSeed);
	double dGenerateSolution(int iFitnessCalls, int iInitPopulation, int iSeed);

private:
	struct Indiv
	{
		Indiv(int iGenotypeSize);
		Indiv(vector<double> &vSolution);
		Indiv(const Indiv &other);
		~Indiv();
		void operator=(const Indiv &other);
		void v_copy(const Indiv &other);
		vector<double> v_vector();

		double *pd_tab;
		int i_genotype_size;
	};

	CMscnProblem *pc_problem;

	bool b_validate_genotype(Indiv &ind, int iErrCode);
	bool b_indivs_are_different(vector<Indiv*> &vIndivs);
	bool b_indivs_are_equal(vector<Indiv*> &vIndivs);	
};

