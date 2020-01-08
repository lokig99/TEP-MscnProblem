#pragma once
#include <vector>
#include <string>
#include "CRandom.h"

//ERROR CODES
#define INVALID_PROD_CAP_SD 1
#define INVALID_PROD_CAP_SF 2
#define INVALID_AMOUNT_SM 3
#define INVALID_AMOUNT_SS 4
#define INVALID_AMOUNT_XD_XF 5
#define INVALID_AMOUNT_XF_XM 6
#define NEGATIVE_NUMBER_ERR 7
#define INCOMPATIBLE_INSTANCE_NUMBER 8
#define INVALID_SOLUTION_FORMAT 9
#define VALUE_OUT_OF_RANGE 10

//CONSTANTS
#define DEF_D "D"
#define DEF_F "F"
#define DEF_M "M"
#define DEF_S "S"
#define DEF_CD "cd"
#define DEF_CF "cf"
#define DEF_CM "cm"
#define DEF_SD "sd"
#define DEF_SF "sf"
#define DEF_SM "sm"
#define DEF_SS "ss"
#define DEF_UD "ud"
#define DEF_UF "uf"
#define DEF_UM "um"
#define DEF_P "p"
#define DEF_XD "xd"
#define DEF_XF "xf"
#define DEF_XM "xm"
#define DEF_RANGE_XD "xdminmax"
#define DEF_RANGE_XF "xfminmax"
#define DEF_RANGE_XM "xmminmax"
#define INSTANCE_WRITE_FORMAT " %d\n"
#define TABLE_WRITE_FORMAT "(%d) %g\n"
#define MATRIX_WRITE_FORMAT "(%d)(%d) %g\n"
#define RANGE_WRITE_FORMAT "(%d)(%d) %g %g\n"
#define INSTANCE_TYPE_AMOUNT 4
#define FILE_FORMAT ".txt"

//RANDOMLY GENERATED INSTANCE - RANGES
#define RAND_CD_MIN 1
#define RAND_CD_MAX 10

#define RAND_CF_MIN 1
#define RAND_CF_MAX 10

#define RAND_CM_MIN 1
#define RAND_CM_MAX 10

#define RAND_SD_MIN 50
#define RAND_SD_MAX 200

#define RAND_SF_MIN 50
#define RAND_SF_MAX 200

#define RAND_SM_MIN 50
#define RAND_SM_MAX 200

#define RAND_SS_MIN 50
#define RAND_SS_MAX 200

#define RAND_UD_MIN 1
#define RAND_UD_MAX 100

#define RAND_UF_MIN 1
#define RAND_UF_MAX 100

#define RAND_UM_MIN 1
#define RAND_UM_MAX 100

#define RAND_P_MIN 10
#define RAND_P_MAX 100


using std::vector;
using std::string;

vector<double> vLoadSolutionFromFile(string sFileName);

class CMscnProblem
{
public:
	CMscnProblem();

	friend class CRandomSearch;
	friend class CDiffEvol;

	bool bSetDeliverers(int iAmount);
	bool bSetFactiories(int iAmount);
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

	double dGetQuality(vector<double> &vSolution, int &iErrorCode);
	bool bConstraintsSatisfied(vector<double> &vSolution, int &iErrorCode);

	bool bSaveToFile(string sFileName);
	bool bLoadFromFile(string sFileName);
	bool bCreateSolutionFile(string sFileName);
	vector<double> vGetSolutionVector();

	void vGenerateInstance(int iInstanceSeed);
	
private:
	int i_deliverers;
	int i_factories;
	int i_magazines;
	int i_shops;

	vector<vector<double>> v_costs_cd;
	vector<vector<double>> v_costs_cf;
	vector<vector<double>> v_costs_cm;
	vector<vector<double>> v_amount_xd;
	vector<vector<double>> v_amount_xf;
	vector<vector<double>> v_amount_xm;
	
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

	void v_update_matrix_size(vector<vector<double>> &vMatrix, int iHeight, int iWidth);
	void v_update_table_size(vector<double> &vTable, int iSize);
	void v_update_range_size(vector<vector<vector<double>>> &vRange, int iHeight, int iWidth);
	bool b_set_value(vector<vector<double>> &vMatrix, int iRow, int iColumn, double dValue);
	bool b_set_value(vector<double> &vTable, int iPosition, double dValue);
	bool b_set_range_value(vector<vector<vector<double>>> &vRange, int iRow, int iColumn, double dMin, double dMax);
	bool b_set_global_range_value(vector<vector<vector<double>>> &vRange, double dMin, double dMax);
	vector<double> v_get_range(vector<vector<vector<double>>> &vRange, int iRow, int iColumn);
	
	bool b_validate_prod_cap_sd();
	bool b_validate_prod_cap_sf();
	bool b_validate_amount_sm();
	bool b_validate_amount_ss();
	bool b_validate_amount_xd_xf();
	bool b_validate_amount_xf_xm();
	bool b_validate_amount(vector<vector<double>> &vAmount, vector<double> &vLimits);
	bool b_validate_prod_cap(vector<vector<double>> &vAmount, vector<double> &vCapacity);
	
	bool b_write_tab(FILE *pfFile, string sTabName, vector<double> &vTab);
	bool b_write_matrix(FILE *pfFile, string sMatrixName, vector<vector<double>> &vMatrix);
	bool b_write_range(FILE *pfFile, string sRangeName, vector<vector<vector<double>>> &vRange);
	bool b_read_tab(FILE *pfFile, string sTabName, vector<double> &vTab);
	bool b_read_matrix(FILE *pfFile, string sMatrixName, vector<vector<double>> &vMatrix);
	bool b_read_range(FILE *pfFile, string sRangeName, vector<vector<vector<double>>> &vRange);

	bool b_apply_solution(vector<double> &vSolution, int &iErrorCode);
	bool b_check_range(vector<vector<vector<double>>> &vRange, int iRow, int iColumn, double dValue);
	bool b_set_matrix_values(vector<vector<double>> &vMatrix, vector<vector<vector<double>>> &vRange, 
							 vector<double> &vSolution, int &iIndex, int &iErrorCode);

	double d_calculate_quality();
	double d_calculate_subtotal_costs(vector<vector<double>> &vCosts, vector<vector<double>> &vAmount);
	double d_calculate_subtotal_contract(vector<double> &vContract, vector<vector<double>> &vAmount);
	double d_calculate_total_income(vector<double> &vIncome, vector<vector<double>> &vAmount);
	int i_has_contract(vector<vector<double>> &vMatrix, int iInput, int iOutput);

	void v_random_fill_tab(vector<double> &vTab, double dMinValue, double dMaxValue, CRandom &cRand);
	void v_random_fill_matrix(vector<vector<double>> &vMatrix, double dMinValue, double dMaxValue, CRandom &cRand);
};

