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
#include <numeric>
#include <random>
#include <chrono>
#include <locale.h>
#include <fstream>
#include <cstdlib>
#include "problem.h"
#include "polyreg_2.h"
#include "poly_vector.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <algorithm>


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
void Problem::generate_problem(int n, int R, int k, int number_instance, std::string type_generator){
	std::random_device                  rand_dev;
	number_items = n;
	values.push_back(n);
	weights.push_back(0);

	if (type_generator == "uncorellated"){
		std::uniform_int_distribution<int>  distr(1, R);
		std::mt19937                        generator(rand_dev());
		for (int i = 0; i < n; i++) {
			values.push_back(distr(generator));
			weights.push_back(distr(generator));
		}
	}
	else if(type_generator == "weakly_correlated") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  dist_delta(R/(double)20, (3*R) / (double)20);
		int delta = dist_delta(generator);
		std::uniform_int_distribution<int>  distr_w(1, R);
		
		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			std::uniform_int_distribution<int>  distr_v(std::max(1, weights[i+1]- delta), weights[i+1] + delta);
			values.push_back(distr_v(generator));
		}
	}
	else if (type_generator == "strongly_correlated") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
		int delta = dist_delta(generator);
		std::uniform_int_distribution<int>  distr_w(1, R);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			values.push_back(weights[i+1] + delta);
		}
	}
	else if (type_generator == "subset_sum") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_w(1, R);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			values.push_back(weights[i+1]);
		}
	}
	else if (type_generator == "inverse_strongly_correlated") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_v(1, R);
		std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
		int delta = dist_delta(generator);


		for (int i = 0; i < n; i++) {
			values.push_back(distr_v(generator));
			weights.push_back(values[i+1] + delta);
		}
	}
	else if (type_generator == "almost_strongly_correlated") {
		std::mt19937                        generator(rand_dev());

		std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
		int delta = dist_delta(generator);
		std::uniform_int_distribution<int>  dist_epsilon(R / (double)500, (3 * R) / (double)500);
		int epsilon = dist_epsilon(generator);

		std::uniform_int_distribution<int>  distr_w(1, R);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			std::uniform_int_distribution<int>  distr_v(std::max(1, weights[i+1] + delta - epsilon), weights[i+1] + delta + epsilon);
			values.push_back(distr_v(generator));
		}
	}
	else if (type_generator == "similar_uncorrelated_weights") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_v(1, R);
		
		std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
		int delta = dist_delta(generator);
		std::uniform_int_distribution<int>  distr_w(R-delta, R);
		for (int i = 0; i < n; i++) {
			values.push_back(distr_v(generator));
			weights.push_back(distr_w(generator));
		}
	}
	else if (type_generator == "even_odd_strongly_correlated") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_w(1, R/(double)2);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator)*2);
			values.push_back(weights[i+1] + R/(double)10);
		}
	}
	else if (type_generator == "even_odd_subset_sum") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_w(1, R / (double)2);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator) * 2);
			values.push_back(weights[i+1]);
		}
	}
	else if (type_generator == "no_small_weights") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_w((R / (double)2) + 1, R);

		std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
		int delta = dist_delta(generator);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			std::uniform_int_distribution<int>  distr_v(std::max(1, weights[i+1] - delta), weights[i+1] + delta);
			values.push_back(distr_v(generator));
		}
	}
	else if (type_generator == "spanner_uncorrelated") {
		int v = 2;
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_spanner_set(1, R);
		std::uniform_int_distribution<int>  distr_multyply_limit(5, 15);
		double m = distr_multyply_limit(generator);
		int* spanner_set_weight = 0;
		int* spanner_set_value = 0;
		spanner_set_weight = new int[v];
		spanner_set_value = new int[v];

		for (int i = 0; i < v; i++) {
			spanner_set_weight[i] = distr_spanner_set(generator);
			spanner_set_weight[i] = std::ceil(2* spanner_set_weight[i]/m);
			spanner_set_value[i] = distr_spanner_set(generator);
			spanner_set_value[i] = std::ceil(2 * spanner_set_value[i] / m);
		}

		std::uniform_int_distribution<int>  dist_choose(0, v-1);
		std::uniform_int_distribution<int>  dist_multyplier(1, m);

		for (int i = 0; i < n; i++) {
			int id_from_spanner = dist_choose(generator);
			int multyplier = dist_multyplier(generator);
			weights.push_back(spanner_set_weight[id_from_spanner]* multyplier);
			values.push_back(spanner_set_value[id_from_spanner] * multyplier);
		}
	}
	else if (type_generator == "spanner_weakly_correlated") {
	int v = 2;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr_spanner_set_w(1, R);
	std::uniform_int_distribution<int>  distr_multyply_limit(5, 15);
	double m = distr_multyply_limit(generator);
	int* spanner_set_weight = 0;
	int* spanner_set_value = 0;
	spanner_set_weight = new int[v];
	spanner_set_value = new int[v];
	std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
	int delta = dist_delta(generator);

	for (int i = 0; i < v; i++) {
		spanner_set_weight[i] = distr_spanner_set_w(generator);
		std::uniform_int_distribution<int>  distr_spanner_set_v(std::max(1, spanner_set_weight[i] - delta), spanner_set_weight[i] + delta);
		spanner_set_weight[i] = std::ceil(2 * spanner_set_weight[i] / m);
		spanner_set_value[i] = distr_spanner_set_v(generator);
		spanner_set_value[i] = std::ceil(2 * spanner_set_value[i] / m);
	}

	std::uniform_int_distribution<int>  dist_choose(0, v - 1);
	std::uniform_int_distribution<int>  dist_multyplier(1, m);

	for (int i = 0; i < n; i++) {
		int id_from_spanner = dist_choose(generator);
		int multyplier = dist_multyplier(generator);
		weights.push_back(spanner_set_weight[id_from_spanner] * multyplier);
		values.push_back(spanner_set_value[id_from_spanner] * multyplier);
	}
	}
	else if (type_generator == "spanner_strongly_correlated") {
	int v = 2;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr_spanner_set_w(1, R);
	std::uniform_int_distribution<int>  distr_multyply_limit(5, 15);
	double m = distr_multyply_limit(generator);
	int* spanner_set_weight = 0;
	int* spanner_set_value = 0;
	spanner_set_weight = new int[v];
	spanner_set_value = new int[v];
	std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
	int delta = dist_delta(generator);

	for (int i = 0; i < v; i++) {
		spanner_set_weight[i] = distr_spanner_set_w(generator);
		spanner_set_value[i] = spanner_set_weight[i] + delta;
		spanner_set_weight[i] = std::ceil(2 * spanner_set_weight[i] / m);
		spanner_set_value[i] = std::ceil(2 * spanner_set_value[i] / m);
	}

	std::uniform_int_distribution<int>  dist_choose(0, v - 1);
	std::uniform_int_distribution<int>  dist_multyplier(1, m);

	for (int i = 0; i < n; i++) {
		int id_from_spanner = dist_choose(generator);
		int multyplier = dist_multyplier(generator);
		weights.push_back(spanner_set_weight[id_from_spanner] * multyplier);
		values.push_back(spanner_set_value[id_from_spanner] * multyplier);
	}
	}
	else if (type_generator == "multiple_correlated") {
		std::uniform_int_distribution<int>  distr_w(1, R);
		std::uniform_int_distribution<int>  distr_f(3, 10);
		std::mt19937                        generator(rand_dev());
		int f = distr_f(generator);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			if (weights[i+1] % f == 0) {
				values.push_back(weights[i+1]+3*R/(double)10);
			}
			else {
				values.push_back(weights[i+1] + 2 * R / (double)10);
			}
		}
	}
	else if (type_generator == "profit_ceiling") {
		std::uniform_int_distribution<int>  distr_w(1, R);
		std::mt19937                        generator(rand_dev());

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			values.push_back(3 * std::ceil(weights[i]/(double)3));
		}
	}
	else if (type_generator == "circle") {
		std::uniform_int_distribution<int>  distr_w(1, R);
		std::mt19937                        generator(rand_dev());

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			values.push_back(2/(double)3 * std::sqrt(4*std::pow(R,2)-std::pow((weights[i+1]-2*R),2)));
		}
	}
	else if (type_generator == "quadratic_fit") {
		std::uniform_int_distribution<int>  distr(1, R);
		std::mt19937                        generator(rand_dev());
		Vector <double> dummy;
		Vector<double>dani;

		for (int i = 0; i < int(n / (double)10); i++) {
			dummy.v.push_back({ double(distr(generator)),double(distr(generator) )});
		}

		dani = PolynomialRegression(dummy.v, 3);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr(generator));
			values.push_back(distr(generator));
			values[i+1] = std::max(int(dani.v[0][0] + dani.v[1][0] * values[i+1] + dani.v[2][0] * std::pow(values[i+1], 2)), 1);
		}


	}
	else if (type_generator == "cubic_fit") {
	std::uniform_int_distribution<int>  distr(1, R);
	std::mt19937                        generator(rand_dev());
	Vector <double> dummy;
	Vector<double>dani;

	for (int i = 0; i < int(n / (double)10); i++) {
		dummy.v.push_back({ double(distr(generator)),double(distr(generator)) });
	}

	dani = PolynomialRegression(dummy.v, 4);

	for (int i = 0; i < n; i++) {
		weights.push_back(distr(generator));
		values.push_back(distr(generator));
		values[i + 1] = std::max(int(dani.v[0][0] + dani.v[1][0] * values[i + 1] + dani.v[2][0] * std::pow(values[i + 1], 2) + dani.v[3][0] * std::pow(values[i + 1], 3)), 1);
	}


	}
	else if (type_generator == "random_ceiling") {
		std::uniform_int_distribution<int>  distr(1, R);
		std::uniform_int_distribution<int>  distr_q(0, 2);
		std::mt19937                        generator(rand_dev());
		int q = 0;
		for (int i = 0; i < n; i++) {
			weights.push_back(distr(generator));
			q = distr_q(generator);
			values.push_back(weights[i+1]+q);
		}
	}
	else if (type_generator == "profit_ceiling") {
	std::uniform_int_distribution<int>  distr_w(int(R/(double)2)+1, R);
	std::mt19937                        generator(rand_dev());

	for (int i = 0; i < n; i++) {
		weights.push_back(distr_w(generator));
		values.push_back(3 * std::ceil(weights[i] / (double)3));
	}
	}



	float coef = (k / (float)(number_instance + 1)); //;
	W = (coef)* std::accumulate(weights.begin(), weights.end(), 0);
	weights[0] = W;

	std::ofstream myfile;
	myfile.open("C:/Users/EBurashnikov/source/repos/Knapsack/Knapsack/data_generated/" + type_generator +"/file_"+ std::to_string(k)+".txt");
	myfile << number_items << " " << W << std::endl;

	for (int i = 1; i <= number_items; i++) {
		myfile << values[i] << " " << weights[i] << std::endl;
	}
	myfile.close();
		
}
void Problem::clear() {
	values.clear();
	weights.clear();
}
