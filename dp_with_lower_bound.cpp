#include "dp_with_lower_bound.h"
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


struct Subject
{
	int weight;
	int value;
};


bool compare_subjects_(Subject a, Subject b) {
	return a.weight < b.weight;
}


int DP_with_lower_bound_Solver::solve() {
	table = create2DArray(number_items + 1, W + 1);

	int counter = 0;
	std::vector<Subject> subjects;

	for (int i = 0; i <= number_items; i++) {
		Subject tmp;
		tmp.value = values[i];
		tmp.weight = weights[i];
		subjects.push_back(tmp);
	}
	subjects[0].weight = -1;
	std::sort(subjects.begin(), subjects.end(), compare_subjects_);

	int* left_bounds = 0;
	left_bounds = new int [number_items+1];
	left_bounds[number_items] = W;

	int j = number_items;
	for (int i = 1; i <= number_items; i++) {
		if (left_bounds[j] - subjects[i].weight > 0) {
			left_bounds[j-1] = left_bounds[j] - subjects[i].weight;
		}
		else {
			left_bounds[j-1] = left_bounds[j];
		}
		//std::cout << left_bounds[j-1] << std::endl;
		j--;
	}

	int index = 0;

	for (int n = 1; n <= number_items; n++) {
		for (int w = left_bounds[n]; w <= W; w++) {
			counter++;
			index = number_items - n + 1;
			if (subjects[index].weight > w) {
				table[n][w] = table[n-1][w];
			}
			else {
				table[n][w] = std::max(table[n-1][w], table[n-1][w - subjects[index].weight] + subjects[index].value);
			}
			//std::cout << table[n][w] << " ";
		}
		//std::cout << std::endl;
	}
	std::cout << counter << std::endl;

	return table[number_items][W];
}

DP_with_lower_bound_Solver::DP_with_lower_bound_Solver(std::vector<int> tmp_values, std::vector<int> tmp_weights, int tmp_W, int tmp_number_items)
{
	W = tmp_W;
	number_items = tmp_number_items;
	values = tmp_values;
	weights = tmp_weights;
	table = create2DArray(number_items + 1, W + 1);
}
int** DP_with_lower_bound_Solver::create2DArray(unsigned height, unsigned width)
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
int** DP_with_lower_bound_Solver::get_table() { return table; }
