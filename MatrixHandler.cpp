#include "MatrixHandler.h"

void MatrixHandler::output()
{
	for (int i = 0; i < rows;i++) {
		for (int j = 0; j < cols; j++) {
			std::cout << std::to_string(matrix.coeff(i, j)) << " ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void MatrixHandler::multiply(float n)
{
	matrix *= n;
}

 MatrixHandler MatrixHandler::dot(MatrixHandler n)
{
	 MatrixHandler result = MatrixHandler(rows, n.cols);
	 result.matrix = matrix* n.matrix;
	return result;
}

void MatrixHandler::randomize()
{
	matrix = MatrixXf::Random(rows, cols);

}

void MatrixHandler::Add(float n)
{
	matrix = matrix.array() + n;
}

MatrixHandler MatrixHandler::add(MatrixHandler n)
{
	MatrixHandler newMatrix = MatrixHandler(rows, cols);
	newMatrix.matrix = matrix + n.matrix;
	return newMatrix;
}

MatrixHandler MatrixHandler::subtract(MatrixHandler n)
{
	MatrixHandler newMatrix = MatrixHandler(rows, cols);
	newMatrix.matrix = matrix - n.matrix;
	return newMatrix;
}

MatrixHandler MatrixHandler::transpose()
{
	MatrixHandler n = MatrixHandler(cols, rows);
	n.matrix = matrix.transpose();
	return n;
}

MatrixHandler MatrixHandler::singleColumnMatrixFromArray(float *arr)
{
	int size = 24;//sizeof(arr) / sizeof(arr[0]);
	MatrixHandler n = MatrixHandler(size, 1);
	for (int i = 0; i < size; i++) {
		n.matrix(i, 0) = arr[i];
	}
	return n;
}

void MatrixHandler::fromArray(float* arr)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			matrix(i, j) = arr[j + i * cols];
		}
	}
}

float* MatrixHandler::toArray()
{
	float* arr = new float[rows*cols]();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols;j++) {
			arr[j + i * cols] = matrix(i, j);
		}
	}

	return arr;
}

MatrixHandler MatrixHandler::addBias()
{
	MatrixHandler n = MatrixHandler(rows + 1, 1);
	for (int i = 0; i < rows; i++) {
		n.matrix(i, 0) = matrix(i, 0);
	}
	n.matrix(rows, 0) = 1;
	return n;
}

MatrixHandler MatrixHandler::activate()
{
	MatrixHandler n = MatrixHandler(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols;j++) {
			n.matrix(i, j) = relu(matrix(i, j));
		}
	}
	return n;
}

float MatrixHandler::sigmoid(float x)
{
	float y = 1 / (1 + exp(-x));
	return y;
}

float MatrixHandler::relu(float x)
{
	if (x > 0)
		return x;
	else return 0;
}


MatrixHandler MatrixHandler::sigmoidDerived()
{
	MatrixHandler n = MatrixHandler(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			n.matrix(i, j) = (matrix(i, j) * (1 - matrix(i, j)));
		}
	}
	return n;
}

MatrixHandler MatrixHandler::removeBottomLayer()
{
	MatrixHandler n = MatrixHandler(rows - 1, cols);
	for (int i = 0; i < n.rows;i++) {
		for (int j = 0; j < cols; j++) {
			n.matrix(i, j) = matrix(i, j);
		}
	}
	return n;
}

void MatrixHandler::mutate(float mutationRate)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			float r = (rand() % 100);
			if (r < mutationRate) {
				matrix(i, j) += rand() / 5;

				if (matrix(i, j) > 1) matrix(i, j) = 1;
				if (matrix(i, j) < -1) matrix(i, j) = -1;
			}
		}
	}
}

MatrixHandler MatrixHandler::crossover(MatrixHandler partner)
{
	MatrixHandler child = MatrixHandler(rows, cols);

	int randC = floor(rand() % cols + 1);
	int randR = floor(rand() % rows + 1);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			double r = ((double)rand() / (RAND_MAX));
			//std::cout << r << '\n';
			if(r < 0.5){ 
				child.matrix(i, j) = matrix(i, j);
			}
			else {
				child.matrix(i, j) = partner.matrix(i, j);
			}
		}
	}
	return child;
}

MatrixHandler MatrixHandler::clone()
{
	MatrixHandler clone = MatrixHandler(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			clone.matrix(i, j) = matrix(i, j);
		}
	}
	return clone;
}
