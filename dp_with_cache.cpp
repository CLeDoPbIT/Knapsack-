#include "dp_with_cache.h"
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
#include <unordered_map>


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

int DP_With_Cache_Solver::solve() {

	int* table = 0;
	table = new int[W + 1];
	for (int i = 0; i <= W + 1; i++) {table[i] = -1;}

	int* new_table = 0;
	new_table = new int[W + 1];
	for (int i = 0; i <= W + 1; i++) { new_table[i] = -1; }

	std::vector <int> keys, new_keys;

	table[0] = 0;
	keys.push_back(0);
	int tmp_weight = -1;
	int tmp_value = -1;
	int counter = 0;

	for (int n = 1; n <= number_items; n++) {
		//std::cout << "----------------------------" << std::endl;
		new_table = table;
		new_keys = keys;

		for (int i = 0; i < keys.size();i++) {
			tmp_weight = weights[n] + keys[i];
			if (tmp_weight <= W) {
				tmp_value = values[n] + new_table[keys[i]];
				counter++;

				if (tmp_weight == 20) { 
					std::cout<<n << " "<<values[n]<<" "<< new_table[keys[i]] <<std::endl; }
				if (new_table[tmp_weight]!=-1) {
					table[tmp_weight] = std::max(tmp_value, new_table[tmp_weight]);
				}
				else {
					table[tmp_weight] = tmp_value;
					new_keys.push_back(tmp_weight);
				}
				//std::cout << tmp_weight  << " " << table[tmp_weight] << std::endl;

			}

		}
		keys = new_keys;
	}

	std::cout << counter << std::endl;
	int tmp_max = -1;
	for (int i = 1; i <= W + 1; i++) { 
		//std::cout << i << " " << table[i] << std::endl;
		if (tmp_max < table[i]) { 
			tmp_max = table[i]; } }

	return tmp_max;

}