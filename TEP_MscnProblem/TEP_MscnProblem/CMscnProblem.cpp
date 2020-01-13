#include "pch.h"
#include "CMscnProblem.h"


bool CMscnProblem::bSetDeliverers(int iAmount)
{
	if(iAmount > 0)
	{
		i_deliverers = iAmount;
		v_update_matrix_size(v_costs_cd, i_deliverers, i_factories);
		v_update_range_size(v_minmax_xd, i_deliverers, i_factories);
		v_update_table_size(v_contract_ud, i_deliverers);
		v_update_table_size(v_production_cap_sd, i_deliverers);
		return true;
	}

	return false;
}

bool CMscnProblem::bSetFactories(int iAmount)
{
	if(iAmount > 0)
	{
		i_factories = iAmount;
		v_update_matrix_size(v_costs_cd, i_deliverers, i_factories);
		v_update_matrix_size(v_costs_cf, i_factories, i_magazines);
		v_update_range_size(v_minmax_xd, i_deliverers, i_factories);
		v_update_range_size(v_minmax_xf, i_factories, i_magazines);
		v_update_table_size(v_contract_uf, i_factories);
		v_update_table_size(v_production_cap_sf, i_factories);
		return true;
	}

	return false;
}

bool CMscnProblem::bSetMagazines(int iAmount)
{
	if(iAmount > 0)
	{
		i_magazines = iAmount;
		v_update_matrix_size(v_costs_cf, i_factories, i_magazines);
		v_update_matrix_size(v_costs_cm, i_magazines, i_shops);
		v_update_range_size(v_minmax_xf, i_factories, i_magazines);
		v_update_table_size(v_contract_um, i_magazines);
		v_update_table_size(v_capacity_sm, i_magazines);
		return true;
	}

	return false;
}

bool CMscnProblem::bSetShops(int iAmount)
{
	if(iAmount > 0)
	{
		i_shops = iAmount;
		v_update_matrix_size(v_costs_cm, i_magazines, i_shops);
		v_update_range_size(v_minmax_xm, i_magazines, i_shops);
		v_update_table_size(v_income_p, i_shops);
		v_update_table_size(v_need_ss, i_shops);
		return true;
	}

	return false;
}

bool CMscnProblem::bSetValueCD(int iRow, int iColumn, double dValue)
{
	return b_set_value(v_costs_cd, iRow, iColumn, dValue);
}

bool CMscnProblem::bSetValueCF(int iRow, int iColumn, double dValue)
{
	return b_set_value(v_costs_cf, iRow, iColumn, dValue);
}

bool CMscnProblem::bSetValueCM(int iRow, int iColumn, double dValue)
{
	return b_set_value(v_costs_cm, iRow, iColumn, dValue);
}

bool CMscnProblem::bSetValueSD(int iPosition, double dValue)
{
	return b_set_value(v_production_cap_sd, iPosition, dValue);
}

bool CMscnProblem::bSetValueSF(int iPosition, double dValue)
{
	return b_set_value(v_production_cap_sf, iPosition, dValue);
}

bool CMscnProblem::bSetValueSM(int iPosition, double dValue)
{
	return b_set_value(v_capacity_sm, iPosition, dValue);
}

bool CMscnProblem::bSetValueSS(int iPosition, double dValue)
{
	return b_set_value(v_need_ss, iPosition, dValue);
}

bool CMscnProblem::bSetValueUD(int iPosition, double dValue)
{
	return b_set_value(v_contract_ud, iPosition, dValue);
}

bool CMscnProblem::bSetValueUF(int iPosition, double dValue)
{
	return b_set_value(v_contract_uf, iPosition, dValue);
}

bool CMscnProblem::bSetValueUM(int iPosition, double dValue)
{
	return b_set_value(v_contract_um, iPosition, dValue);
}

bool CMscnProblem::bSetValueP(int iPosition, double dValue)
{
	return b_set_value(v_income_p, iPosition, dValue);
}

bool CMscnProblem::bSetRangeXD(int iRow, int iColumn, double dMin, double dMax)
{
	return b_set_range_value(v_minmax_xd, iRow, iColumn, dMin, dMax);
}

bool CMscnProblem::bSetRangeXF(int iRow, int iColumn, double dMin, double dMax)
{
	return b_set_range_value(v_minmax_xf, iRow, iColumn, dMin, dMax);
}

bool CMscnProblem::bSetRangeXM(int iRow, int iColumn, double dMin, double dMax)
{
	return b_set_range_value(v_minmax_xm, iRow, iColumn, dMin, dMax);
}

bool CMscnProblem::bSetGlobalRangeXD(double dMin, double dMax)
{
	return b_set_global_range_value(v_minmax_xd, dMin, dMax);
}

