#pragma once
#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <math.h>

using namespace::Eigen;

class MatrixHandler
{
public:
	int rows;
	int cols;
	Eigen::MatrixXf matrix;

	MatrixHandler() {
		//Default constructor
	}

	MatrixHandler(int r, int c) {
		rows = r;
		cols = c;
		matrix(rows, cols);
	}
	MatrixHandler(MatrixXf m) {
		matrix = m;
		rows = m.rows();
		cols = m.cols();
	}

	void output();

	void multiply(float n);

	MatrixHandler dot(MatrixHandler n);

	void randomize();

	void Add(float n);

	MatrixHandler add(MatrixHandler n);

	MatrixHandler subtract(MatrixHandler n);

	MatrixHandler transpose();

	MatrixHandler singleColumnMatrixFromArray(float array[]);

	void fromArray(float* arr);

	float* toArray();

	MatrixHandler addBias();

	MatrixHandler activate();

	float sigmoid(float x);

	MatrixHandler sigmoidDerived();

	MatrixHandler removeBottomLayer();

	void mutate(float mutationRate);

	MatrixHandler crossover(MatrixHandler partner);

	MatrixHandler clone();
};

