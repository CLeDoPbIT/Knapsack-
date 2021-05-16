#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <cmath>
#include <math.h>
#include <fstream>
#include <vector>
#include <ctime>
#include <chrono>
#include <stack>
#include <locale.h>
#include "classical_bnb.h"

Classical_BnB_Solver::Classical_BnB_Solver(std::vector<int> tmp_values, std::vector<int> tmp_weights, int tmp_W, int tmp_number_items)
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

bool cmp(Item a, Item b)
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
	int level, profit, bound;
	float weight;
};



int bound(Node u, int n, int W, Item arr[])
{
	// if weight overcomes the knapsack capacity, return
	// 0 as expected bound
	if (u.weight >= W)
		return 0;

	// initialize bound on profit by current profit
	int profit_bound = u.profit;

	// start including items from index 1 more to current
	// item index
	int j = u.level+1;
	int totweight = u.weight;

	// checking index condition and knapsack capacity
	// condition
	while ((j < n) && (totweight + arr[j].weight <= W))
	{
		totweight += arr[j].weight;
		profit_bound += arr[j].value;
		j++;
	}

	// If k is not n, include last item partially for
	// upper bound on profit
	if (j < n)
		profit_bound += (W - totweight) * arr[j].value / arr[j].weight;

	return profit_bound;
}


int greed_solve(int W, Item* items, int number_items) {
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


std::pair <long, long> Classical_BnB_Solver::solve() {
	Item* items = 0;
	items = new Item[number_items];
	for (int i = 1; i <= number_items; i++) {
		items[i-1].value = values[i];
		items[i-1].weight = weights[i];
	}

	std::sort(items, items+ (number_items-1), cmp);
	
	std::stack <Node> queue;
	Node u, v;

	u.level = -1;
	u.profit = u.weight = 0;
	queue.push(u);

	int maxProfit = greed_solve(W, items, number_items);
	int counter = 0;
	std::time_t startTime = time(0);

	while ((!queue.empty()) && (time(0) < startTime + 15))
	{
		// Dequeue a node
		
		u = queue.top();
		queue.pop();

		// If it is starting node, assign level 0
		if (u.level == -1)
			v.level = 0;

		// If there is nothing on next level
		if (u.level == number_items - 1)
			continue;
		v.level = u.level + 1;
		counter++;
		// Do the same thing, but Without taking
		// the item in knapsack
		v.weight = u.weight;
		v.profit = u.profit;
		v.bound = bound(v, number_items, W, items);
		if (v.bound > maxProfit)
			queue.push(v);


		// Else if not last node, then increment level,
		// and compute profit of children nodes.

		// Taking current level's item add current
		// level's weight and value to node u's
		// weight and value
		v.weight = u.weight + items[v.level].weight;
		v.profit = u.profit + items[v.level].value;

		// If cumulated weight is less than W and
		// profit is greater than previous profit,
		// update maxprofit
		if (v.weight <= W && v.profit > maxProfit)
			maxProfit = v.profit;

		// Get the upper bound on profit to decide
		// whether to add v to Q or not.
		v.bound = bound(v, number_items, W, items);

		// If bound value is greater than profit,
		// then only push into queue for further
		// consideration
		if (v.bound > maxProfit)
			queue.push(v);

	}
	//std::cout << counter << std::endl;
	if (time(0) > startTime + 15) {
		return std::make_pair(0, 0);
	}
	else {
		return std::make_pair(maxProfit, counter);
	}

}