bool CMscnProblem::bSetGlobalRangeXF(double dMin, double dMax)
{
	return b_set_global_range_value(v_minmax_xf, dMin, dMax);
}

bool CMscnProblem::bSetGlobalRangeXM(double dMin, double dMax)
{
	return b_set_global_range_value(v_minmax_xm, dMin, dMax);
}

vector<double> CMscnProblem::vGetRangeXD(int iRow, int iColumn)
{
	return v_get_range(v_minmax_xd, iRow, iColumn);
}

vector<double> CMscnProblem::vGetRangeXF(int iRow, int iColumn)
{
	return v_get_range(v_minmax_xf, iRow, iColumn);
}

vector<double> CMscnProblem::vGetRangeXM(int iRow, int iColumn)
{
	return v_get_range(v_minmax_xm, iRow, iColumn);
}

double CMscnProblem::dGetQuality(CMscnSolution &cSolution, int & iErrorCode)
{
	if(!bConstraintsSatisfied(cSolution, iErrorCode))
		return -NAN;
	
	return d_calculate_quality();
}

bool CMscnProblem::bConstraintsSatisfied(CMscnSolution &cSolution, int & iErrorCode)
{
	if(!b_apply_solution(cSolution, iErrorCode))
		return false;

	if(!b_validate_amount_sm())
	{
		iErrorCode = INVALID_AMOUNT_SM;
		return false;
	}

	if(!b_validate_amount_ss())
	{
		iErrorCode = INVALID_AMOUNT_SS;
		return false;
	}

	if(!b_validate_amount_xd_xf())
	{
		iErrorCode = INVALID_AMOUNT_XD_XF;
		return false;
	}

	if(!b_validate_amount_xf_xm())
	{
		iErrorCode = INVALID_AMOUNT_XF_XM;
		return false;
	}

	if(!b_validate_prod_cap_sd())
	{
		iErrorCode = INVALID_PROD_CAP_SD;
		return false;
	}

	if(!b_validate_prod_cap_sf())
	{
		iErrorCode = INVALID_PROD_CAP_SF;
		return false;
	}

	iErrorCode = 0;
	return true;
}

bool CMscnProblem::bSaveToFile(string sFileName)
{
	FILE *pf_file = fopen((sFileName + FILE_FORMAT).c_str(), "w");

	if(pf_file != NULL)
	{
		//write D, F, M, S
		if(fprintf(pf_file, DEF_D INSTANCE_WRITE_FORMAT, i_deliverers) < 0)
			return false;

		if(fprintf(pf_file, DEF_F INSTANCE_WRITE_FORMAT, i_factories) < 0)
			return false;

		if(fprintf(pf_file, DEF_M INSTANCE_WRITE_FORMAT, i_magazines) < 0)
			return false;

		if(fprintf(pf_file, DEF_S INSTANCE_WRITE_FORMAT, i_shops) < 0)
			return false;

		if(fprintf(pf_file, "\n") < 0)
			return false;

		//write sd
		if(!b_write_tab(pf_file, DEF_SD, v_production_cap_sd))
		   return false;

		//write sf
		if(!b_write_tab(pf_file, DEF_SF, v_production_cap_sf))
			return false;

		//write sm
		if(!b_write_tab(pf_file, DEF_SM, v_capacity_sm))
			return false;

		//write ss
		if(!b_write_tab(pf_file, DEF_SS, v_need_ss))
			return false;

		//write cd
		if(!b_write_matrix(pf_file, DEF_CD, v_costs_cd))
			return false;

		//write cf
		if(!b_write_matrix(pf_file, DEF_CF, v_costs_cf))
			return false;

		//write cm
		if(!b_write_matrix(pf_file, DEF_CM, v_costs_cm))
			return false;

		//write ud
		if(!b_write_tab(pf_file, DEF_UD, v_contract_ud))
			return false;

		//write uf
		if(!b_write_tab(pf_file, DEF_UF, v_contract_uf))
			return false;

		//write um
		if(!b_write_tab(pf_file, DEF_UM, v_contract_um))
			return false;

		//write p
		if(!b_write_tab(pf_file, DEF_P, v_income_p))
			return false;

		//write xdminmax
		if(!b_write_range(pf_file, DEF_RANGE_XD, v_minmax_xd))
			return false;

		//write xfminmax
		if(!b_write_range(pf_file, DEF_RANGE_XF, v_minmax_xf))
			return false;

		//write xmminmax
		if(!b_write_range(pf_file, DEF_RANGE_XM, v_minmax_xm))
			return false;

		fclose(pf_file);
		return true;
	}

	return false;
}

