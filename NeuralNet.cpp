#include "NeuralNet.h"

NeuralNet::~NeuralNet()
{
	/*delete whi;
	delete whh;
	delete who;*/
}

void NeuralNet::mutate(float mr)
{
	whi.mutate(mr);
	whh.mutate(mr);
	who.mutate(mr);
}

 MatrixXf NeuralNet::output(float *inputsArr)
{
	MatrixHandler inputs = who.singleColumnMatrixFromArray(inputsArr);

	MatrixHandler inputsBias = inputs.addBias();

	MatrixHandler hiddenInputs = whi.dot(inputsBias);

	MatrixHandler hiddenOutputs = hiddenInputs.activate();

	MatrixHandler hiddenOutputsBias = hiddenOutputs.addBias();

	MatrixHandler hiddenInputs2 = whh.dot(hiddenOutputsBias);
	MatrixHandler hiddenOutputs2 = hiddenInputs2.activate();
	MatrixHandler hiddenOutputsBias2 = hiddenOutputs2.addBias();

	MatrixHandler outputInputs = who.dot(hiddenOutputsBias2);

	MatrixHandler outputs = outputInputs.activate();

	return outputs.matrix;
}

NeuralNet NeuralNet::crossover(NeuralNet partner)
{
	NeuralNet child = NeuralNet(iNodes, hNodes, oNodes);
	child.whi = whi.crossover(partner.whi);
	child.whh = whh.crossover(partner.whh);
	child.who = who.crossover(partner.who);
	return child;
}

NeuralNet NeuralNet::clone()
{
	NeuralNet clone = NeuralNet(iNodes, hNodes, oNodes);
	clone.whi = whi.clone();
	clone.whh = whh.clone();
	clone.who = who.clone();

	return clone;
}

bool NeuralNet::writeRecordToFile(std::string file_name)
{
	float* inpArr = whi.toArray();
	int inpSize = whi.rows * whi.cols;
	
	float* hidArr = whh.toArray();
	int hidSize = whh.rows * whh.cols;

	float* outArr = who.toArray();
	int outSize = who.rows * who.cols;

	std::ofstream file;
	file.open(file_name);
	for (int count = 0; count < inpSize; count++) {
		file << inpArr[count] << " ";
	}
	file << '\n';
	for (int count = 0; count < hidSize; count++) {
		file << hidArr[count] << " ";
	}
	file << '\n';
	for (int count = 0; count < outSize; count++) {
		file << hidArr[count] << " ";
	}
	file.close();
	return true;
}

bool NeuralNet::readFiletoNetwork(std::string file_name)
{
	float* inpArr = whi.toArray();
	int inpSize = whi.rows * whi.cols;

	float* hidArr = whh.toArray();
	int hidSize = whh.rows * whh.cols;

	float* outArr = who.toArray();
	int outSize = who.rows * who.cols;

	std::ifstream file(file_name);
	if (file.is_open()) {
		while (!file.eof()) {
			for (int i = 0; i < inpSize; i++) {
				file >> inpArr[i];
			}
			whi.fromArray(inpArr);
			for (int i = 0; i < hidSize; i++) {
				file >> inpArr[i];
			}
			whh.fromArray(hidArr);
			for (int i = 0; i < outSize; i++) {
				file >> inpArr[i];
			}
			who.fromArray(outArr);
		}
		return true;
	}
	else {
		std::cout << "Did not open file";
		return false;
	}
}
