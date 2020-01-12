#include "pch.h"
#include "CMscn.h"


CMscn::CMscn()
{
	i_deliverers = 0;
	i_factories = 0;
	i_magazines = 0;
	i_shops = 0;
}

void CMscn::v_update_matrix_size(vector<vector<double>>& vMatrix, int iHeight, int iWidth)
{
	vector<vector<double>> v_tmp;

	for(size_t i = 0; i < iHeight; ++i)
	{
		vector<double> v_row;

		for(size_t j = 0; j < iWidth; ++j)
		{
			v_row.push_back(0.0);
		}

		v_tmp.push_back(v_row);
	}

	vMatrix = v_tmp;
}

void CMscn::v_update_table_size(vector<double>& vTable, int iSize)
{
	vector<double> v_tmp;

	for(size_t i = 0; i < iSize; ++i)
	{
		v_tmp.push_back(0.0);
	}

	vTable = v_tmp;
}

void CMscn::v_update_range_size(vector<vector<vector<double>>>& vRange, int iHeight, int iWidth)
{
	vector<vector<vector<double>>> v_tmp;

	for(size_t i = 0; i < iHeight; ++i)
	{
		vector<vector<double>> v_row;

		for(size_t j = 0; j < iWidth; ++j)
		{
			v_row.push_back({ 0, 0 });
		}

		v_tmp.push_back(v_row);
	}

	vRange = v_tmp;
}

bool CMscn::b_set_value(vector<vector<double>>& vMatrix, int iRow, int iColumn, double dValue)
{
	if((iRow >= 0 && iRow < vMatrix.size()) && (iColumn >= 0 && iColumn < vMatrix[0].size()) && dValue >= 0.0)
	{
		vMatrix[iRow][iColumn] = dValue;
		return true;
	}

	return false;
}

bool CMscn::b_set_value(vector<double>& vTable, int iPosition, double dValue)
{
	if(iPosition >= 0 && iPosition < vTable.size() && dValue >= 0.0)
	{
		vTable[iPosition] = dValue;
		return true;
	}

	return false;
}

bool CMscn::b_set_range_value(vector<vector<vector<double>>>& vRange, int iRow, int iColumn, double dMin, double dMax)
{
	if((iRow >= 0 && iRow < vRange.size()) && (iColumn >= 0 && iColumn < vRange[0].size()) && dMin >= 0.0 && dMax >= 0.0)
	{
		vRange[iRow][iColumn] = { dMin, dMax };
		return true;
	}

	return false;
}

bool CMscn::b_set_global_range_value(vector<vector<vector<double>>>& vRange, double dMin, double dMax)
{
	for(size_t i = 0; i < vRange.size(); ++i)
	{
		for(size_t j = 0; j < vRange[0].size(); ++j)
		{
			if(!b_set_range_value(vRange, i, j, dMin, dMax))
				return false;
		}
	}

	return true;
}

vector<double> CMscn::v_get_range(vector<vector<vector<double>>>& vRange, int iRow, int iColumn)
{
	if(iRow >= 0 && iRow < vRange.size() && iColumn >= 0 && iColumn < vRange[0].size())
		return vRange[iRow][iColumn];

	return vector<double>();
}

bool CMscn::b_validate_amount(vector<vector<double>>& vAmount, vector<double>& vLimits)
{
	for(size_t i = 0; i < vLimits.size(); ++i)
	{
		double d_sum = 0.0;

		for(size_t j = 0; j < vAmount.size(); ++j)
		{
			d_sum += vAmount[j][i];
		}

		if(d_sum > vLimits[i])
			return false;
	}

	return true;
}

bool CMscn::b_validate_prod_cap(vector<vector<double>>& vAmount, vector<double>& vCapacity)
{
	for(size_t i = 0; i < vCapacity.size(); ++i)
	{
		double d_sum = 0.0;

		for(size_t j = 0; j < vAmount[i].size(); ++j)
		{
			d_sum += vAmount[i][j];
		}

		if(d_sum > vCapacity[i])
			return false;
	}

	return true;
}

bool CMscn::b_write_tab(FILE * pfFile, string sTabName, vector<double>& vTab)
{
	for(size_t i = 0; i < vTab.size(); ++i)
	{
		if(fprintf(pfFile, (sTabName + TABLE_WRITE_FORMAT).c_str(), i, vTab[i]) < 0)
			return false;
	}

	if(fprintf(pfFile, "\n") < 0)
		return false;

	return true;
}

