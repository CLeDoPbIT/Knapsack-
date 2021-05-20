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


//void threadFunction(std::future<void> futureObj)
//{
//	std::cout << "Thread Start" << std::endl;
//	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
//	{
//		std::cout << "Doing Some Work" << std::endl;
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//	}
//	std::cout << "Thread End" << std::endl;
//}


double* run_one_problem(Problem problem, std::string method) {
	
	int W = problem.get_W();
	int number_items = problem.get_number_items();
	std::vector <int> weights = problem.get_weights();
	std::vector <int> values = problem.get_values();

	
	Classic_DP_Solver solver_dp(values, weights, W, number_items);// TODO: MEMORY LEAK IN THIS CASES
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

	start_time = std::chrono::steady_clock::now();
	float average = 0;
	for (int i = 0; i < 1; i++) {
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

		average = std::accumulate(run_time.begin(), run_time.end(), 0.0) / run_time.size();
		std::cout << method << " mean1 run time " << average << ", Solution " << solution.first << " Iterations " << solution.second << std::endl;

	}
	double *result = 0;
	result = new double[3];

	result[0] = average;
	result[1] = solution.first;
	result[2] = solution.second;
	return result;
}


int main(int argc, char** argv) {
	std::string text; 
	Problem problem;
	boolean generate_task = TRUE;

	std::vector <std::string> methods;
	methods.push_back("Classical_DP");
	methods.push_back("DP_with_left_bounds");
	methods.push_back("DP_with_cache");
	methods.push_back("DP_with_lower_bounds_and_cache"); // not correct 
	methods.push_back("Classical_BnB");
	methods.push_back("BnB_with_cache"); // not correct 
	methods.push_back("Minknap");
	methods.push_back("Expknap");
	methods.push_back("Expknap_with_cache");

	double *result = 0;
	if (generate_task) {
		int number_tasks = 10;
		const char* types_benchmarks[1] = { "even_odd_subset_sum",
			 };

		// "uncorellated", "strongly_correlated", "subset_sum", "weakly_correlated"


		//	// 
		/*"spanner_weakly_correlated", "inverse_strongly_correlated",
			"almost_strongly_correlated", "similar_uncorrelated_weights", "even_odd_strongly_correlated", "even_odd_subset_sum",
			"no_small_weights"*/

		for (int type_bech = 0; type_bech < 1; type_bech++) {
			for (int i = 1; i <= number_tasks; i++) {
				problem.generate_problem(10000, 1000, i, number_tasks, types_benchmarks[type_bech]);
				std::cout << "------------------"<< types_benchmarks[type_bech] << " " << i <<"--------------------------" << std::endl;
				for (int m = 0; m < methods.size(); m++) {
					
					try {
						result = run_one_problem(problem, methods[m]);

						std::ofstream myfile;
						std::string str(types_benchmarks[type_bech]);
						std::string str_m(methods[m]);
						
						myfile.open("C:/Users/EBurashnikov/source/repos/Knapsack/Knapsack/data_generated/" + str + "/result_" + str_m +"_" + std::to_string(i) + ".txt");
						myfile << result[0] << " " << long(result[1]) << " " << long(result[2]);
						myfile.close();
					}
					catch (int a) {
						result = new double[3];
						result[0] = 0;
						result[1] = 0;
						result[2] = 0;
					}
					catch (exception & e)
					{
						cout << "Standard exception: " << e.what() << endl;
					}
					catch (...) {
						result = new double[3];
						result[0] = 0;
						result[1] = 0;
						result[2] = 0;

					}
				}
				problem.clear();
			}
		}  
		
		// uncorellated weakly_correlated strongly_correlated subset_sum inverse_strongly_correlated almost_strongly_correlated
		// similar_uncorrelated_weights even_odd_strongly_correlated even_odd_subset_sum no_small_weights
		// spanner_uncorrelated spanner_weakly_correlated spanner_strongly_correlated
		// multiple_correlated profit_ceiling circle quadratic_fit  cubic_fit random_ceiling profit_ceiling
	} 
	else {
		std::vector<std::string>path_to_folders = { "data/large_scale",
				"data/large_scale-optimum",
				"data/low-dimensional",
				"data/low-dimensional-optimum" };

		std::string filepath = path_to_folders[0] + "/knapPI_3_2000_1000_1";  // "/knapPI_1_1000_1000_1" "/f4_l-d_kp_4_11" "/f8_l-d_kp_23_10000" "/f3_l-d_kp_4_20"


		problem.read_data_from_file(filepath);
		for (int i = 0; i < methods.size(); i++) {
			//std::cout << methods[i] << " mean run time " << float(time_run) << ", Solution " << solution.first << " Iterations " << solution.second << std::endl;
			run_one_problem(problem, methods[i]);

		} //  "Classical_DP" "DP_with_left_bounds" "DP_with_cache" "DP_with_lower_bounds_and_cache" "Classical_BnB" "BnB_with_cache"
	}

	return 0;
}

