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

class BnB_With_Cache_Solver
{
public:
	int W;
	int number_items;
	std::vector <int> values;
	std::vector <int> weights;
	BnB_With_Cache_Solver(std::vector <int> tmp_values, std::vector <int> tmp_weights, int tmp_W, int tmp_number_items);
	int solve();
};