bool CMscnProblem::bLoadFromFile(string sFileName)
{
	FILE *pf_file = fopen((sFileName + FILE_FORMAT).c_str(), "r");

	if(pf_file != NULL)
	{
		//read D, F, M, S
		int i_input;

		fscanf(pf_file, DEF_D INSTANCE_WRITE_FORMAT, &i_input);
		if(!bSetDeliverers(i_input))
			return false;

		fscanf(pf_file, DEF_F INSTANCE_WRITE_FORMAT, &i_input);
		if(!bSetFactories(i_input))
			return false;

		fscanf(pf_file, DEF_M INSTANCE_WRITE_FORMAT, &i_input);
		if(!bSetMagazines(i_input))
			return false;

		fscanf(pf_file, DEF_S INSTANCE_WRITE_FORMAT, &i_input);
		if(!bSetShops(i_input))
			return false;

		//read sd
		if(!b_read_tab(pf_file, DEF_SD, v_production_cap_sd))
			return false;

		//read sf
		if(!b_read_tab(pf_file, DEF_SF, v_production_cap_sf))
			return false;

		//read sm
		if(!b_read_tab(pf_file, DEF_SM, v_capacity_sm))
			return false;

		//read ss
		if(!b_read_tab(pf_file, DEF_SS, v_need_ss))
			return false;

		//read cd 
		if(!b_read_matrix(pf_file, DEF_CD, v_costs_cd))
			return false;
		
		//read cf 
		if(!b_read_matrix(pf_file, DEF_CF, v_costs_cf))
			return false;

		//read cm 
		if(!b_read_matrix(pf_file, DEF_CM, v_costs_cm))
			return false;

		//read ud
		if(!b_read_tab(pf_file, DEF_UD, v_contract_ud))
			return false;

		//read uf
		if(!b_read_tab(pf_file, DEF_UF, v_contract_uf))
			return false;

		//read um
		if(!b_read_tab(pf_file, DEF_UM, v_contract_um))
			return false;

		//read p
		if(!b_read_tab(pf_file, DEF_P, v_income_p))
			return false;

		//read xdminmax 
		if(!b_read_range(pf_file, DEF_RANGE_XD, v_minmax_xd))
			return false;

		//read xfminmax 
		if(!b_read_range(pf_file, DEF_RANGE_XF, v_minmax_xf))
			return false;

		//read xmminmax 
		if(!b_read_range(pf_file, DEF_RANGE_XM, v_minmax_xm))
			return false;

		fclose(pf_file);
		return true;
	}

	return false;
}

void CMscnProblem::vGenerateInstance(int iInstanceSeed)
{
	CRandom c_rand;

	if(iInstanceSeed == 0)
		c_rand.vResetGlobalSeed();
	else
		c_rand.vSetGlobalSeed(iInstanceSeed);

	v_random_fill_tab(v_production_cap_sd, RAND_SD_MIN, RAND_SD_MAX, c_rand);
	v_random_fill_tab(v_production_cap_sf, RAND_SF_MIN, RAND_SF_MAX, c_rand);
	v_random_fill_tab(v_contract_ud, RAND_UD_MIN, RAND_UD_MAX, c_rand);
	v_random_fill_tab(v_contract_uf, RAND_UF_MIN, RAND_UF_MAX, c_rand);
	v_random_fill_tab(v_contract_um, RAND_UM_MIN, RAND_UM_MAX, c_rand);
	v_random_fill_tab(v_capacity_sm, RAND_SM_MIN, RAND_SM_MAX, c_rand);
	v_random_fill_tab(v_need_ss, RAND_SS_MIN, RAND_SS_MAX, c_rand);
	v_random_fill_tab(v_income_p, RAND_P_MIN, RAND_P_MAX, c_rand);

	v_random_fill_matrix(v_costs_cd, RAND_CD_MIN, RAND_CD_MAX, c_rand);
	v_random_fill_matrix(v_costs_cf, RAND_CF_MIN, RAND_CF_MAX, c_rand);
	v_random_fill_matrix(v_costs_cm, RAND_CM_MIN, RAND_CM_MAX, c_rand);
}

bool CMscnProblem::b_validate_prod_cap_sd()
{
	return b_validate_prod_cap(*pc_solution->pv_amount_xd, v_production_cap_sd);
}

bool CMscnProblem::b_validate_prod_cap_sf()
{
	return b_validate_prod_cap(*pc_solution->pv_amount_xf, v_production_cap_sf);
}

bool CMscnProblem::b_validate_amount_sm()
{
	return  b_validate_amount(*pc_solution->pv_amount_xm, v_capacity_sm);
}

bool CMscnProblem::b_validate_amount_ss()
{
	return  b_validate_amount(*pc_solution->pv_amount_xm, v_need_ss);
}

