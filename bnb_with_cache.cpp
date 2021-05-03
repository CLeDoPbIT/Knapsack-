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

struct Node
{
	// level --> Level of node in decision tree (or index
	//			 in arr[]
	// profit --> Profit of nodes on path from root to this
	//		 node (including this node)
	// bound ---> Upper bound of maximum profit in subtree
	//		 of this node/
	int profit, bound;
};



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
	if (current_nun_item < n)
		profit_bound += (W - totweight) * arr[current_nun_item].value / arr[current_nun_item].weight;

	return profit_bound;
}


int greed_solve_(int W, Item* items, int number_items) {
	int solution = 0;
	int current_W = W;
	int i = 0;
	while ((i < number_items) && (current_W - items[i].weight > 0)) {
		solution = solution + items[i].value;
		current_W = current_W - items[i].weight;
		i++;
	}
	return solution;
}


int BnB_With_Cache_Solver::solve() {
	Item* items = 0;
	items = new Item[number_items];
	for (int i = 1; i <= number_items; i++) {
		items[i - 1].value = values[i];
		items[i - 1].weight = weights[i];
	}

	std::sort(items, items + (number_items - 1), cmp_);
	robin_hood::unordered_map <int, Node> cache, tmp_cache;
	
	Node u, v;

	u.profit = 0;
	//u.bound = bound_(u, number_items, W, items, 0, 0);
	cache[0] = u;

	int maxProfit = greed_solve_(W, items, number_items);
	int counter = 0;
	float ub = 0;
	int weight = 0;
	for (int n = 0; n < number_items; n++) {
		//std::cout << "----------------------" << std::endl;
		tmp_cache = cache;
		for (const auto& tmp : tmp_cache) {
			v.profit = tmp.second.profit;
			v.bound = bound_(v, number_items, W, items, n, tmp.first);
			if (v.bound > maxProfit){
				weight = items[n].weight + tmp.first;
				if (weight <= W) {
					counter++;
					if (cache.count(weight)) {
						cache[weight].profit = std::max(tmp.second.profit + items[n].value, cache[weight].profit);
					}
					else {
						cache[weight].profit = tmp.second.profit + items[n].value;
					}
					if (cache[weight].profit > maxProfit) {
						maxProfit = cache[weight].profit;
					}

					//std::cout << weight <<" " << cache[weight].profit << std::endl;
				}
			}
		}
	}
	std::cout << counter << std::endl;

	return maxProfit;

}