#include "NeuralNet.h"

void NeuralNet::mutate(float mr)
{
	whi.mutate(mr);
	whh.mutate(mr);
	who.mutate(mr);
}

/*float**/MatrixXf NeuralNet::output(float *inputsArr)
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
