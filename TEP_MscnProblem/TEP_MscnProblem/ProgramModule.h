#pragma once
#include <iostream>
#include <string>
#include "CMscnProblem.h"
#include "CRandomSearch.h"
#include "CDiffEvol.h"

CMscnProblem v_input_data()
{
	CMscnProblem c_problem;
	int i_input = -1;

	while(!c_problem.bSetDeliverers(i_input))
	{
		std::cout << "Input Deliverers amount: ";
		std::cin >> i_input;
	}

	i_input = -1;
	while(!c_problem.bSetFactiories(i_input))
	{
		std::cout << "Input Factories amount: ";
		std::cin >> i_input;
	}

	i_input = -1;
	while(!c_problem.bSetMagazines(i_input))
	{
		std::cout << "Input Magazines amount: ";
		std::cin >> i_input;
	}

	i_input = -1;
	while(!c_problem.bSetShops(i_input))
	{
		std::cout << "Input Shops amount: ";
		std::cin >> i_input;
	}

	return c_problem;
}

void vCreateEmptyProblemFile()
{
	CMscnProblem c_problem = v_input_data();

	std::cout << "Input file name: ";
	std::string s_file_name;
	std::cin >> s_file_name;
	if(c_problem.bSaveToFile(s_file_name))
		std::cout << "\nCreated file: \"" << s_file_name << "\"\n";
	else
		std::cout << "\nERROR: Failed to created file: \"" << s_file_name << "\"\n";
}

void vCreateRandomProblemFile()
{
	CMscnProblem c_problem = v_input_data();

	std::cout << "Enter seed (0 = default seed): ";
	int i_seed;
	std::cin >> i_seed;

	std::cout << "Input file name: ";
	std::string s_file_name;
	std::cin >> s_file_name;

	c_problem.vGenerateInstance(i_seed);

	std::cout << "\nEnter global range for solutions\nMinimal value: ";
	int i_max;
	int i_min;
	std::cin >> i_min;
	std::cout << "Maximal value: ";
	std::cin >> i_max;

	c_problem.bSetGlobalRangeXD(i_min, i_max);
	c_problem.bSetGlobalRangeXF(i_min, i_max);
	c_problem.bSetGlobalRangeXM(i_min, i_max);

	if(c_problem.bSaveToFile(s_file_name))
		std::cout << "\nCreated file: \"" << s_file_name << "\"\n";
	else
		std::cout << "\nERROR: Failed to created file: \"" << s_file_name << "\"\n";
}

void vCreateEmptySolutionFile()
{
	CMscnProblem c_problem = v_input_data();

	std::cout << "Input file name: ";
	std::string s_file_name;
	std::cin >> s_file_name;
	if(c_problem.bCreateSolutionFile(s_file_name))
		std::cout << "\nCreated file: \"" << s_file_name << "\"\n";
	else
		std::cout << "\nERROR: Failed to created file: \"" << s_file_name << "\"\n";
}

void vCreateSolutionFile(CMscnProblem &cProblem)
{
	std::cout << "\nInput file name: ";
	std::string s_file_name;
	std::cin >> s_file_name;
	if(cProblem.bCreateSolutionFile(s_file_name))
		std::cout << "\nCreated file: \"" << s_file_name << "\"\n";
	else
		std::cout << "\nERROR: Failed to created file: \"" << s_file_name << "\"\n";
}

void vGenerateSolutionRS(CMscnProblem &cProblem)
{
	std::cout << "Enter seed (0 = default seed): ";
	int i_seed;
	std::cin >> i_seed;

	std::cout << "Enter max number of iterations: ";
	int i_input;
	std::cin >> i_input;

	CRandomSearch c_search(cProblem);

	clock_t timer;
	timer = clock();

	double d_quality = c_search.dGenerateSolution(i_input, i_seed);

	timer = clock() - timer;
	printf("\ncalculation time: %f seconds\n", (float) timer / CLOCKS_PER_SEC);
	std::cout << "\nQuality of found solution for current problem" << "\n= " << d_quality << std::endl;

	vCreateSolutionFile(cProblem);
}

void vGenerateSolutionDE(CMscnProblem &cProblem)
{
	std::cout << "Enter seed (0 = default seed): ";
	int i_seed;
	std::cin >> i_seed;

	std::cout << "Enter max number of fitness evaluations: ";
	int i_evals;
	std::cin >> i_evals;

	std::cout << "Enter population size(minimum size = " << MIN_POPULATION << "): ";
	int i_pop;
	std::cin >> i_pop;

	CDiffEvol c_evol(cProblem);

	clock_t timer;
	timer = clock();

	double d_quality = c_evol.dGenerateSolution(i_evals, i_pop, i_seed);

	timer = clock() - timer;

	printf("\ncalculation time: %f seconds\n", (float)timer / CLOCKS_PER_SEC);
	std::cout << "\nQuality of found solution for current problem" << "\n= " << d_quality << std::endl;

	if(d_quality >= 0.0)
		vCreateSolutionFile(cProblem);
}

void vLoadProblemFile(CMscnProblem &cProblem)
{
	std::cout << "Input file name: ";
	std::string s_file_name;
	std::cin >> s_file_name;
	if(cProblem.bLoadFromFile(s_file_name))
		std::cout << "\nLoaded settings from file: \"" << s_file_name << "\"\n";
	else
		std::cout << "\nERROR: Failed to load settings from file: \"" << s_file_name << "\"\n";
}

void vLoadAndEvalSolutionFile(CMscnProblem &cProblem)
{
	std::cout << "Input file name: ";
	std::string s_file_name;
	std::cin >> s_file_name;

	int i_err;
	vector<double> v_solution = vLoadSolutionFromFile(s_file_name);
	double d_quality = cProblem.dGetQuality(v_solution, i_err);

	if(i_err == 0)
		std::cout << "\nQuality of solution from file \"" << s_file_name << "\"\n= " << d_quality << std::endl;
	else
		std::cout << "\nERROR: " << i_err << "\n";
}

void vShowMenu(CMscnProblem &cProblem)
{
	std::cout << "\n\n\t===== MSCN Problem =====\n\n";

	std::cout << "[1] Create empty problem file\n";
	std::cout << "[2] Create empty solution file\n";
	std::cout << "[3] Load problem file\n";
	std::cout << "[4] Get quality of solution from file\n";
	std::cout << "[5] Create random problem file\n";
	std::cout << "[6] Generate optimal solution using rand-search and save to file\n";
	std::cout << "[7] Generate optimal solution using diff-evolution and save to file\n";
	std::cout << "[9] Exit program\n";
	std::cout << "\nChoose option: ";

	int input;
	std::cin >> input;

	switch(input)
	{
	case 1:
		vCreateEmptyProblemFile();
		vShowMenu(cProblem);
		break;
	case 2:
		vCreateEmptySolutionFile();
		vShowMenu(cProblem);
		break;
	case 3:
		vLoadProblemFile(cProblem);
		vShowMenu(cProblem);
		break;
	case 4:
		vLoadAndEvalSolutionFile(cProblem);
		vShowMenu(cProblem);
		break;
	case 5:
		vCreateRandomProblemFile();
		vShowMenu(cProblem);
		break;
	case 6:
		vGenerateSolutionRS(cProblem);
		vShowMenu(cProblem);
		break;
	case 7:
		vGenerateSolutionDE(cProblem);
		vShowMenu(cProblem);
		break;
	case 9:
		std::cout << "\n\tExiting program...\n";
		break;
	default:
		std::cout << "ERROR: undefined input";
		vShowMenu(cProblem);
	}
}