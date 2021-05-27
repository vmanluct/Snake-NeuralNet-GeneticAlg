#pragma once
#include <Eigen/Dense>
#include "MatrixHandler.h"

using namespace Eigen;

class NeuralNet
{
public:
	int iNodes; //Number of input nodes
	int hNodes; // Number of hidden nodes
	int oNodes; //Number of outputs nodes

	MatrixHandler whi; //Weight matrix for weights between input and hidden nodes
	MatrixHandler whh; //Weight matrix for weights between hidden nodes and second layer hidden nodes
	MatrixHandler who; //Weight matrix weights between second hidden layer and output nodes;

	NeuralNet() {
		//Default Constructor
	}

	NeuralNet(int inputs, int hiddenNo, int outputNo) {
		iNodes = inputs;
		oNodes = outputNo;
		hNodes = hiddenNo;

		whi = MatrixHandler(hNodes, iNodes + 1);

		whh = MatrixHandler(hNodes, hNodes + 1);

		who = MatrixHandler(oNodes, hNodes + 1);

		whi.randomize();
		whh.randomize();
		who.randomize();
	}

	void mutate(float mr);

	float* output(float *inputsArr);

	NeuralNet crossover(NeuralNet Partner);

	NeuralNet clone();
};