bool CMscnProblem::b_validate_amount_xd_xf()
{
	for(size_t f = 0; f < i_factories; ++f)
	{
		double d_resource_sum = 0.0;
		double d_prod_sum = 0.0;

		for(size_t d = 0; d < i_deliverers; ++d)
		{
			d_resource_sum += (*pc_solution->pv_amount_xd)[d][f];
		}

		for(size_t m = 0; m < i_magazines; ++m)
		{
			d_prod_sum += (*pc_solution->pv_amount_xf)[f][m];
		}

		if(d_resource_sum < d_prod_sum)
			return false;
	}

	return true;
}

bool CMscnProblem::b_validate_amount_xf_xm()
{
	for(size_t m = 0; m < i_magazines; ++m)
	{
		double d_prod_magazine_sum = 0.0;
		double d_prod_shop_sum = 0.0;

		for(size_t f = 0; f < i_factories; ++f)
		{
			d_prod_magazine_sum += (*pc_solution->pv_amount_xf)[f][m];
		}

		for(size_t s = 0; s < i_shops; ++s)
		{
			d_prod_shop_sum += (*pc_solution->pv_amount_xm)[m][s];
		}

		if(d_prod_magazine_sum < d_prod_shop_sum)
			return false;
	}

	return true;
}

bool CMscnProblem::b_apply_solution(CMscnSolution& cSolution, int &iErrorCode)
{
	bool b_solution_size_valid = cSolution.iGetDeliverers() == i_deliverers && cSolution.iGetFactories() == i_factories &&
		cSolution.iGetMagazines() == i_magazines && cSolution.iGetShops() == i_shops;

	if(b_solution_size_valid)
	{
		pc_solution = &cSolution;
		return true;
	}
			
	iErrorCode = INVALID_SOLUTION_FORMAT;
	return false;
}

double CMscnProblem::d_calculate_quality()
{
	//////////////  calculate costs  //////////////////

	double d_total_costs_K = d_calculate_subtotal_costs(v_costs_cd, *pc_solution->pv_amount_xd) +
		d_calculate_subtotal_costs(v_costs_cf, *pc_solution->pv_amount_xf) +
		d_calculate_subtotal_costs(v_costs_cm, *pc_solution->pv_amount_xm);

	////////////// calculate contracts costs /////////

	double d_total_costs_KU = d_calculate_subtotal_contract(v_contract_ud, *pc_solution->pv_amount_xd) +
		d_calculate_subtotal_contract(v_contract_uf, *pc_solution->pv_amount_xf) +
		d_calculate_subtotal_contract(v_contract_um, *pc_solution->pv_amount_xm);

	////////////// calculate income //////////////////

	double d_total_income = d_calculate_total_income(v_income_p, *pc_solution->pv_amount_xm);

	return d_total_income - d_total_costs_K - d_total_costs_KU;
}

double CMscnProblem::d_calculate_subtotal_costs(vector<vector<double>> &vCosts, vector<vector<double>> &vAmount)
{
	double d_result = 0.0;

	for(size_t i = 0; i < vAmount.size(); ++i)
	{
		for(size_t j = 0; j < vAmount[0].size(); ++j)
		{
			d_result += vCosts[i][j] * vAmount[i][j];
		}
	}

	return d_result;
}

double CMscnProblem::d_calculate_subtotal_contract(vector<double> &vContract, vector<vector<double>> &vAmount)
{
	double d_result = 0.0;
	
	for(size_t i = 0; i < vAmount.size(); ++i)
	{
		int i_contracts = 0;

		for(size_t j = 0; j < vAmount[0].size(); ++j)
			i_contracts += i_has_contract(vAmount, i, j);
		
		if(i_contracts > 0)
			d_result += vContract[i];	
	}

	return d_result;
}

double CMscnProblem::d_calculate_total_income(vector<double> &vIncome, vector<vector<double>> &vAmount)
{
	double d_result = 0.0;

	for(size_t i = 0; i < vAmount.size(); ++i)
	{
		for(size_t j = 0; j < vAmount[0].size(); ++j)
		{
			d_result += vIncome[j] * vAmount[i][j];
		}
	}

	return d_result;
}

int CMscnProblem::i_has_contract(vector<vector<double>> &vMatrix, int iInput, int iOutput)
{
	return (vMatrix[iInput][iOutput] > 0) ? 1 : 0;
}

void CMscnProblem::v_random_fill_tab(vector<double>& vTab, double dMinValue, double dMaxValue, CRandom & cRand)
{
	for(size_t i = 0; i < vTab.size(); ++i)
		b_set_value(vTab, i, cRand.dRange(dMinValue, dMaxValue));
}

void CMscnProblem::v_random_fill_matrix(vector<vector<double>>& vMatrix, double dMinValue, double dMaxValue, CRandom & cRand)
{
	for(size_t i = 0; i < vMatrix.size(); ++i)
	{
		for(size_t j = 0; j < vMatrix[0].size(); ++j)
		{
			b_set_value(vMatrix[i], j, cRand.dRange(dMinValue, dMaxValue));
		}
	}
}