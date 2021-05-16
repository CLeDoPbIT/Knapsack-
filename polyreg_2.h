#include <vector>
#include <iostream>
#include "poly_vector.h"

Vector<double> PolynomialRegression(
	const std::vector<std::vector<double>>& Pts,			// the points we use
	const int& order)		// the coefficient we pretend to generate
{
	// Calculate the half part of A (Matrix)
	Vector<double> A(order);
	for (int i = 0; i < order; i++) {
		for (int j = i; j < order; j++) {
			for (int k = 0; k < Pts.size(); k++) {
				A.v[i][j] += pow(Pts[k][0], i) * pow(Pts[k][0], j);
			}
			A.v[j][i] = A.v[i][j];
		}
	}

	// Pivotate the Matrix to fill the rest part
	Vector<double>C(order, 1);
	for (int i = 0; i < order; i++) {
		for (int k = 0; k < Pts.size(); k++) {
			C.v[i][0] += (Pts[k][1] * pow(Pts[k][0], i));
		}
	}

	// A*B=C --> B = A.inverse() * C
	A.Inverse2();
	A *= C;
	return A;
}
