#include "pch.h"
#include "CMscnSolution.h"
#include <iostream>

CMscnSolution::CMscnSolution()
{
	i_deliverers = 0;
	i_factories = 0;
	i_magazines = 0;
	i_shops = 0;

	pc_problem = NULL;

	pv_amount_xd = new vector<vector<double>>;
	pv_amount_xf = new vector<vector<double>>;
	pv_amount_xm = new vector<vector<double>>;
}

CMscnSolution::CMscnSolution(const CMscnSolution & cOther)
{
	pv_amount_xd = new vector<vector<double>>;
	pv_amount_xf = new vector<vector<double>>;
	pv_amount_xm = new vector<vector<double>>;

	i_deliverers = cOther.i_deliverers;
	i_factories = cOther.i_factories;
	i_magazines = cOther.i_magazines;
	i_shops = cOther.i_shops;

	pc_problem = NULL;

	v_copy_matrix(*pv_amount_xd, *cOther.pv_amount_xd);
	v_copy_matrix(*pv_amount_xf, *cOther.pv_amount_xf);
	v_copy_matrix(*pv_amount_xm, *cOther.pv_amount_xm);
}

CMscnSolution::~CMscnSolution()
{
	delete pv_amount_xd;
	delete pv_amount_xf;
	delete pv_amount_xm;
}

void CMscnSolution::vSetProblem(CMscnProblem & cProblem)
{
	pc_problem = &cProblem;
	bSetDeliverers(pc_problem->iGetDeliverers());
	bSetFactories(pc_problem->iGetFactories());
	bSetMagazines(pc_problem->iGetMagazines());
	bSetShops(pc_problem->iGetShops());
}

void CMscnSolution::vTranferDataTo(CMscnSolution & cOther)
{
	delete cOther.pv_amount_xd;
	delete cOther.pv_amount_xf;
	delete cOther.pv_amount_xm;

	cOther.i_deliverers = i_deliverers;
	cOther.i_factories = i_factories;
	cOther.i_magazines = i_magazines;
	cOther.i_shops = i_shops;
		
	cOther.pv_amount_xd = pv_amount_xd;
	cOther.pv_amount_xf = pv_amount_xf;
	cOther.pv_amount_xm = pv_amount_xm;

	pv_amount_xd = new vector<vector<double>>;
	pv_amount_xf = new vector<vector<double>>;
	pv_amount_xm = new vector<vector<double>>;

	bSetDeliverers(iGetDeliverers());
	bSetFactories(iGetFactories());
	bSetMagazines(iGetMagazines());
	bSetShops(iGetShops());
}

bool CMscnSolution::bSetDeliverers(int iAmount)
{
	if(iAmount > 0)
	{
		i_deliverers = iAmount;
		v_update_matrix_size(*pv_amount_xd, i_deliverers, i_factories);
		return true;
	}

	return false;
}

bool CMscnSolution::bSetFactories(int iAmount)
{
	if(iAmount > 0)
	{
		i_factories = iAmount;
		v_update_matrix_size(*pv_amount_xd, i_deliverers, i_factories);
		v_update_matrix_size(*pv_amount_xf, i_factories, i_magazines);
		return true;
	}

	return false;
}

bool CMscnSolution::bSetMagazines(int iAmount)
{
	if(iAmount > 0)
	{
		i_magazines = iAmount;
		v_update_matrix_size(*pv_amount_xf, i_factories, i_magazines);
		v_update_matrix_size(*pv_amount_xm, i_magazines, i_shops);
		return true;
	}

	return false;
}

bool CMscnSolution::bSetShops(int iAmount)
{
	if(iAmount > 0)
	{
		i_shops = iAmount;
		v_update_matrix_size(*pv_amount_xm, i_magazines, i_shops);
		return true;
	}

	return false;
}

bool CMscnSolution::bSaveToFile(string sFileName)
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

		//write xd
		if(!b_write_matrix(pf_file, DEF_XD, *pv_amount_xd))
			return false;

		//write xf
		if(!b_write_matrix(pf_file, DEF_XF, *pv_amount_xf))
			return false;

		//write xm
		if(!b_write_matrix(pf_file, DEF_XM, *pv_amount_xm))
			return false;

		fclose(pf_file);
		return true;
	}

	return false;
}

