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

class CMscn
{
public:
	CMscn();

	virtual bool bSetDeliverers(int iAmount) = 0;
	virtual bool bSetFactories(int iAmount) = 0;
	virtual bool bSetMagazines(int iAmount) = 0;
	virtual bool bSetShops(int iAmount) = 0;

	int iGetDeliverers() { return i_deliverers; }
	int iGetFactories() { return i_factories; }
	int iGetMagazines() { return i_magazines; }
	int iGetShops() { return i_shops; }

	virtual bool bSaveToFile(string sFileName) = 0;
	virtual bool bLoadFromFile(string sFileName) = 0;
	virtual void vGenerateInstance(int iInstanceSeed) = 0;

protected:
	int i_deliverers;
	int i_factories;
	int i_magazines;
	int i_shops;

	void v_update_matrix_size(vector<vector<double>> &vMatrix, int iHeight, int iWidth);
	void v_update_table_size(vector<double> &vTable, int iSize);
	void v_update_range_size(vector<vector<vector<double>>> &vRange, int iHeight, int iWidth);
	bool b_set_value(vector<vector<double>> &vMatrix, int iRow, int iColumn, double dValue);
	bool b_set_value(vector<double> &vTable, int iPosition, double dValue);
	bool b_set_range_value(vector<vector<vector<double>>> &vRange, int iRow, int iColumn, double dMin, double dMax);
	bool b_set_global_range_value(vector<vector<vector<double>>> &vRange, double dMin, double dMax);
	vector<double> v_get_range(vector<vector<vector<double>>> &vRange, int iRow, int iColumn);
	bool b_validate_amount(vector<vector<double>> &vAmount, vector<double> &vLimits);
	bool b_validate_prod_cap(vector<vector<double>> &vAmount, vector<double> &vCapacity);
	bool b_write_tab(FILE *pfFile, string sTabName, vector<double> &vTab);
	bool b_write_matrix(FILE *pfFile, string sMatrixName, vector<vector<double>> &vMatrix);
	bool b_write_range(FILE *pfFile, string sRangeName, vector<vector<vector<double>>> &vRange);
	bool b_read_tab(FILE *pfFile, string sTabName, vector<double> &vTab);
	bool b_read_matrix(FILE *pfFile, string sMatrixName, vector<vector<double>> &vMatrix);
	bool b_read_range(FILE *pfFile, string sRangeName, vector<vector<vector<double>>> &vRange);
	bool b_check_range(vector<vector<vector<double>>> &vRange, int iRow, int iColumn, double dValue);
	bool b_set_matrix_values(vector<vector<double>> &vMatrix, vector<vector<vector<double>>> &vRange,
							 vector<double> &vSolution, int &iIndex, int &iErrorCode);

	void v_random_fill_tab(vector<double> &vTab, double dMinValue, double dMaxValue, CRandom &cRand);
	void v_random_fill_matrix(vector<vector<double>> &vMatrix, double dMinValue, double dMaxValue, CRandom &cRand);
};
