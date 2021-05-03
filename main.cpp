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
#include <numeric>
#include "problem.h"
#include "classical_dp.h"
#include "dp_with_lower_bound.h"
#include "dp_with_cache.h"


void run_one_problem(Problem problem, std::string method) {

	int W = problem.get_W();
	int number_items = problem.get_number_items();
	std::vector <int> weights = problem.get_weights();
	std::vector <int> values = problem.get_values();

	
	Classic_DP_Solver solver_dp(values, weights, W, number_items);
	DP_with_lower_bound_Solver solver_dp_with_left_bounds(values, weights, W, number_items);
	DP_With_Cache_Solver solver_dp_with_cache(values, weights, W, number_items);

	std::vector <float> run_time;
	std::chrono::steady_clock::time_point start_time, end_time;
	int solution = 0;
	for (int i = 0; i < 3; i++) {
		start_time = std::chrono::steady_clock::now();
		if (method == "Classical_DP") {
			solution = solver_dp.solve();
		}
		if (method == "DP_with_left_bounds") {
			solution = solver_dp_with_left_bounds.solve();
		}
		if (method == "DP_with_cache") {
			solution = solver_dp_with_cache.solve();
		}
		end_time = std::chrono::steady_clock::now();
		double dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
		run_time.push_back(dur_seconds);
	}
	float average = std::accumulate(run_time.begin(), run_time.end(), 0.0) / run_time.size();
	std::cout << "Mean run time " << average << ", Solution " << solution << std::endl;
}


int main(int argc, char** argv) {
	std::string text; 

	std::vector<std::string>path_to_folders = { "data/large_scale",
			"data/large_scale-optimum",
			"data/low-dimensional",
			"data/low-dimensional-optimum" };

	std::string filepath = path_to_folders[0] + "/knapPI_1_5000_1000_1";  // "/knapPI_1_1000_1000_1" "/f4_l-d_kp_4_11" "/f8_l-d_kp_23_10000" "/f3_l-d_kp_4_20"


	Problem problem;
	problem.read_data_from_file(filepath);

	run_one_problem(problem, "DP_with_cache"); //  "Classical_DP" "DP_with_left_bounds" "DP_with_cache"

	return 0;
}

