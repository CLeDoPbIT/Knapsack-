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
#include <stack>
#include <locale.h>
#include <unordered_map>
#include "bnb_with_cache.h"
#include "robin_hood.h"

BnB_With_Cache_Solver::BnB_With_Cache_Solver(std::vector<int> tmp_values, std::vector<int> tmp_weights, int tmp_W, int tmp_number_items)
{
	W = tmp_W;
	number_items = tmp_number_items;
	values = tmp_values;
	weights = tmp_weights;
}

struct Item
{
	float weight;
	int value;
};

bool cmp_(Item a, Item b)
{
	double r1 = (double)a.value / a.weight;
	double r2 = (double)b.value / b.weight;
	return r1 > r2;
}


//int bound_(Node u, int n, int W, Item arr[], int* current_nun_item, float current_weight)
//{
//	// if weight overcomes the knapsack capacity, return
//	// 0 as expected bound
//	if (current_weight >= W)
//		return 0;
//
//	// initialize bound on profit by current profit
//	int profit_bound = u.profit;
//
//	// start including items from index 1 more to current
//	// item index
//	int totweight = current_weight;
//
//	// checking index condition and knapsack capacity
//	// condition
//	int i = 0;
//	while (totweight + arr[i].weight <= W)
//	{
//		if (current_nun_item[i] != 1) {
//			totweight += arr[i].weight;
//			profit_bound += arr[i].value;
//		}
//		i++;
//	}
//
//	// If k is not n, include last item partially for
//	// upper bound on profit
//	while (i < n){
//		if (current_nun_item[i] != 1) {
//			profit_bound += (W - totweight) * arr[i].value / arr[i].weight;
//		}
//		i++;
//	}
//		
//
//	return profit_bound;
//}


int bound_(Node u, int n, int W, Item arr[], int current_nun_item, float current_weight)
{
	// if weight overcomes the knapsack capacity, return
	// 0 as expected bound
	if (current_weight >= W)
		return 0;

	// initialize bound on profit by current profit
	int profit_bound = u.profit;

	// start including items from index 1 more to current
	// item index
	int totweight = current_weight;

	// checking index condition and knapsack capacity
	// condition

	while ((current_nun_item < n) && (totweight + arr[current_nun_item].weight <= W))
	{
		totweight += arr[current_nun_item].weight;
		profit_bound += arr[current_nun_item].value;
		current_nun_item++;
	}

	// If k is not n, include last item partially for
	// upper bound on profit
	if (current_nun_item <= n)
		profit_bound += (W - totweight) * arr[current_nun_item].value / arr[current_nun_item].weight;

	return profit_bound;
}


std::tuple<int, float, int >  greed_solve_(int W, Item* items, int number_items) {
	int solution = 0;
	float current_W = W;
	int i = 1;
	while ((i <= number_items) && (current_W - items[i].weight > 0)) {
		solution = solution + items[i].value;
		current_W = current_W - items[i].weight;
		i++;
	}
	std::tuple<int, float, int > result(solution, current_W, i);
	return result;
}


Node** BnB_With_Cache_Solver::create2DArray(unsigned height, unsigned width)
{
	Node** array2D = 0;
	array2D = new Node * [height];

	for (int h = 0; h < height; h++)
	{
		array2D[h] = new Node[width];

		for (int w = 0; w < width; w++)
		{
			Node tmp;
			tmp.profit = 0;
			tmp.bound = 0;
			array2D[h][w] = tmp;
		}
	}

	return array2D;
};



int BnB_With_Cache_Solver::solve() {
	Item* items = 0;
	items = new Item[number_items+1];
	for (int i = 1; i <= number_items; i++) {
		items[i].value = values[i];
		items[i].weight = weights[i];
	}
	items[0].value = std::numeric_limits<int>::max();
	items[0].weight = 1;
	std::sort(items, items + number_items+1, cmp_);
	//for (int i = 0; i <= number_items;i++ ) {
	//	std::cout << items[i].weight << " " << items[i].value << " " << items[i].value/ items[i].weight << std::endl;
	//}

	Node** table = create2DArray(number_items + 1, W + 1);

	std::vector <float> time_count;
	int* keys = 0;
	keys = new int[W];
	keys[0] = 1;

	Node u, v;
	int maxProfit = 0;
	int num_greed_items = 0;
	float w_greed = 0;
	std::tuple<int, float, int > greed_solution;
	greed_solution = greed_solve_(W, items, number_items);

	u.profit = 0;

	u.bound = bound_(u, number_items, W, items, 1, 0);
	table[0][0] = u;
	u.profit = std::get<0>(greed_solution);
	table[number_items][W] = u;
	
	int counter = 0;
	float ub = 0;
	int weight = 0;
	for (int n = 1; n <= number_items; n++) {
		//std::cout << "----------------------" << std::endl;
		for (int w = W; w >= 0; w--) {
			if ((keys[w] == 1) && (w >= items[n].weight)) {
				if (table[n - 1][w].bound > maxProfit) {
					if (items[n].weight <= W) {
						if (items[n].weight > w) {
							v.profit = table[n - 1][w].profit;
						}
						else {
							v.profit = std::max(table[n - 1][w].profit, table[n - 1][w - int(items[n].weight)].profit + items[n].value);
						}
						v.bound = bound_(v, number_items, W, items, n + 1, w);
						table[n][w] = v;
						counter++;
						keys[w] = 1;
						if (maxProfit < v.profit) {
							maxProfit = v.profit;
						}
					}
				}
			}
			else if ((keys[w - int(items[n].weight)] == 1) && (w >= items[n].weight))
			{
				if (table[n - 1][w - int(items[n].weight)].bound > maxProfit) {
					if (items[n].weight <= W) {
						if (items[n].weight > w) {
							v.profit = table[n - 1][w].profit;
						}
						else {
							v.profit = std::max(table[n - 1][w].profit, table[n - 1][w - int(items[n].weight)].profit + items[n].value);
						}
						v.bound = bound_(v, number_items, W, items, n + 1, w);
						table[n][w] = v;
						counter++;
						keys[w] = 1;
						if (maxProfit < v.profit) {
							maxProfit = v.profit;
						}
					}
				}
			}
		}

	}
	//std::cout << counter << std::endl;

	return maxProfit;

}



