#include "problem.h"
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


int Problem::get_W() { return W; }
void Problem::set_w(int tmp_W) { W = tmp_W; }
int Problem::get_number_items() { return number_items; }
void Problem::set_number_items(int tmp_number_items) { number_items = tmp_number_items; }
std::vector <int> Problem::get_values() { return values; }
void Problem::set_values(std::vector <int>  tmp_values) { values = tmp_values; }
std::vector <int> Problem::get_weights() { return weights; }
void Problem::set_weights(std::vector <int>  tmp_weights) { weights = tmp_weights; }
void Problem::read_data_from_file(std::string filepath) {
	std::ifstream file_text(filepath);

	int tmp1, tmp2;
	while (file_text >> tmp1 >> tmp2)
	{
		values.push_back(tmp1);
		weights.push_back(tmp2);
	};
	number_items = values[0];
	W = weights[0];
}
