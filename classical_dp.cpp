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
#include "classical_dp.h"

Classic_DP_Solver::Classic_DP_Solver(std::vector<int> tmp_values, std::vector<int> tmp_weights, int tmp_W, int tmp_number_items)
{
	W = tmp_W;
	number_items = tmp_number_items;
	values = tmp_values;
	weights = tmp_weights;
	table = create2DArray(number_items+1, W+1);
}

int** Classic_DP_Solver::create2DArray(unsigned height, unsigned width)
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
int** Classic_DP_Solver::get_table() { return table; }
int Classic_DP_Solver::solve() {
	for (int n = 1; n <= number_items; n++) {
		for (int w = 1; w <= W; w++) {
			if (weights[n] > w) {
				table[n][w] = table[n - 1][w];
			}
			else {
				table[n][w] = std::max(table[n - 1][w], table[n - 1][w - weights[n]] + values[n]);
			}
		}
	}
	return table[number_items][W];
}