//int BnB_With_Cache_Solver::solve() {
//	Item* items = 0;
//	items = new Item[number_items];
//	for (int i = 1; i <= number_items; i++) {
//		items[i - 1].value = values[i];
//		items[i - 1].weight = weights[i];
//	}
//
//	std::sort(items, items + number_items, cmp_);
//
//	//for (int i = 0; i < number_items;i++ ) {
//	//	std::cout << items[i].weight << " " << items[i].value << " " << items[i].value/ items[i].weight << std::endl;
//	//}
//
//	robin_hood::unordered_map <int, Node> cache, tmp_cache;
//
//	Node u, v;
//
//	u.profit = 0;
//	u.level = new int[number_items];
//	
//	//u.bound = bound_(u, number_items, W, items, 0, 0);
//	cache[0] = u;
//
//	int maxProfit = greed_solve_(W, items, number_items);
//	int counter = 0;
//	float ub = 0;
//	int weight = 0;
//	for (const auto& tmp : cache) {
//		//std::cout << "----------------------" << std::endl;
//		for (int n = 0; n < number_items; n++) {
//			if (tmp.second.level[n] != 1) {
//				tmp.second.level[n] = 1;
//				v.profit = tmp.second.profit;
//				v.bound = bound_(v, number_items, W, items, tmp.second.level, tmp.first);
//				if (v.bound > maxProfit) {
//					weight = items[n].weight + tmp.first;
//					if (weight <= W) {
//						counter++;
//						if (cache.count(weight)) {
//							cache[weight].profit = std::max(tmp.second.profit + items[n].value, cache[weight].profit);
//						}
//						else {
//							cache[weight].profit = tmp.second.profit + items[n].value;
//							cache[weight].level = new int[number_items];
//						}
//						cache[weight].level[n] = 1;
//						if (cache[weight].profit > maxProfit) {
//							maxProfit = cache[weight].profit;
//						}
//
//						//std::cout << weight <<" " << cache[weight].profit << std::endl;
//					}
//				}
//			}
//		}
//	}
//	std::cout << counter << std::endl;
//
//	return maxProfit;
//
//}


//int BnB_With_Cache_Solver::solve() {
//	Item* items = 0;
//	items = new Item[number_items];
//	for (int i = 1; i <= number_items; i++) {
//		items[i - 1].value = values[i];
//		items[i - 1].weight = weights[i];
//	}
//
//	std::sort(items, items + (number_items - 1), cmp_);
//	robin_hood::unordered_map <int, Node> cache, tmp_cache;
//	
//	Node u, v;
//
//	u.profit = 0;
//	//u.bound = bound_(u, number_items, W, items, 0, 0);
//	cache[0] = u;
//
//	int maxProfit = greed_solve_(W, items, number_items);
//	int counter = 0;
//	float ub = 0;
//	int weight = 0;
//	for (int n = 0; n < number_items; n++) {
//		//std::cout << "----------------------" << std::endl;
//		tmp_cache = cache;
//		for (const auto& tmp : tmp_cache) {
//			v.profit = tmp.second.profit;
//			v.bound = bound_(v, number_items, W, items, n, tmp.first);
//			if (v.bound > maxProfit){
//				weight = items[n].weight + tmp.first;
//				if (weight <= W) {
//					counter++;
//					if (cache.count(weight)) {
//						cache[weight].profit = std::max(tmp.second.profit + items[n].value, cache[weight].profit);
//					}
//					else {
//						cache[weight].profit = tmp.second.profit + items[n].value;
//					}
//					if (cache[weight].profit > maxProfit) {
//						maxProfit = cache[weight].profit;
//					}
//
//					//std::cout << weight <<" " << cache[weight].profit << std::endl;
//				}
//			}
//		}
//	}
//	std::cout << counter << std::endl;
//
//	return maxProfit;
//
//}