#include "dp_with_cache.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <math.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <locale.h>
#include <unordered_map>
#include <map>
#include <unordered_set>




DP_With_Cache_Solver::DP_With_Cache_Solver(std::vector<int> tmp_values, std::vector<int> tmp_weights, int tmp_W, int tmp_number_items)
{
	W = tmp_W;
	number_items = tmp_number_items;
	values = tmp_values;
	weights = tmp_weights;
}

//int DP_With_Cache_Solver::solve() {
//
//	std::unordered_map <int, int> table, table_new;
//	std::unordered_map<int, int>::iterator it;
//
//	table[0] = 0;
//	table_new[0] = 0;
//	int tmp_weight = -1;
//	int tmp_value = -1;
//	int counter = 0;
//
//
//
//	for (int n = 1; n <= number_items; n++) {
//
//
//		for(auto & x: table) {
//			tmp_weight = weights[n] + x.first;
//			if (tmp_weight <= W) {
//				tmp_value = values[n] + x.second;
//				counter++;
//				if (table.count(tmp_weight)) {
//					table_new[tmp_weight] = std::max(tmp_value, table[tmp_weight]);
//				}
//				else {
//					table_new[tmp_weight] = tmp_value;
//				}
//			}
//
//		}
//		table = table_new;
//	}
//
//	std::cout << counter << std::endl;
//	std::unordered_map<int, int>::iterator best = std::max_element(table_new.begin(), table_new.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b)->bool { return a.second < b.second; });
//
//	return best->second;
//
//}

int** DP_With_Cache_Solver::create2DArray(unsigned height, unsigned width)
{
	int** array2D = 0;
	array2D = new int* [height];

	for (int h = 0; h < height; h++)
	{
		array2D[h] = new int[width];

		for (int w = 0; w < width; w++)
		{
			array2D[h][w] = 0;
		}
	}

	return array2D;
};


int DP_With_Cache_Solver::solve() {
	//std::chrono::steady_clock::time_point start_time, end_time;
	//start_time = std::chrono::steady_clock::now();
	int** table = create2DArray(number_items + 1, W + 1);
	//end_time = std::chrono::steady_clock::now();
	//float dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
	//std::cout << dur_seconds << std::endl;
	std::vector <float> time_count;
	int* keys = 0;
	keys = new int[W];
	table[0][0] = 0;
	keys[0] = 1;
	int tmp_weight = -1;
	int tmp_value = -1;
	int counter = 0;
	float dur_seconds = 0;
	int size = 0;
	std::chrono::steady_clock::time_point start_time, end_time;
	for (int n = 1; n <= number_items; n++) {
		//std::cout << "----------- --- --  ------------" << std::endl;

		//start_time = std::chrono::steady_clock::now();
		//end_time = std::chrono::steady_clock::now();
		//dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
		//time_count.push_back(dur_seconds);
		
		for (int w = W; w >= 0; w--) {
			//start_time = std::chrono::steady_clock::now();
			if ((keys[w] == 1)|| ((keys[w - weights[n]]==1)&&(w>= weights[n]))) {
				counter++;
				if (weights[n] > w) {
					table[n][w] = table[n - 1][w];
				}
				else {
					table[n][w] = std::max(table[n - 1][w], table[n - 1][w - weights[n]] + values[n]);
				}
				keys[w] = 1;
			}
			//end_time = std::chrono::steady_clock::now();
			//dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
			//time_counter.push_back(dur_seconds);

		
		}
		//start_time = std::chrono::steady_clock::now();
		//end_time = std::chrono::steady_clock::now();
		//dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
		//time_count.push_back(dur_seconds);

	}

	std::cout << counter << std::endl;

	//float sum_time = 0;
	//for (int n = 0; n <= time_count.size(); n++) { sum_time = sum_time + time_count[n]; }
	//std::cout << sum_time << std::endl;

	int tmp_max = -1;
	for (int i = 1; i <= W + 1; i++) {
		//std::cout << i << " " << table[number_items][i] << std::endl;
		if (tmp_max < table[number_items][i]) {
			tmp_max = table[number_items][i];
		}
	}

	return tmp_max;

}


//int DP_With_Cache_Solver::solve() {
//	//std::chrono::steady_clock::time_point start_time, end_time;
//	//start_time = std::chrono::steady_clock::now();
//	int** table = create2DArray(number_items + 1, W + 1);
//	//end_time = std::chrono::steady_clock::now();
//	//float dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
//	//std::cout << dur_seconds << std::endl;
//	std::vector <float> time_count;
//	std::vector <int> keys;
//
//	table[0][0] = 0;
//	keys.push_back(0);
//	int tmp_weight = -1;
//	int tmp_value = -1;
//	int counter = 0;
//	float dur_seconds = 0;
//	int size = 0;
//	std::chrono::steady_clock::time_point start_time, end_time;
//	for (int n = 1; n <= number_items; n++) {
//		//std::cout << "----------- --- --  ------------" << std::endl;
//
//		//start_time = std::chrono::steady_clock::now();
//		//end_time = std::chrono::steady_clock::now();
//		//dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
//		//time_count.push_back(dur_seconds);
//		size = keys.size();
//		for (int i = 0; i < size; i++) {
//			//start_time = std::chrono::steady_clock::now();
//			table[n][keys[i]] = std::max(table[n - 1][keys[i]], table[n][keys[i]]);
//			counter++;
//			//end_time = std::chrono::steady_clock::now();
//			//dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
//			//time_count.push_back(dur_seconds);
//
//			tmp_weight = weights[n] + keys[i];
//			if (tmp_weight <= W) {
//				counter++;
//
//				//if (tmp_weight == 3) { 
//				//	std::cout<<n << " "<<values[n]<<"   "<< table[n-1][keys[i]] <<std::endl; }
//				//if (tmp_weight == 20) {
//				//	std::cout << n << " " << values[n] << "   " << new_table[keys[i]] << std::endl;
//				//}
//
//				if (table[n - 1][tmp_weight] != -1) {
//					table[n][tmp_weight] = std::max(values[n] + table[n - 1][keys[i]], table[n - 1][tmp_weight]);
//				}
//				else {
//					table[n][tmp_weight] = values[n] + table[n - 1][keys[i]];
//					keys.push_back(tmp_weight);
//				}
//				//std::cout << tmp_weight  << " " << table[n][tmp_weight] << std::endl;
//
//			}
//			//table[n][keys[i]] = std::max(table[n - 1][keys[i]], table[n][keys[i]]);
//		}
//		//start_time = std::chrono::steady_clock::now();
//		//end_time = std::chrono::steady_clock::now();
//		//dur_seconds = std::chrono::duration<float>(end_time - start_time).count();
//		//time_count.push_back(dur_seconds);
//
//	}
//
//	std::cout << counter << std::endl;
//
//	//float sum_time = 0;
//	//for (int n = 0; n <= time_count.size(); n++) { sum_time = sum_time + time_count[n]; }
//	//std::cout << sum_time << std::endl;
//
//	//int tmp_max = -1;
//	//for (int i = 1; i <= W + 1; i++) { 
//	//	//std::cout << i << " " << table[number_items][i] << std::endl;
//	//	if (tmp_max < table[number_items][i]) {
//	//		tmp_max = table[number_items][i]; } }
//
//	return table[num
//][W];
//
//}