bool CMscnSolution::bLoadFromFile(string sFileName)
{
	FILE *pf_file = fopen((sFileName + FILE_FORMAT).c_str(), "r");

	if(pf_file != NULL)
	{
		//read D, F, M, S
		int i_input;
		float f_input;

		fscanf(pf_file, DEF_D INSTANCE_WRITE_FORMAT, &i_input);
		bSetDeliverers(i_input);

		fscanf(pf_file, DEF_F INSTANCE_WRITE_FORMAT, &i_input);
		bSetFactories(i_input);

		fscanf(pf_file, DEF_M INSTANCE_WRITE_FORMAT, &i_input);
		bSetMagazines(i_input);

		fscanf(pf_file, DEF_S INSTANCE_WRITE_FORMAT, &i_input);
		bSetShops(i_input);

		//read xd
		if(!b_read_matrix(pf_file, DEF_XD, *pv_amount_xd))
			return false;

		//read xf 
		if(!b_read_matrix(pf_file, DEF_XF, *pv_amount_xf))
			return false;

		//read xm 
		if(!b_read_matrix(pf_file, DEF_XM, *pv_amount_xm))
			return false;

		fclose(pf_file);
		return true;
	}

	return false;
}

void CMscnSolution::vGenerateInstance(int iInstanceSeed)
{
	int i_err_code;
	if(pc_problem == NULL)
		return;

	CRandom c_rand;
	if(iInstanceSeed == 0)
		c_rand.vResetGlobalSeed();
	else
		c_rand.vSetGlobalSeed(iInstanceSeed);

	bool b_is_filled = false;

	while(!pc_problem->bConstraintsSatisfied(*this, i_err_code) || !b_is_filled)
	{
		b_is_filled = false;

		//fill xd
		while(!b_is_filled || !pc_problem->b_validate_prod_cap_sd())
		{
			b_fill_safe_xd();
			b_is_filled = true;
		}

		//fill xf
		b_is_filled = false;
		while(!b_is_filled || !pc_problem->b_validate_prod_cap_sf()
			  || !pc_problem->b_validate_amount_xd_xf())
		{
			b_fill_safe_xf();
			b_is_filled = true;
		}

		//fill xm
		b_is_filled = false;
		while(!b_is_filled || !pc_problem->b_validate_amount_sm()
			  || !pc_problem->b_validate_amount_ss() || !pc_problem->b_validate_amount_xf_xm())
		{
			b_fill_safe_xm();
			b_is_filled = true;
		}
	}
}

bool CMscnSolution::bSetValueXD(int iRow, int iColumn, double dValue)
{
	return b_set_value(*pv_amount_xd, iRow, iColumn, dValue);
}

bool CMscnSolution::bSetValueXF(int iRow, int iColumn, double dValue)
{
	return b_set_value(*pv_amount_xf, iRow, iColumn, dValue);
}

bool CMscnSolution::bSetValueXM(int iRow, int iColumn, double dValue)
{
	return b_set_value(*pv_amount_xm, iRow, iColumn, dValue);
}

vector<double> CMscnSolution::vGetVector()
{
	vector<double> v_tmp;

	v_tmp.push_back(i_deliverers);
	v_tmp.push_back(i_factories);
	v_tmp.push_back(i_magazines);
	v_tmp.push_back(i_shops);

	//push xd matrix
	for(size_t i = 0; i < pv_amount_xd->size(); ++i)
	{
		for(size_t j = 0; j < (*pv_amount_xd)[0].size(); ++j)
		{
			v_tmp.push_back((*pv_amount_xd)[i][j]);
		}
	}

	//push xf matrix
	for(size_t i = 0; i < pv_amount_xf->size(); ++i)
	{
		for(size_t j = 0; j < (*pv_amount_xf)[0].size(); ++j)
		{
			v_tmp.push_back((*pv_amount_xf)[i][j]);
		}
	}

	//push xm matrix
	for(size_t i = 0; i < pv_amount_xm->size(); ++i)
	{
		for(size_t j = 0; j < (*pv_amount_xm)[0].size(); ++j)
		{
			v_tmp.push_back((*pv_amount_xm)[i][j]);
		}
	}

	return v_tmp;
}

void CMscnSolution::operator=(const CMscnSolution & cOther)
{
	this->~CMscnSolution();
	pv_amount_xd = new vector<vector<double>>;
	pv_amount_xf = new vector<vector<double>>;
	pv_amount_xm = new vector<vector<double>>;

	i_deliverers = cOther.i_deliverers;
	i_factories = cOther.i_factories;
	i_magazines = cOther.i_magazines;
	i_shops = cOther.i_shops;

	pc_problem = NULL;

	v_copy_matrix(*pv_amount_xd, *cOther.pv_amount_xd);
	v_copy_matrix(*pv_amount_xf, *cOther.pv_amount_xf);
	v_copy_matrix(*pv_amount_xm, *cOther.pv_amount_xm);
}