bool CMscn::b_write_matrix(FILE * pfFile, string sMatrixName, vector<vector<double>>& vMatrix)
{
	for(size_t i = 0; i < vMatrix.size(); ++i)
	{
		for(size_t j = 0; j < vMatrix[0].size(); ++j)
		{
			if(fprintf(pfFile, (sMatrixName + MATRIX_WRITE_FORMAT).c_str(), i, j, vMatrix[i][j]) < 0)
				return false;
		}
	}

	if(fprintf(pfFile, "\n") < 0)
		return false;

	return true;
}

bool CMscn::b_write_range(FILE * pfFile, string sRangeName, vector<vector<vector<double>>>& vRange)
{
	for(size_t i = 0; i < vRange.size(); ++i)
	{
		for(size_t j = 0; j < vRange[0].size(); ++j)
		{
			if(fprintf(pfFile, (sRangeName + RANGE_WRITE_FORMAT).c_str(), i, j, vRange[i][j][0], vRange[i][j][1]) < 0)
				return false;
		}
	}

	if(fprintf(pfFile, "\n") < 0)
		return false;

	return true;
}

bool CMscn::b_read_tab(FILE * pfFile, string sTabName, vector<double>& vTab)
{
	int i_row;

	for(size_t i = 0; i < vTab.size(); ++i)
	{
		float f_tmp;

		fscanf(pfFile, (sTabName + TABLE_WRITE_FORMAT).c_str(), &i_row, &f_tmp);

		if(i_row != i || !b_set_value(vTab, i, f_tmp))
			return false;
	}

	return true;
}

bool CMscn::b_read_matrix(FILE * pfFile, string sMatrixName, vector<vector<double>>& vMatrix)
{
	int i_row;
	int i_column;

	for(size_t i = 0; i < vMatrix.size(); ++i)
	{
		for(size_t j = 0; j < vMatrix[0].size(); ++j)
		{
			float f_tmp;
			fscanf(pfFile, (sMatrixName + MATRIX_WRITE_FORMAT).c_str(), &i_row, &i_column, &f_tmp);
			if(i_row != i || i_column != j || !b_set_value(vMatrix, i, j, f_tmp))
				return false;
		}
	}

	return true;
}

bool CMscn::b_read_range(FILE * pfFile, string sRangeName, vector<vector<vector<double>>>& vRange)
{
	int i_row;
	int i_column;

	for(size_t i = 0; i < vRange.size(); ++i)
	{
		for(size_t j = 0; j < vRange[0].size(); ++j)
		{
			float f_tmp_min;
			float f_tmp_max;

			fscanf(pfFile, (sRangeName + RANGE_WRITE_FORMAT).c_str(), &i_row, &i_column, &f_tmp_min, &f_tmp_max);
			if(i_row != i || i_column != j || !b_set_range_value(vRange, i, j, f_tmp_min, f_tmp_max))
				return false;
		}
	}

	return true;
}

bool CMscn::b_check_range(vector<vector<vector<double>>>& vRange, int iRow, int iColumn, double dValue)
{
	vector<double> v_range = v_get_range(vRange, iRow, iColumn);

	return dValue >= v_range[0] && dValue <= v_range[1];
}

bool CMscn::b_set_matrix_values(vector<vector<double>>& vMatrix, vector<vector<vector<double>>> &vRange,
									   vector<double> &vSolution, int & iIndex, int &iErrorCode)
{
	for(size_t i = 0; i < vMatrix.size(); ++i)
	{
		for(size_t j = 0; j < vMatrix[0].size(); ++j)
		{
			if(b_check_range(vRange, i, j, vSolution[iIndex]))
			{
				if(!b_set_value(vMatrix, i, j, vSolution[iIndex++]))
				{
					iErrorCode = NEGATIVE_NUMBER_ERR;
					return false;
				}
			}
			else
			{
				iErrorCode = VALUE_OUT_OF_RANGE;
				return false;
			}
		}
	}

	return true;
}

void CMscn::v_random_fill_tab(vector<double>& vTab, double dMinValue, double dMaxValue, CRandom & cRand)
{
	for(size_t i = 0; i < vTab.size(); ++i)
		b_set_value(vTab, i, cRand.dRange(dMinValue, dMaxValue));
}

void CMscn::v_random_fill_matrix(vector<vector<double>>& vMatrix, double dMinValue, double dMaxValue, CRandom & cRand)
{
	for(size_t i = 0; i < vMatrix.size(); ++i)
	{
		for(size_t j = 0; j < vMatrix[0].size(); ++j)
		{
			b_set_value(vMatrix[i], j, cRand.dRange(dMinValue, dMaxValue));
		}
	}
}