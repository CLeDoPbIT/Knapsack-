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
#include <cstdlib>
#include "problem.h"


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
			std::uniform_int_distribution<int>  distr_v(weights[i]- delta, weights[i] + delta);
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
			values.push_back(weights[i] + delta);
		}
	}
	else if (type_generator == "subset_sum") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_w(1, R);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			values.push_back(weights[i]);
		}
	}
	else if (type_generator == "inverse_strongly_correlated") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_v(1, R);
		std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
		int delta = dist_delta(generator);


		for (int i = 0; i < n; i++) {
			values.push_back(distr_v(generator));
			weights.push_back(values[i] + delta);
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
			std::uniform_int_distribution<int>  distr_v(weights[i] + delta - epsilon, weights[i] + delta + epsilon);
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
			values.push_back(weights[i] + R/(double)10);
		}
	}
	else if (type_generator == "even_odd_subset_sum") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_w(1, R / (double)2);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator) * 2);
			values.push_back(weights[i]);
		}
	}
	else if (type_generator == "no_small_weights") {
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr_w((R / (double)2)+1, R);

		std::uniform_int_distribution<int>  dist_delta(R / (double)20, (3 * R) / (double)20);
		int delta = dist_delta(generator);

		for (int i = 0; i < n; i++) {
			weights.push_back(distr_w(generator));
			std::uniform_int_distribution<int>  distr_v(weights[i] - delta, weights[i] + delta);
			values.push_back(distr_v(generator));
		}
	}






	float coef = (k / (float)(number_instance + 1)); //;
	W = (coef)* std::accumulate(weights.begin(), weights.end(), 0);
	weights[0] = W;


}