double CMscnSolution::d_resources_amount(vector<vector<double>> &vMatrix, int iPosition)
{
	double d_res = 0.0;

	if(iPosition >= vMatrix[0].size())
		return d_res;

	for(size_t i = 0; i < vMatrix.size(); ++i)
		d_res += vMatrix[i][iPosition];

	return d_res;
}

//fills xd matrix with random values which satisfy constraints of CMscnProblem
bool CMscnSolution::b_fill_safe_xd()
{
	if(pc_problem == NULL)
		return false;

	CRandom c_rand;
	vector<vector<vector<double>>> v_range = pc_problem->v_minmax_xd;

	for(size_t i = 0; i < pv_amount_xd->size(); ++i)
	{
		double d_deliverers_prod_cap_left = pc_problem->v_production_cap_sd[i];

		for(size_t j = 0; j < (*pv_amount_xd)[0].size(); ++j)
		{
			(*pv_amount_xd)[i][j] =
				c_rand.dRange(v_range[i][j][0], std::min(v_range[i][j][1],
							  std::min(d_deliverers_prod_cap_left, pc_problem->v_production_cap_sf[j])));

			if((*pv_amount_xd)[i][j] < ROUND_TO_ZERO_BELOW)
				(*pv_amount_xd)[i][j] = 0.0;

			d_deliverers_prod_cap_left -= (*pv_amount_xd)[i][j];
		}
	}

	return true;
}

//fills xf matrix with random values which satisfy constraints of CMscnProblem
bool CMscnSolution::b_fill_safe_xf()
{
	if(pc_problem == NULL)
		return false;

	CRandom c_rand;
	vector<vector<vector<double>>> v_range = pc_problem->v_minmax_xf;

	for(size_t i = 0; i < pv_amount_xf->size(); ++i)
	{
		double d_products_left = d_resources_amount(*pv_amount_xd, i);
		double d_factory_prod_cap_left = std::min(pc_problem->v_production_cap_sf[i], d_products_left);

		for(size_t j = 0; j < (*pv_amount_xf)[0].size(); ++j)
		{
			(*pv_amount_xf)[i][j] =
				c_rand.dRange(v_range[i][j][0],
							  std::min(v_range[i][j][1],
							  std::min(d_factory_prod_cap_left, pc_problem->v_capacity_sm[j])));

			if((*pv_amount_xf)[i][j] < ROUND_TO_ZERO_BELOW)
				(*pv_amount_xf)[i][j] = 0.0;

			d_factory_prod_cap_left -= (*pv_amount_xf)[i][j];
		}
	}

	return true;
}

//fills xm matrix with random values which satisfy constraints of CMscnProblem
bool CMscnSolution::b_fill_safe_xm()
{
	if(pc_problem == NULL)
		return false;

	CRandom c_rand;
	vector<vector<vector<double>>> v_range = pc_problem->v_minmax_xm;

	for(size_t i = 0; i < pv_amount_xm->size(); ++i)
	{
		double d_products_left = d_resources_amount(*pv_amount_xf, i);
		double d_magazine_cap_left = std::min(pc_problem->v_capacity_sm[i], d_products_left);

		for(size_t j = 0; j < (*pv_amount_xm)[0].size(); ++j)
		{
			(*pv_amount_xm)[i][j] =
				c_rand.dRange(v_range[i][j][0],
							  std::min(v_range[i][j][1],
							  std::min(d_magazine_cap_left, pc_problem->v_need_ss[j])));

			if((*pv_amount_xm)[i][j] < ROUND_TO_ZERO_BELOW)
				(*pv_amount_xm)[i][j] = 0.0;

			d_magazine_cap_left -= (*pv_amount_xm)[i][j];
		}
	}

	return true;
}

void CMscnSolution::v_copy_matrix(vector<vector<double>>& vTarget, vector<vector<double>>& vSource)
{
	if(vSource.empty())
		return;

	vTarget.clear();

	for(size_t i = 0; i < vSource.size(); ++i)
	{
		vTarget.push_back(vector<double>());

		for(size_t j = 0; j < vSource[0].size(); ++j)
		{
			vTarget[i].push_back(vSource[i][j]);
		}
	}
}



