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

class Classical_BnB_Solver
{	
public:
	int W;
	int number_items;
	std::vector <int> values;
	std::vector <int> weights;
	Classical_BnB_Solver(std::vector <int> tmp_values, std::vector <int> tmp_weights, int tmp_W, int tmp_number_items);
	std::pair <long, long> solve();
};

