#pragma once
#include "CMscn.h"
#include "CMscnSolution.h"

class CMscnProblem : public CMscn
{
public:
	friend class CRandomSearch;
	friend class CDiffEvol;
	friend class CMscnSolution;

	bool bSetDeliverers(int iAmount);
	bool bSetFactories(int iAmount);
	bool bSetMagazines(int iAmount);
	bool bSetShops(int iAmount);
	bool bSetValueCD(int iRow, int iColumn, double dValue);
	bool bSetValueCF(int iRow, int iColumn, double dValue);
	bool bSetValueCM(int iRow, int iColumn, double dValue);
	bool bSetValueSD(int iPosition, double dValue);
	bool bSetValueSF(int iPosition, double dValue);
	bool bSetValueSM(int iPosition, double dValue);
	bool bSetValueSS(int iPosition, double dValue);
	bool bSetValueUD(int iPosition, double dValue);
	bool bSetValueUF(int iPosition, double dValue);
	bool bSetValueUM(int iPosition, double dValue);
	bool bSetValueP(int iPosition, double dValue);
	bool bSetRangeXD(int iRow, int iColumn, double dMin, double dMax);
	bool bSetRangeXF(int iRow, int iColumn, double dMin, double dMax);
	bool bSetRangeXM(int iRow, int iColumn, double dMin, double dMax);
	bool bSetGlobalRangeXD(double dMin, double dMax);
	bool bSetGlobalRangeXF(double dMin, double dMax);
	bool bSetGlobalRangeXM(double dMin, double dMax);
	
	vector<double> vGetRangeXD(int iRow, int iColumn);
	vector<double> vGetRangeXF(int iRow, int iColumn);
	vector<double> vGetRangeXM(int iRow, int iColumn);

	double dGetQuality(CMscnSolution &cSolution, int &iErrorCode);
	bool bConstraintsSatisfied(CMscnSolution &cSolution, int &iErrorCode);

	bool bSaveToFile(string sFileName);
	bool bLoadFromFile(string sFileName);

	void vGenerateInstance(int iInstanceSeed);
	
private:
	CMscnSolution *pc_solution;

	vector<vector<double>> v_costs_cd;
	vector<vector<double>> v_costs_cf;
	vector<vector<double>> v_costs_cm;
	
	vector<vector<vector<double>>> v_minmax_xd;
	vector<vector<vector<double>>> v_minmax_xf;
	vector<vector<vector<double>>> v_minmax_xm;

	vector<double> v_contract_ud;
	vector<double> v_contract_uf;
	vector<double> v_contract_um;
	vector<double> v_production_cap_sd;
	vector<double> v_production_cap_sf;
	vector<double> v_capacity_sm;
	vector<double> v_need_ss;
	vector<double> v_income_p;

	bool b_validate_prod_cap_sd();
	bool b_validate_prod_cap_sf();
	bool b_validate_amount_sm();
	bool b_validate_amount_ss();
	bool b_validate_amount_xd_xf();
	bool b_validate_amount_xf_xm();
	
	bool b_apply_solution(CMscnSolution &cSolution, int &iErrorCode);

	double d_calculate_quality();
	double d_calculate_subtotal_costs(vector<vector<double>> &vCosts, vector<vector<double>> &vAmount);
	double d_calculate_subtotal_contract(vector<double> &vContract, vector<vector<double>> &vAmount);
	double d_calculate_total_income(vector<double> &vIncome, vector<vector<double>> &vAmount);
	int i_has_contract(vector<vector<double>> &vMatrix, int iInput, int iOutput);

	void v_random_fill_tab(vector<double> &vTab, double dMinValue, double dMaxValue, CRandom &cRand);
	void v_random_fill_matrix(vector<vector<double>> &vMatrix, double dMinValue, double dMaxValue, CRandom &cRand);
};

