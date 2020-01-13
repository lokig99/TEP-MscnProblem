#pragma once
#include "CMscnProblem.h"
#include "CRandomSearch.h"
#include "COptimizer.h"

#define MIN_POPULATION 10
#define MIN_GENE_OFFSET 4
#define CROSS_PROBABILITY 0.75
#define DIFF_WEIGHT 1
#define PRECISION 1E-2
#define ITERATION_INTERVAL 10
#define CSV_FILE_NAME "DiffEvol-Recent"

class CDiffEvol : public COptimizer
{
public:
	virtual CMscnSolution cGenerateSolution(int iFitnessCalls, 
											int iInitPopulation, double &dResultQuality);
	virtual CMscnSolution cGenerateSolution(int iFitnessCalls, 
									int iInitPopulation, int iSeed, double &dResultQuality) override;

private:
	bool b_validate_genotype(CMscnSolution &cIndividual, const string &sMatrixName);
	bool b_indivs_are_different(vector<CMscnSolution*> &vIndivs);
	bool b_indivs_are_equal(vector<CMscnSolution*> &vIndivs);
	bool b_save_to_csv_file(vector<vector<double>*> *pvSolutionQualityHistory);
	int i_get_best_solution(vector<CMscnSolution*> &vIndivs, double &dQualityOutput);
	void v_mutate_genotype(vector<CMscnSolution*> &vBasePool, CMscnSolution &cMutatedIndiv, const string &sMatrixName);
};

