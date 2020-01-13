#pragma once
#include "CMscnProblem.h"
#include "CRandom.h"
#include "COptimizer.h"

#define CSV_FILE_NAME "RS-Data.csv"

class CRandomSearch : public COptimizer 
{
public:
	 CMscnSolution cGenerateSolution(int iIterations, double &dResultQuality) ;
	 CMscnSolution cGenerateSolution(int iIterations, int iSeed, double &dResultQuality);

private:
	bool b_save_to_csv_file(vector<double> &vSolutionQualityHistory);
	virtual CMscnSolution cGenerateSolution(int iIterations, int iInitPopulation, int iSeed, double & dResultQuality) override;

};

