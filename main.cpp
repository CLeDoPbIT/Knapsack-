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
#include "dp_with_lower_bounds_and_cache.h"
#include "classical_bnb.h"
#include "bnb_with_cache.h"
#include "minknap.h"
#include "expknap.h"
#include "expknap_with_cache.h"

void run_one_problem(Problem problem, std::string method) {

	int W = problem.get_W();
	int number_items = problem.get_number_items();
	std::vector <int> weights = problem.get_weights();
	std::vector <int> values = problem.get_values();

	
	Classic_DP_Solver solver_dp(values, weights, W, number_items);
	DP_with_lower_bound_Solver solver_dp_with_left_bounds(values, weights, W, number_items);
	DP_With_Cache_Solver solver_dp_with_cache(values, weights, W, number_items);
	DP_With_Lower_Bounds_and_Cache_Solver solver_dp_with_lower_bounds_and_cache(values, weights, W, number_items);
	Classical_BnB_Solver solver_classical_bnb(values, weights, W, number_items);
	BnB_With_Cache_Solver solver_bnb_with_cache(values, weights, W, number_items);

	int* w = &weights[1];
	int* p = &values[1];
	int* x = 0;
	x = new int[number_items];
	std::vector <float> run_time;
	std::chrono::steady_clock::time_point start_time, end_time;

	exitem* f, * l;
	exitem* a;
	/* allocate space for test example */
	a = (exitem*)palloc(number_items, sizeof(exitem));
	f = a; l = &a[number_items - 1];
	int i = 1;
	for (exitem* j = f; j <= l; j++) {
		j->p = values[i];
		j->w = weights[i];
		i++;
	}


	exitem_with_cache* f_with_cache, * l_with_cache;
	exitem_with_cache* a_with_cache;
	/* allocate space for test example */
	a_with_cache = (exitem_with_cache*)palloc_with_cache(number_items, sizeof(exitem_with_cache));
	f_with_cache = a_with_cache; l_with_cache = &a_with_cache[number_items - 1];
	int k = 1;
	for (exitem_with_cache* j = f_with_cache; j <= l_with_cache; j++) {
		j->p = values[k];
		j->w = weights[k];
		k++;
	}

	std::pair <long, long> solution;

	for (int i = 0; i < 1; i++) {
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
		if (method == "DP_with_lower_bounds_and_cache") {
			solution = solver_dp_with_lower_bounds_and_cache.solve();
		}
		if (method == "Classical_BnB") {
			solution = solver_classical_bnb.solve();
		}
		if (method == "BnB_with_cache") {
			solution = solver_bnb_with_cache.solve();
		}
		if (method == "Minknap") {
			solution = minknap(number_items, p, w, x, W);
		}
		if (method == "Expknap") {
			solution = expknap(f, l, W);
		}
		if (method == "Expknap_with_cache") {
			solution = expknap_with_cache(f_with_cache, l_with_cache, W);
		}

		end_time = std::chrono::steady_clock::now();
		double dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
		run_time.push_back(dur_seconds);
	}
	float average = std::accumulate(run_time.begin(), run_time.end(), 0.0) / run_time.size();
	std::cout << method << " mean run time " << average << ", Solution " << solution.first << " Iterations " << solution.second  << std::endl;
}


int main(int argc, char** argv) {
	std::string text; 
	Problem problem;
	boolean generate_task = TRUE;
	if (generate_task) {
		problem.generate_problem(1000,1000,100,200,"no_small_weights"); 
		// uncorellated weakly_correlated strongly_correlated subset_sum inverse_strongly_correlated almost_strongly_correlated
		// similar_uncorrelated_weights even_odd_strongly_correlated even_odd_subset_sum no_small_weights
	}
	else {
		std::vector<std::string>path_to_folders = { "data/large_scale",
				"data/large_scale-optimum",
				"data/low-dimensional",
				"data/low-dimensional-optimum" };

		std::string filepath = path_to_folders[2] + "/f4_l-d_kp_4_11";  // "/knapPI_1_1000_1000_1" "/f4_l-d_kp_4_11" "/f8_l-d_kp_23_10000" "/f3_l-d_kp_4_20"


		problem.read_data_from_file(filepath);
	}
	
	
	std::vector <std::string> methods;
	//methods.push_back("Classical_DP");
	//methods.push_back("DP_with_left_bounds");
	//methods.push_back("DP_with_cache");
	//methods.push_back("DP_with_lower_bounds_and_cache"); // not correct 
 //   methods.push_back("Classical_BnB");
	//methods.push_back("BnB_with_cache"); // not correct 
	methods.push_back("Minknap");
	methods.push_back("Expknap");
	methods.push_back("Expknap_with_cache");

	for (int i = 0; i < methods.size(); i++) {
		run_one_problem(problem, methods[i]);
	} //  "Classical_DP" "DP_with_left_bounds" "DP_with_cache" "DP_with_lower_bounds_and_cache" "Classical_BnB" "BnB_with_cache"

	return 0;
}

