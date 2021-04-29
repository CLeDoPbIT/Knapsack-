#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <cmath>
#include <math.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <locale.h>
#include "problem.h"
#include "classical_dp.h"


int main(int argc, char** argv) {
	std::string text;

	std::vector<std::string>path_to_folders = { "data/large_scale",
			"data/large_scale-optimum",
			"data/low-dimensional",
			"data/low-dimensional-optimum" };

	std::string filepath = path_to_folders[2] + "/f4_l-d_kp_4_11";


	Problem problem;
	problem.read_data_from_file(filepath);

	int W = problem.get_W();
	int number_items = problem.get_number_items();
	std::vector <int> weights = problem.get_weights();
	std::vector <int> values = problem.get_values();

	Classic_DP_Solver classic_dp_solver(values, weights, W, number_items);
	int solution = classic_dp_solver.solve();
	std::cout << "Classical DP solutuion for " << solution << std::endl;

	return 0;
}

