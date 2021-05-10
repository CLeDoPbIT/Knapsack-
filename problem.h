#pragma once
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

class Problem
{
private:
	int W;
	int number_items;
	std::vector <int> values, weights;
public:
	int get_W();
	void set_w(int tmp_W);
	int get_number_items();
	void set_number_items(int tmp_number_items);
	std::vector <int> get_values();
	void set_values(std::vector <int>  tmp_values);
	std::vector <int> get_weights();
	void set_weights(std::vector <int>  tmp_weights);
	void read_data_from_file(std::string filepath);
	void generate_problem(int n);
};

