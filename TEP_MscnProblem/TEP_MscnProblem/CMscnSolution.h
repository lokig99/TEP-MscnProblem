#pragma once
#include "CMscn.h"
#include "CMscnProblem.h"

#define ROUND_TO_ZERO_BELOW 0.1

class CMscnSolution : public CMscn
{
public:	
	CMscnSolution();
	CMscnSolution(const CMscnSolution &cOther);
	~CMscnSolution();

	friend class CMscnProblem;
	friend class CDiffEvol;

	void vSetProblem(CMscnProblem &cProblem);
	void vTranferDataTo(CMscnSolution &cOther);
	virtual bool bSetDeliverers(int iAmount) override;
	virtual bool bSetFactories(int iAmount) override;
	virtual bool bSetMagazines(int iAmount) override;
	virtual bool bSetShops(int iAmount) override;
	virtual bool bSaveToFile(string sFileName) override;
	virtual bool bLoadFromFile(string sFileName) override;
	virtual void vGenerateInstance(int iInstanceSeed) override;

	bool bSetValueXD(int iRow, int iColumn, double dValue);
	bool bSetValueXF(int iRow, int iColumn, double dValue);
	bool bSetValueXM(int iRow, int iColumn, double dValue);

	vector<double> vGetVector();

	void operator=(const CMscnSolution &cOther);

private:
	CMscnProblem *pc_problem;

	vector<vector<double>> *pv_amount_xd;
	vector<vector<double>> *pv_amount_xf;
	vector<vector<double>> *pv_amount_xm;

	double d_resources_amount(vector<vector<double>> &vMatrix, int iPosition);
	bool b_fill_safe_xd();
	bool b_fill_safe_xf();
	bool b_fill_safe_xm();
	void v_copy_matrix(vector<vector<double>> &vTarget, vector<vector<double>> &vSource);
};

