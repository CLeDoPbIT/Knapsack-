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



struct Node
{
	// level --> Level of node in decision tree (or index
	//			 in arr[]
	// profit --> Profit of nodes on path from root to this
	//		 node (including this node)
	// bound ---> Upper bound of maximum profit in subtree
	//		 of this node/
	int level, profit, bound;
	float weight;
};



class BnB_With_Cache_Solver
{
public:
	int W;
	int number_items;
	std::vector <int> values;
	std::vector <int> weights;
	BnB_With_Cache_Solver(std::vector <int> tmp_values, std::vector <int> tmp_weights, int tmp_W, int tmp_number_items);
	Node** create2DArray(unsigned height, unsigned width);
	int solve();
};

